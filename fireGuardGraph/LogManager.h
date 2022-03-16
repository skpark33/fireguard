#pragma once
#include <list>
#include <map>
#include <memblock.h>

////////////////////

class TEMPER_DATA
{
public:
	double temper;
	time_t now;
};

typedef  std::list<CString>		CiStringList;
typedef  std::map<CString, TEMPER_DATA>		TEMPER_MAP;
typedef  std::map<int, DoubleArray*>		LOG_DATA_MAP;

#define _NO_VALUE_		-100.0f
#define MAX_LOG_COUNT   86400

class ciGuard
{
public:
	ciGuard(CCriticalSection& s) { section = &s;  section->Lock(); }
	~ciGuard() { section->Unlock(); }
protected:
	CCriticalSection* section;
};

class LogManager
{
public:
	LogManager(LPCTSTR prefix = "fireGuardGraph");
	virtual ~LogManager();

	void destroy();
	void SaveLog(LPCTSTR serialNo, double temper);
	int  LoadLog(int max_camera);
	DoubleArray*	GetLog() { return _dataArray; }
	DoubleArray*	GetTime() { return _timeArray; }
	DoubleArray*	GetLogMax(int max);
	DoubleArray*	GetLogMin(int min);

	bool OpenLogForWrite(LPCTSTR day, LPCTSTR sec);
	bool OpenLogForRead();

	void CloseLog();
	
	void SetPrefix(LPCTSTR p) { _prefix = p;}
	void SetFullPath(LPCTSTR f) { _logFullPath.Format("%s%s", UBC_UPLOADER_PATH, f); }

	void ClearLog();

	int _year;
	int _month;
	int _day;
	int _hour;
	int _min;
	int _sec;

	CString _prefix;

protected:

	FILE*			_dailyLog;
	CString		_logFullPath;
	int				_cameraIdx;
	CCriticalSection	_fileLock;

	DoubleArray*		_dataArray;
	DoubleArray*		_dataArrayMax;
	DoubleArray*		_dataArrayMin;
	DoubleArray*		_timeArray;
};

