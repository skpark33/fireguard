#ifndef _FireProcess_h_
#define _FireProcess_h_
#include "stdafx.h"

#include <cstdio>
#include <string>
#include <list>
#include <map>
#include <fstream>

#define MAX_CAMERA 8
#define MAX_SIM_DATA  360



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

	virtual ~FireProcess();
	const char*	getLastError() { return _errStr; }
	void	Push(float temperature, int level, LPCTSTR serialNo);
	void Clear();
	bool  Pop(FireData&  data);
	bool Start();
	bool ProcessEvent();
	bool Dialog(FireData& data);
	bool IsAlive() { return _isAlive;  }

	// simulator
	void StartSimulator(LPCTSTR serialNo);
	float GenerateTemperature();
	static UINT  GenerateData(LPVOID pParam);

	CString id;
	CString pwd;
	int monitor_sec;

protected:
	FireProcess();
	static FireProcess*	_instance;

	CString _errStr;
	FireDataList		_list;
	CCriticalSection	_lock;

	CWinThread*  m_pThread;
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