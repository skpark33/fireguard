// HttpRequest.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "HttpRequest.h"

//#include <afxinet.h>


#define		DEFAULT_SERVER_IP			_T("10.10.112.39")
#define		DEFAULT_SERVER_WEBPORT		8080

// CHttpRequestNo

CHttpRequestNo* CHttpRequestNo::_instance = NULL;
CMutex	CHttpRequestNo::_mutex;

CHttpRequestNo* CHttpRequestNo::getInstance()
{
	_mutex.Lock();

	if(_instance == NULL)
	{
		_instance = new CHttpRequestNo();
	}

	_mutex.Unlock();
	return _instance;
}

CHttpRequestNo::CHttpRequestNo()
: 	m_strServerIP ( DEFAULT_SERVER_IP )
,	m_nServerPort ( DEFAULT_SERVER_WEBPORT )
{
}

CHttpRequestNo::~CHttpRequestNo()
{
}

BOOL CHttpRequestNo::Init(LPCTSTR lpszServerIP, UINT nServerPort)
{
	//
	if(lpszServerIP != NULL)
	{
		m_strServerIP = lpszServerIP;
	}

	//
	if(nServerPort != 0)
	{
		m_nServerPort = nServerPort;
	}

	return TRUE;
}

BOOL CHttpRequestNo::Request(bool isGet, LPCTSTR lpUrl, LPCTSTR lpszSendMsg, CString& strOutMsg)
{
	//
	CString url;
	if( _tcsnicmp( lpUrl, _T("http://"), 7) )
	{
		url.Format(_T("http://%s:%d%s%s"), 
			m_strServerIP,
			m_nServerPort,
			(lpUrl[0] == _T('/') ? "" : _T("/")),
			lpUrl
		);
	}
	else
		url = lpUrl;

	//
	BOOL bRet = FALSE;

	DWORD dwSearviceType;
	CString strServer, strObject;
	INTERNET_PORT nPort;

	if(!AfxParseURL(url, dwSearviceType, strServer, strObject, nPort))
	{
		ASSERT(0);
		strOutMsg = "Parse URL Fail";	
		return bRet;
	}

	CString strReciveMessage("");
	DWORD dwReadSize;

	CInternetSession Session;
	//Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 10000);	// timeout : 5sec
	CHttpConnection* pServer = NULL;
	CHttpFile *pFile = NULL;

	try
	{
		pServer = Session.GetHttpConnection(strServer, nPort);

		if(isGet)
		{
			CString strHeader = "Accept: */*\r\n";
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
			pFile->SendRequest(strHeader, (LPVOID)(LPCTSTR)strHeader, strHeader.GetLength());
		}
		else
		{
			CString strHeader = "Content-Type: application/x-www-form-urlencoded\r\n";
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
			pFile->SendRequest(strHeader, (LPVOID)lpszSendMsg, _tcslen(lpszSendMsg));
		}
	}
	catch (CInternetException* e)
	{
		TCHAR szError[255];
		e->GetErrorMessage(szError,255);
		strcpy(szError, strOutMsg);
		e->Delete();
		return bRet;
	}

	char szLen[32]="";
	DWORD dwLenSize = sizeof(szLen);
	pFile->QueryInfo( HTTP_QUERY_CONTENT_LENGTH, szLen, &dwLenSize );
	DWORD length = atoi( szLen);

	//int length = pFile->GetLength();
	TCHAR* buf = new TCHAR[length+1];
	::ZeroMemory(buf, sizeof(TCHAR)*(length+1));
	dwReadSize = pFile->Read(buf, length);
	strReciveMessage = buf;
	delete[]buf;

	if(dwReadSize != (DWORD)strReciveMessage.GetLength())
		bRet = FALSE;
	else
		bRet = TRUE;

	strOutMsg = strReciveMessage;

	if(pFile)
	{
		pFile->Close();
		delete pFile;
		pFile = NULL;
	}

	if(pServer)
	{
		pServer->Close();
		delete pServer;
		pServer = NULL;
	}

	Session.Close();

	return bRet;
}

BOOL CHttpRequestNo::RequestGet(LPCTSTR lpUrl, CString &strOutMsg)
{
	return Request(true, lpUrl, NULL, strOutMsg);
}

BOOL CHttpRequestNo::RequestPost(LPCTSTR lpUrl, LPCTSTR lpszSendMsg, CString& strOutMsg)
{
	return Request(false, lpUrl, lpszSendMsg, strOutMsg);
}

CString CHttpRequestNo::ConvertToURLString(CString str, bool bReturnNullString)
{
	if(str.GetLength() == 0) return (bReturnNullString ? _T("(null)") : _T(""));
	str.Replace(_T("%"), _T("%25"));
	str.Replace(_T(" "), _T("%20"));
	str.Replace(_T("&"), _T("%26"));
	str.Replace(_T("="), _T("%3d"));
	str.Replace(_T("+"), _T("%2b"));
	str.Replace(_T("?"), _T("%3f"));
	return str;
}

CString CHttpRequestNo::ToString(int nValue)
{
	TCHAR buf[16] = {0};
	_stprintf(buf, _T("%d"), nValue);

	return buf;
}
