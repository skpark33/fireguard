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
#define WM_TEMPERATURE_ALARM 1049

#define EXTERNAL_MSG_PREFIX    "external/"

class CFireGuardCameraDlg;

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

	void SetDlg(CFireGuardCameraDlg* dlg) { _dlg = dlg; }

	CString id;
	CString pwd;
	int monitor_sec;

protected:
	FireProcess();
	static FireProcess*	_instance;

	CFireGuardCameraDlg*	_dlg;

	CString _errStr;
	FireDataList		_list;
	CCriticalSection	_lock;

	list<CString>		_exlist;
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

	//CCriticalSection	_popupLock;
	//list<int>	_popupList;

//	void _pushPopup(int cameraId);
public:
//	int popPopup();
	

};

#endif // _FireProcess_h_