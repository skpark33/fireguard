#include "stdafx.h"
#include "SockServer.h"

#define WIN32_LEAN_AND_MEAN

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define	SOCK_DATA_TERMINATOR	'\n'

int CSockServer::Open()
{
	WSADATA wsaData;
	int iResult;

	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, (PCSTR)(LPCTSTR)(this->m_port), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return -2;
	}

	// Create a SOCKET for connecting to server
	m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return -3;
	}

	// Setup the TCP listening socket
	iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(m_listenSocket);
		WSACleanup();
		return -4;
	}

	freeaddrinfo(result);

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(m_listenSocket);
		WSACleanup();
		return -5;
	}

	return 1;
}

int CSockServer::Start()
{

	while (1)
	{

		// Accept a client socket
		SOCKET ClientSocket = accept(m_listenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			continue;
		}

		CString output, err;
		if (!Receive(ClientSocket, output, err))
		{
			printf("Receive Error  (%s)", err);
			continue;
		}





	} // accept socket
}

bool CSockServer::Receive(SOCKET socket_fd, CString& output, CString& err)
{
	boolean terminated_normally = false;
	while (1)
	{
		char buf[DEFAULT_BUFLEN + 1];
		memset(buf, 0x00, DEFAULT_BUFLEN + 1);
		int msg_size = recv(socket_fd, buf, DEFAULT_BUFLEN, 0);
		if (msg_size== 0) 
		{
			err = "Nothing to read";
			break;
		}
		if (msg_size < 0)
		{
			err.Format(_T("recv failed with error : %d"), WSAGetLastError());
			break;
		}

		buf[msg_size] = 0;
		if (buf[msg_size - 1] == SOCK_DATA_TERMINATOR)
		{
			//printf("socket terminated with TERMINATOR\n");//
			terminated_normally = true;
			if (msg_size > 1)
			{
				buf[msg_size - 1] = 0;
				output += buf;
			}
			break;
		}
		output += buf;
	}
	if (output.IsEmpty()){
		err = err + _T(", output empty!!!");
		return false;
	}
	// skpark 2010.09.13
	if (!terminated_normally){
		err = err + _T(", terminated abnormally!!!");
		return false;
	}
	return true;
}



int CSockServer::Close()
{
	// shutdown the connection since we're done
	//iResult = shutdown(ClientSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR) {
	//	printf("shutdown failed with error: %d\n", WSAGetLastError());
	//	closesocket(ClientSocket);
	//	WSACleanup();
	//	return 1;
	//}

	//// cleanup
	//closesocket(ClientSocket);
	//WSACleanup();

	closesocket(m_listenSocket);
	WSACleanup();

	return 0;
}

CSockHandler* 	CSockHandler::_instance = 0;
CCriticalSection 	CSockHandler::_instanceLock;

CSockHandler*
CSockHandler::GetInstance() 
{
	if (!_instance) 
	{
		_instanceLock.Lock();
		if (!_instance) 
		{
			_instance = new CSockHandler;
		}
		_instanceLock.Unlock();
	}
	return _instance;
}

void
CSockHandler::ClearInstance() 
{
	if (_instance) 
	{
		_instanceLock.Lock();
		if (_instance) 
		{
			delete _instance;
			_instance = 0;
		}
		_instanceLock.Unlock();
	}
}

void
CSockHandler::Push(LPCTSTR input)
{
	m_qLock.Lock();
	m_q.push_back(input);
	m_qLock.Unlock();
}

void
CSockHandler::Pop(CString& output)
{
	m_qLock.Lock();
	output = m_q.front();
	m_q.pop_front();
	m_qLock.Unlock();
}

