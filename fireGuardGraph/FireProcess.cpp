/* 이 파일은  GuardianCenter 과 fireGuardGraph  프로젝트에서 공통으로 쓰이는 파일이다. 항상 동일해야 한다.*/
#include "stdAfx.h"

#include "FireProcess.h"
#ifndef __GUARDIAN_CENTER__
#include "skpark/TraceLog.h"
#include "skpark/util.h"
#else
#include "TraceLog.h"
#include "util.h"
#endif
#include "sockUtil.h"



FireProcess* 	FireProcess::_instance = 0;

FireProcess*
FireProcess::getInstance() {
	if (!_instance) {
		if (!_instance) {
			_instance = new FireProcess;
		}
	}
	return _instance;
}

void
FireProcess::clearInstance() {
	if (_instance) {
		if (_instance) {
			delete _instance;
			_instance = 0;
		}
	}
}

FireProcess::FireProcess()
:_isStart(false)
, _port(14152)
, _isAlive(true)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;
	char buf[2048];
	memset(buf, 0x00, 2048);
	GetPrivateProfileString("FIREWATCH", "IP", "127.0.0.1", buf, 2048, iniPath);
	_ipAddress = buf;

	GetPrivateProfileString("FIREWATCH", "PORT", "14152", buf, 2048, iniPath);
	_port = atoi(buf);

	GetInfoFromIni();

	TraceLog(("FireProcess()\n"));
}

void FireProcess::GetInfoFromIni()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	char buf[1024];
	memset(buf, 0x00, 1024);
	GetPrivateProfileStringA("SOCK_WATCH_SERVER", "IdList", "", buf, 1024, iniPath);

	CString newList = buf;
	if (newList.IsEmpty()) {
		return;
	}
	receiverMap.clear();
	m_foundedCount = 0;

	int pos = 0;
	CString id = newList.Tokenize(",", pos);
	while (!id.IsEmpty())
	{
		RECEIVER_INFO info;

		CString entry;
		entry.Format("SOCK_WATCH_SERVER_%s", id);

		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "NAME", "", buf, 1024, iniPath);
		info.name = buf;

		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "IP", "", buf, 1024, iniPath);
		info.ip = buf;

		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "PORT", "", buf, 1024, iniPath);
		info.port = buf;


		receiverMap[id] = info;
		//insertItem(id, info.name, info.ip, info.port);
		id = newList.Tokenize(",", pos);
	}

	for (int id = 0; id < MAX_CAMERA; id++) {
		CString iniName;
		if (id == 0) {
			iniName = "MAX_THRESHOLD";
		}
		else {
			iniName.Format("MAX_THRESHOLD%d", id);
		}
		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA("FIRE_WATCH", iniName, "", buf, 1024, iniPath);
		CString threshold = buf;
		thresholdMap[id] = threshold;
		TraceLog(("threshold founded--------------- %d = %s", id, threshold ));
	}

	memset(buf, 0x00, 1024);
	GetPrivateProfileStringA("FIRE_WATCH", "MAX_VELOC_THRESHOLD", "", buf, 1024, iniPath);
	trendThreshold = buf;



}
FireProcess::~FireProcess()
{
	m_pThread = 0;
	m_pExThread = 0;
	m_pSimulatorThread = 0;
	Clear();
	exClear();
}
void FireProcess::Clear()
{
	_lock.Lock();
	_isAlive = false;
	_list.clear();
	_lock.Unlock();
}
void FireProcess::exClear()
{
	_exlock.Lock();
	_exlist.clear();
	_exlock.Unlock();
}


void	FireProcess::Push(float temperature, int level, LPCTSTR serialNo)
{
	_lock.Lock();
	_list.push_back(FireData(temperature, level, serialNo));
	_lock.Unlock();

	if (!_isStart)
	{
		_isStart = Start();
	}

}
void	FireProcess::exPush(CString stream)
{
	_exlock.Lock();
	_exlist.push_back(stream);
	_exlock.Unlock();
#ifndef __GUARDIAN_CENTER__
	if (!_isStart)
	{
		_isStart = Start();
	}
#endif

}



bool  FireProcess::Pop(FireData&  data)
{

	_lock.Lock();
	if (_list.size() == 0)
	{
		_lock.Unlock();
		return false;
	}
	data = _list.front();
	_list.pop_front();
	_lock.Unlock();
	return true;
}

bool  FireProcess::exPop(CString&  data)
{
	_exlock.Lock();
	if (_exlist.size() == 0)
	{
		_exlock.Unlock();
		return false;
	}
	data = _exlist.front();
	_exlist.pop_front();
	_exlock.Unlock();
	return true;
}

bool  FireProcess::Start()
{
	TraceLog(("Start()"));
#ifdef __GUARDIAN_CENTER__
	m_pThread = AfxBeginThread(FireProcess::ProcessEvent, NULL);
#endif
	m_pExThread = AfxBeginThread(FireProcess::exProcessEvent, NULL);
	
	return true;
}

UINT  FireProcess::ProcessEvent(LPVOID pParam)
{
	while (FireProcess::getInstance()->IsAlive())
	{
		FireData aData;
		if (!FireProcess::getInstance()->Pop(aData))
		{
			Sleep(1000);
			continue;
		}
		FireProcess::getInstance()->Dialog(aData);
	}
	return 0;
}

UINT  FireProcess::exProcessEvent(LPVOID pParam)
{
	while (FireProcess::getInstance()->IsAlive())
	{
		CString aData;
		if (!FireProcess::getInstance()->exPop(aData))
		{
			Sleep(1000);
			continue;
		}
		FireProcess::getInstance()->exDialog(aData);
	}
	return 0;
}

bool FireProcess::Dialog(FireData& data)
{
	CString  stream;
	stream.Format("%s/%.2f/%d/0", data.serialNo, data.temperature, data.level);

	TraceLog(("isFire:%s", stream));

	std::string result, errMsg;
	bool retval = sockUtil::getInstance()->dialog(_ipAddress, _port, 1, stream, result, errMsg);

	if (!retval)
	{
		TraceLog(("dialog failed(%s)", errMsg));
	}

	int receiverLen = receiverMap.size();
	TraceLog(("----------------------------receiveMapSize = %d", receiverLen));

	if (receiverLen > 0) {
		CString external = "TEMPE/" + stream;
		external.Append("/");
		int id = atoi(data.serialNo) - 1;
		TraceLog(("threshold--------------------------------id=%d", id));
		CString threshold = thresholdMap[id];
		TraceLog(("threshold--------------------------------%s", threshold));
		external.Append(threshold);
		exPush(external);
	}
	//int  popupLen = strlen("POPUP/");
	//if (result.length() >popupLen + 2 &&    result.substr(0, popupLen) == "POPUP/") {
	//	int cameraId = atoi(result.substr(popupLen, 1).c_str());
	//	_pushPopup(cameraId);
	//}

	return retval;
}

bool FireProcess::exDialog(CString& data)
{
	//TraceLog(("isFire:%s", stream));
	bool retval = true;
	std::string result, errMsg;

	CString buf;
	buf = EXTERNAL_MSG_PREFIX;  // 외부로 보내는 데이터는  external/  표시를 한다.
	buf.Append(LicenseUtil::hostId.c_str());
	buf.Append("/");
	buf.Append(data);

#ifdef __GUARDIAN_CENTER__
	// 외부에 온도 데이터 송신
	if (receiverMap.size() > 0) {
		std::map<CString, RECEIVER_INFO>::iterator jtr;
		for (jtr = receiverMap.begin(); jtr != receiverMap.end(); jtr++){
			RECEIVER_INFO info = jtr->second;
			TraceLog(("external send start"));
			retval &= sockUtil::getInstance()->dialog(info.ip, atoi(info.port), 1, buf, result, errMsg, 1000);
			TraceLog(("external send end  %s ", errMsg.c_str()));
		}
	}
#else
	// 외부에 추세 데이터를 전달
	if (receiverMap.size() > 0) {
		std::map<CString, RECEIVER_INFO>::iterator jtr;
		for (jtr = receiverMap.begin(); jtr != receiverMap.end(); jtr++){
			RECEIVER_INFO info = jtr->second;
			TraceLog(("external send start"));
			retval &= sockUtil::getInstance()->dialog(info.ip, atoi(info.port), 1, buf, result, errMsg, 1000);
			TraceLog(("external send end  %s ", errMsg.c_str()));
		}
	}

#endif
	return retval;
}


//
// simulator 
//
void FireProcess::StartSimulator(LPCTSTR serialNo)
{
	srand(time(NULL));

	for (int i = 0; i < MAX_SIM_DATA; i++)
	{
		float seed = static_cast<float>(i);
		_simArray[0][i] = 20.0f + sinf(seed);
		_simArray[1][i] = 10.0f + cosf(seed);
		_simArray[2][i] = 5.0f + tanf(seed);
		_simArray[3][i] = 30.0f + cosf(seed);
		_simArray[4][i] = 25.0f +((i%130));
		_simArray[5][i] = 11.0f + static_cast<float>(rand() % 10);
		_simArray[6][i] = 50.0f + static_cast<float>(rand() % 30);
		_simArray[7][i] = 70.0f + static_cast<float>(rand() % 100);
	}

	_serialNo = serialNo;
	m_pSimulatorThread = AfxBeginThread(FireProcess::GenerateData, NULL);
}

UINT  FireProcess::GenerateData(LPVOID pParam)
{
	while (FireProcess::getInstance()->IsAlive())
	{
		float temperature = FireProcess::getInstance()->GenerateTemperature();

		FireProcess::getInstance()->Push(temperature, 0, FireProcess::getInstance()->_serialNo);
		{
			Sleep(1000);
			continue;
		}
	}
	return 0;
}


float FireProcess::GenerateTemperature()
{
	int idx = atoi(_serialNo);
	static int jdx = 0;
	if (jdx > MAX_SIM_DATA)
	{
		jdx = 0;
	}
	return _simArray[idx-1][jdx++];
}