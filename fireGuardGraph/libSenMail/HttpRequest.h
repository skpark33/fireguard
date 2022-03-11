#pragma once

#include <afxmt.h>

// CHttpRequestNo

class CHttpRequestNo
{
public:
	virtual ~CHttpRequestNo();

	static	CHttpRequestNo*	getInstance();
protected:
	static	CHttpRequestNo*	_instance;
	static	CMutex			_mutex;

	CHttpRequestNo();

	CString	m_strServerIP;
	UINT	m_nServerPort;

	BOOL	Request(bool isGet, LPCTSTR lpUrl, LPCTSTR lpszSendMsg, CString& strOutMsg);

public:
	BOOL	Init(LPCTSTR lpszServerIP, UINT nServerPort);

	BOOL	RequestGet(LPCTSTR lpUrl, CString &strOutMsg);
	BOOL	RequestPost(LPCTSTR lpUrl, LPCTSTR lpszSendMsg, CString& strOutMsg);

	static CString	ConvertToURLString(CString str, bool bReturnNullString);
	static CString	ToString(int nValue);
};
