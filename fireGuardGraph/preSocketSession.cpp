#include "stdafx.h"
#include "afxmt.h"
#include "preSocketSession.h"
#include "sockUtil.h"
#include "chartdir.h"

#include "skpark/TraceLog.h"
#if defined(_WIN32)
#   include <winsock2.h>
//  WinSock DLL을 사용할 버전
#   define VERSION_MAJOR         2
#   define VERSION_MINOR         0
#   include <time.h>
#else
#   include <sys/socket.h>       /* for socket(), connect(), send(), and recv() */
#   include <arpa/inet.h>        /* for sockaddr_in and inet_addr() */
#endif

#define BUF_LEN 4096

////////////////////
// preSocketHandler

preSocketHandler* 	preSocketHandler::_instance = 0; 
CCriticalSection 	preSocketHandler::_instanceLock;

preSocketHandler*	
preSocketHandler::getInstance() {
	if(!_instance) {
		ciGuard aGuard(_instanceLock);
		if(!_instance) {
			_instance = new preSocketHandler;
		}
	}
	return _instance;
}

void	
preSocketHandler::clearInstance() {
	if(_instance) {
		ciGuard aGuard(_instanceLock);
		if(_instance) {
			delete _instance;
			_instance =0;
		}
	}
}

void preSocketHandler::destroy()
{
	TraceLog(("preSocketHandler::destroy()"));
	_isAlive = false;
	m_pThread = NULL;
}
void preSocketHandler::push(const char* recvMsg)
{
	TraceLog(("preSocketHandler::push(%s)", recvMsg));
	ciGuard aGuard(_listLock);
	_list.push_back(recvMsg);
	TraceLog(("preSocketHandler::push(%s) end", recvMsg));
}

UINT
preSocketHandler::run(LPVOID pParam) {
	TraceLog(("run()"));
	preSocketHandler*  _this = (preSocketHandler*)pParam;
	_this->run();
	return 0;
}
void preSocketHandler::run()
{
	TraceLog(("preSocketHandler::run()"));
	while(_isAlive) {
		{
			std::string recvMsg = "";
			int len = 0;
			{
				ciGuard aGuard(_listLock);
				len = _list.size();
			}
			if (len == 0){
				::Sleep(1000);
				continue;
			}
			{
				ciGuard aGuard(_listLock);
				recvMsg = _list.front();
				_list.pop_front();
			}
			if(_session) _session->ProcessCmd(recvMsg.c_str());
		}
	}
	delete this;
}
void preSocketHandler::start()
{
	m_pThread = ::AfxBeginThread(preSocketHandler::run, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pThread->m_bAutoDelete = TRUE;
	m_pThread->ResumeThread();
}
////////////////////
// preSocketSession

preSocketSession::preSocketSession(int port) 
	: _isAlive(true), _serverFd(-1), _serverPort(port)
{
	TraceLog( ("preSocketSession()"));
}

preSocketSession::~preSocketSession() {
	TraceLog( ("~preSocketSession()"));
	destroy();
}

bool 
preSocketSession::init()
{

#ifdef _WIN32
    WORD VersionRequested = MAKEWORD(VERSION_MAJOR, VERSION_MINOR);
    WSADATA       WsaData;            // receives data from WSAStartup

    // Load WinSock DLL
    if ( WSAStartup(VersionRequested, &WsaData) != 0 ) {
        return false;
    }
#endif
	preSocketHandler* handler = preSocketHandler::getInstance();
	handler->setSession(this);

	struct sockaddr_in server_addr;

	_serverFd = socket( PF_INET, SOCK_STREAM, 0 );
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(_serverPort);

	if(bind(_serverFd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
	{
        TraceLog(("Server: Can't bind local address."));
        return false;
    }
	listen(_serverFd, 5);
	TraceLog(("preSocketServer Socket Listen..."));
	handler->start();

	return true;
}

void
preSocketSession::start()
{
	TraceLog(("start()"));
	m_pThread = ::AfxBeginThread(preSocketSession::run, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pThread->m_bAutoDelete = TRUE;
	m_pThread->ResumeThread();

}

UINT 
preSocketSession::run(LPVOID pParam) {
	TraceLog(("run()"));
	preSocketSession*  _this = (preSocketSession*)pParam;
	_this->run();
	return 0;
}
void
preSocketSession::run() {
	//char buf[BUF_LEN+1];
    int msg_size;

	struct sockaddr_in client_addr;
	int len = sizeof(client_addr);
	
	sockUtil* sUtil = sockUtil::getInstance();

	while(_isAlive) {
		int client_fd = accept(_serverFd, (struct sockaddr *)&client_addr, &len);
        if(client_fd < 0) {
            TraceLog(("Server: accept failed."));
            continue;
        }

		// skpark 2010.09.13
		std::string recvMsg;
		std::string err;
		TraceLog(("hear()"));

		std::string classCode;
		boolean	interactive=false;

		if (!sUtil->hear(client_fd, 10, recvMsg, err, classCode, interactive)) // "reservedProgram" listen
		{
			TraceLog(("message recv failed..%s",err.c_str()));
			sUtil->sock_close(client_fd);
			continue;
		}
		const char* buf  = recvMsg.c_str();

		//if(classCode == CLASS_BYPASS){
		preSocketHandler::getInstance()->push(buf);
			//if(!this->_processCmd(buf)){
			//	sUtil->sock_close(client_fd);
			//	continue;
			//}
		//}
		TraceLog(("received=%s", buf));
		sUtil->talk(client_fd, "ACK", CLASS_ACK, false); // skpark 2010.09.14  ACK 추가
		closesocket(client_fd);
	}
	delete this;
}
void 
preSocketSession::destroy() {
	TraceLog( ("destroy()"));
	closesocket(_serverFd);
	WSACleanup();       // Unload WinSock DLL
	m_pThread = NULL;
	preSocketHandler::clearInstance();
	_isAlive = false;
}

bool 
preSocketSession::ProcessCmd(const char* recvMsg)
{
	TraceLog(("ProcessCmd(%s)", recvMsg));
	//stream.Format("%s/%.2f/%d", data.serialNo, data.temperature, data.level);

	CString recvStr = recvMsg;

	int pos = 0;
	CString serialNo = recvStr.Tokenize("/", pos);
	if (serialNo.IsEmpty())
	{
		TraceLog(("parsing error 1"));
		return false;
	}
	CString temper = recvStr.Tokenize("/", pos);
	if (temper.IsEmpty())
	{
		TraceLog(("parsing error 2"));
		return false;
	}
	CString level = recvStr.Tokenize("/", pos);
	if (level.IsEmpty())
	{
		TraceLog(("parsing error 3"));
		return false;
	}
	CString dType = recvStr.Tokenize("/", pos);
	if (dType.IsEmpty())
	{
		TraceLog(("parsing error 4"));
		return false;
	}


	SetData(serialNo, double(atof(temper)), atoi(dType));
	return true;
}

int preSocketSession::GetNames(CiStringList& nameList)
{
	TEMPER_MAP::iterator itr = _temperMap.begin();
	for (; itr != _temperMap.end(); itr++)
	{
		nameList.push_back(itr->first);
	}
	return nameList.size();
}

void preSocketSession::SetData(LPCTSTR serialNo, double temper, int dType)
{
	// dType == 0 : Teperature
	// dType == 1 : Velocity

	if (IsRealData(serialNo))
	{
		TraceLog(("SetData(%s=%.2f)", serialNo, temper));
		_logger.SaveLog(serialNo, temper);
	}
	ciGuard aGuard(_mapLock);
	if (dType == VELOCITY_OVER)
	{ 
		_velociMap[serialNo] = temper;
	}
	else
	{
		_temperMap[serialNo] = temper;
	}
}
double preSocketSession::GetData(LPCTSTR serialNo, int dType)
{
	ciGuard aGuard(_mapLock);
	if (dType == VELOCITY_OVER)
	{
		TEMPER_MAP::iterator itr = _velociMap.find(serialNo);
		if (itr == _velociMap.end())
		{
			return _NO_VALUE_;
		}
		return itr->second;
	}

	TEMPER_MAP::iterator itr = _temperMap.find(serialNo);
	if (itr == _temperMap.end())
	{
		return _NO_VALUE_;
	}
	return itr->second;

}

