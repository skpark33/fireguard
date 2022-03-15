#include "stdafx.h"
#include "afxmt.h"
#include "LogManager.h"
#include "sockUtil.h"
#include "chartdir.h"

#include "skpark/TraceLog.h"

#define  HEADER  "Event time, CameraId, Temperature\n"

LogManager::LogManager(LPCTSTR prefix)
	: _dailyLog(NULL)
	, _dataArray(NULL)
	, _timeArray(NULL)
	, _prefix(prefix)
{
	_year = _month = _day = _hour = _min = _sec = -1;
	_cameraIdx = 0;
	TraceLog(("LogManager()"));
}

LogManager::~LogManager() {
	TraceLog(("~LogManager()"));
	destroy();
}


void
LogManager::destroy() 
{
	TraceLog(("destroy()"));
	CloseLog();
	ClearLog();
}

bool LogManager::OpenLogForWrite(LPCTSTR day, LPCTSTR sec)
{
	static CString prevFile = "";

	if ((!_logFullPath.IsEmpty() && prevFile != _logFullPath))
	{
		ciGuard aLock(_fileLock);
		TraceLog(("FileChange(%s)", _logFullPath));
		if (_dailyLog)
		{
			fclose(_dailyLog);
			_dailyLog = 0;
		}
		CString mode = "w";
		if (IsLocalExist(_logFullPath))
		{
			mode = "a";
		}
		_dailyLog = _fsopen(_logFullPath, mode, _SH_DENYNO);
		if (_dailyLog)
		{
			prevFile = _logFullPath;
			fputs(HEADER, _dailyLog);
			fflush(_dailyLog);
		}
		else
		{
			TraceLog(("file open failed(%s)", _logFullPath));
			return false;
		}
	}
	return true;
}

bool LogManager::OpenLogForRead()
{
	if (_dailyLog)
	{
		return true;
	}
	if (_logFullPath.IsEmpty())
	{
		TraceLog(("fullpath is empty"));
		return false;
	}
	if (!IsLocalExist(_logFullPath))
	{
		TraceLog(("fullpath is not exist(%s)", _logFullPath));
		return false;
	}
	ciGuard aLock(_fileLock);
	_dailyLog = _fsopen(_logFullPath, "r", _SH_DENYNO);
	if(!_dailyLog)
	{
		TraceLog(("file open error(%s)", _logFullPath));
		return false;
	}
	TraceLog(("%s file opened", _logFullPath));
	return true;
}

void LogManager::CloseLog()
{
	if (_dailyLog)
	{
		fclose(_dailyLog);
		_dailyLog = 0;
	}
}
void LogManager::SaveLog(LPCTSTR serialNo, double temper)
{
	CTime now = CTime::GetCurrentTime();
	CString day, sec;

	day.Format("%04d%02d%02d", now.GetYear(), now.GetMonth(), now.GetDay());
	sec.Format("%02d%02d%02d", now.GetHour(), now.GetMinute(), now.GetSecond());

	if (_logFullPath.IsEmpty())
	{
		_logFullPath.Format("%s%s%s.csv", UBC_UPLOADER_PATH, _prefix, day);
	}

	OpenLogForWrite(day,sec);
	if (_dailyLog)
	{
		CString dailyData;
		dailyData.Format("%s%s,%s,%.2f\n", day, sec, serialNo, temper);
		ciGuard aLock(_fileLock);
		fputs(dailyData, _dailyLog);
		fflush(_dailyLog);
	}
}

int LogManager::LoadLog(int cameraIdx)
{
	OpenLogForRead();
	if (!_dailyLog)
	{
		CloseLog();
		return 0;
	}

	TraceLog(("Read Start"));
	if (_dataArray)
	{
		if (_dataArray->data)
		{
			delete[] _dataArray->data;
		}
		delete _dataArray;
		_dataArray = 0;
	}
	if (_timeArray)
	{
		if (_timeArray->data)
		{
			delete[] _timeArray->data;
		}
		delete _timeArray;
		_timeArray = 0;
	}
	const int maxLength = 64;
	char buf[maxLength + 1];
	memset(buf, 0x00, maxLength + 1);
	ciGuard aLock(_fileLock);

	int dataCount = 0;
	while (fgets(buf, maxLength, _dailyLog) != NULL && MAX_LOG_COUNT > dataCount)
	{
		CString dataLine = buf;
		TraceLog(("data line=(%s)", buf));

		int pos = 0;
		CString dateTime = dataLine.Tokenize(",", pos);
		if (dateTime.IsEmpty() || dateTime.GetLength() < 14)
		{
			TraceLog(("Parsing Error 1, maybe header ?(%s)", buf));
			memset(buf, 0x00, maxLength + 1);
			continue;
		}
		CString serialNo = dataLine.Tokenize(",", pos);
		if (serialNo.IsEmpty())
		{
			TraceLog(("Parsing Error 2(%s)", buf));
			memset(buf, 0x00, maxLength + 1);
			continue;
		}
		int idx = atoi(serialNo);
		if (idx != cameraIdx)
		{
			memset(buf, 0x00, maxLength + 1);
			continue;
		}
		CString temperStr = dataLine.Tokenize(",", pos);
		if (temperStr.IsEmpty())
		{
			TraceLog(("Parsing Error 3(%s)", buf));
			memset(buf, 0x00, maxLength + 1);
			continue;
		}

		int		year			= atoi(dateTime.Mid(0, 4));
		int		month		= atoi(dateTime.Mid(4, 2));
		int		day			= atoi(dateTime.Mid(6, 2));
		int		hour			= atoi(dateTime.Mid(8, 2));
		int     min			= atoi(dateTime.Mid(10, 2));
		int		sec			= atoi(dateTime.Mid(12, 2));

		double chartTime = Chart::chartTime(year, month, day, hour, min, sec);
		double* dtime;
		if (_timeArray == NULL)
		{
			_timeArray = new DoubleArray();
		}

		if (_timeArray->data)
		{
			dtime = (double*)_timeArray->data;
		}
		else
		{
			dtime = new double[MAX_LOG_COUNT];
			_timeArray->data = dtime;
		}
		dtime[dataCount] = chartTime;
		_timeArray->len = (dataCount + 1);


		double val = atof(temperStr);
		double* data;
		if (_dataArray == NULL)
		{
			_dataArray = new DoubleArray();
		}

		if (_dataArray->data)
		{
			data = (double*)_dataArray->data;
		}
		else
		{
			data = new double[MAX_LOG_COUNT];
			_dataArray->data = data;
		}
		data[dataCount] = val;
		_dataArray->len = (dataCount + 1);
		
		
		dataCount++;
	}
	CloseLog();
	return dataCount;
}

void LogManager::ClearLog()
{
	if (_dataArray)
	{
		if (_dataArray->data)
		{
			delete[] _dataArray->data;
			_dataArray->len = 0;
			_dataArray->data = 0;
		}
		delete _dataArray;
		_dataArray = 0;
	}
	if (_timeArray)
	{
		if (_timeArray->data)
		{
			delete[] _timeArray->data;
			_timeArray->len = 0;
			_timeArray->data = 0;
		}
		delete _timeArray;
		_timeArray = 0;
	}
}



