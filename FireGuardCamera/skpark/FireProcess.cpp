#include "stdAfx.h"

#include "FireProcess.h"
#include "TraceLog.h"
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
,monitor_sec(0)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;
	char buf[2048];
	memset(buf, 0x00, 2048);
	GetPrivateProfileString("FIRE_WATCH", "IP", "127.0.0.1", buf, 2048, iniPath);
	_ipAddress = buf;

	GetPrivateProfileString("FIRE_WATCH", "PORT", "14152", buf, 2048, iniPath);
	_port = atoi(buf);

	GetPrivateProfileString("FIRE_WATCH", "ID", "admin", buf, 2048, iniPath);
	id = buf;

	GetPrivateProfileString("FIRE_WATCH", "PWD", "coxcoxcox!", buf, 2048, iniPath);
	pwd = buf;

	GetPrivateProfileString("FIRE_WATCH", "MONITOR_SEC", "1", buf, 2048, iniPath);
	monitor_sec = atoi(buf);


	TraceLog(("FireProcess()\n"));
}
FireProcess::~FireProcess()
{
	m_pThread  = 0;
	m_pSimulatorThread = 0;
	Clear();
}
void FireProcess::Clear()
{
	_lock.Lock();
	_isAlive = false;
	_list.clear();
	_lock.Unlock();
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

bool  FireProcess::Start()
{
	TraceLog(("Start()"));
	m_pThread = AfxBeginThread(FireProcess::ProcessEvent, NULL);
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

bool FireProcess::Dialog(FireData& data)
{
	CString  stream;
	stream.Format("%s/%.2f/%d/0", data.serialNo, data.temperature, data.level);

	//TraceLog(("isFire:%s", stream));

	std::string result, errMsg;
	bool retval = sockUtil::getInstance()->dialog(_ipAddress, _port, 1, stream, result, errMsg);

	if (!retval)
	{
		TraceLog(("dialog failed(%s)", errMsg));
	}
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

