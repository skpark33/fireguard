/* 이 파일은  GuardianCenter 과 fireGuardGraph  프로젝트에서 공통으로 쓰이는 파일이다. 항상 동일해야 한다.*/
#ifndef _FireProcess_h_
#define _FireProcess_h_
#include "stdafx.h"

#include <cstdio>
#include <string>
#include <list>
#include <set>
#include <map>
#include <fstream>

#define MAX_CAMERA 8
#define MAX_SIM_DATA  360
#define EXTERNAL_MSG_PREFIX    "external/"

#ifndef __GUARDIAN_CENTER__
using namespace std;
#endif

class RECEIVER_INFO {
public:
	CString name;
	CString ip;
	CString port;
};

class FireData
{
public:
	FireData() : temperature(0.0), level(0), serialNo("") {}
	FireData(float t, int l, LPCTSTR h) : temperature(t), level(l), serialNo(h) {}
	float temperature;
	int level;
	CString serialNo;
};

typedef  list<FireData>	 FireDataList;

class FireProcess {
public:

	static FireProcess*	getInstance();
	static void	clearInstance();
	static UINT  ProcessEvent(LPVOID pParam);
	static UINT  exProcessEvent(LPVOID pParam);
	
	std::map<CString, RECEIVER_INFO> receiverMap; 
	std::map<int, CString>  thresholdMap;
	CString trendThreshold;

	void GetInfoFromIni();
	int m_foundedCount;

	virtual ~FireProcess();
	const char*	getLastError() { return _errStr; }
	
	void	Push(float temperature, int level, LPCTSTR serialNo);
	void	exPush(CString stream);
	
	void Clear();
	void exClear();
	bool  Pop(FireData&  data);
	bool  exPop(CString&  data);
	bool Start();
	bool ProcessEvent();
	bool Dialog(FireData& data);
	bool exDialog(CString& data);
	bool IsAlive() { return _isAlive;  }

	// simulator
	void StartSimulator(LPCTSTR serialNo);
	float GenerateTemperature();
	static UINT  GenerateData(LPVOID pParam);

protected:
	FireProcess();
	static FireProcess*	_instance;

	CString _errStr;
	FireDataList		_list;
	CCriticalSection	_lock;

	std::list<CString>		_exlist;
	CCriticalSection	_exlock;

	CWinThread*  m_pThread;
	CWinThread*  m_pExThread;
	CWinThread*  m_pSimulatorThread;
	CString _ipAddress;
	int _port;
	bool _isAlive;
	
	bool	_isStart;

	//simulator
	CString _serialNo;
	float _simArray[MAX_CAMERA][MAX_SIM_DATA];
};

#endif // _FireProcess_h_