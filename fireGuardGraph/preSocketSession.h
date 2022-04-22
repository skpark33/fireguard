#pragma once
#include "LogManager.h"

#define  TEMPER_OVER  0
#define  VELOCITY_OVER  1

class preSocketSession;



class preSocketHandler 
{
public:
	static preSocketHandler*	getInstance();
	static void	clearInstance();

	virtual ~preSocketHandler() {}

	void setSession(preSocketSession* session) { _session=session;}
	void start();
	static UINT run(LPVOID pParam);
	void run();
	void destroy();
	void push(const char* recvMsg);

	void pushCommand(const char* command, int cameraId, int alarmType);
	//CString popCommand();

protected:
	preSocketHandler() : _session(0), _isAlive(true) {}
	static preSocketHandler*		_instance;
	static CCriticalSection			_instanceLock;

	CCriticalSection				_listLock;
	std::list<CString>				_list;
	bool			_isAlive;
	preSocketSession*	_session;
	CWinThread* m_pThread;

	//CCriticalSection				_commandListLock;
	//std::list<CString>				_commandList;


};



class preSocketSession
{
public:
	preSocketSession(int port);
	virtual ~preSocketSession();

	bool init();
	void start();

	static UINT run(LPVOID pParam);
	void run();
	void destroy();

	bool ProcessCmd(const char* recvMsg);

	int GetNames(CiStringList& nameList);
	double GetData(LPCTSTR serialNo, int dType);
	void SetData(LPCTSTR serialNo, double temper, int dType);

	bool IsRealData(CString serialNo)
	{
		return (serialNo != "10" && serialNo != "9");
	}

protected:
	CWinThread* m_pThread;

	bool _isAlive;
	int _serverFd;
	int _serverPort;
	int _monitorSec;

	CCriticalSection	_mapLock;
	TEMPER_MAP     _temperMap;
	TEMPER_MAP     _velociMap;

	LogManager		_logger;
};
