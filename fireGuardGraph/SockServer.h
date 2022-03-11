#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "afxmt.h"
#include "afxwin.h"
#include <stdlib.h>
#include <stdio.h>
#include <list>

#define DEFAULT_PORT "20049"


class CSockServer
{
public:
	CSockServer(const char* port = DEFAULT_PORT) : m_port(port), m_listenSocket(INVALID_SOCKET) {}
	~CSockServer() { Close();  }

	int Open();
	int Start();
	int Close();

protected:
	CString m_port;
	SOCKET m_listenSocket;

	bool Receive(SOCKET socket_fd, int waitTime, CString& output, CString& err);

};

class CSockHandler
{
public:
	static CSockHandler*	GetInstance();
	static void	ClearInstance();

	~CSockHandler() {}

	void Push(LPCTSTR input);
	void Pop(CString& output);

protected:
	CSockHandler() {}

	bool Receive(SOCKET socket_fd, CString& output, CString& err);
	
	static CSockHandler*		_instance;
	static CCriticalSection		_instanceLock;

	CCriticalSection		m_qLock;
	std::list<CString>	m_q;

};