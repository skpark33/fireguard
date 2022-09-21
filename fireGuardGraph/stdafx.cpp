// stdafx.cpp : source file that includes just the standard includes
// [project_name].pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

bool IsLocalExist(LPCTSTR fullpath)
{
	//TraceLog(("IsLocalExist(%s)", fullpath));
	CFileFind ff;
	BOOL bFind = ff.FindFile(fullpath);
	ff.Close();
	if (bFind)
	{
		return true;
	}
	return false;
}
void ProcessWindowMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
}
CString RunCLI(LPCTSTR path, LPCTSTR command, LPCTSTR parameter)
{
	CString strPath;
	if (path == NULL)
	{
		TCHAR szWin32Path[MAX_PATH] = { 0, };
		::SHGetSpecialFolderPath(NULL, szWin32Path, CSIDL_SYSTEM, FALSE);
		strPath = szWin32Path;
	}
	else
	{
		strPath = path;
	}
	strPath += "\\";
	strPath += command;

	TCHAR szParam[MAX_PATH * 4];
	sprintf(szParam, _T("%s %s"), strPath, parameter);

	//TraceLog(("skpark face %s", szParam));

	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	TCHAR szOutputFilePath[MAX_PATH];
	lstrcpy(szOutputFilePath, TEXT("stdout.txt"));

	::DeleteFileA(szOutputFilePath);
	HANDLE hFile = CreateFile(szOutputFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = stdin;
	si.hStdOutput = hFile;
	//si.hStdError = hFile;

	/*
	STARTUPINFO si = {0};
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;	// 콘솔창이 보이지 않도록 한다
	si.hStdInput = stdin;
	si.hStdOutput = hFile;
	si.hStdError = hFile;
	*/

	PROCESS_INFORMATION pi;

	if (!CreateProcess((LPSTR)(LPCTSTR)strPath
		, (LPSTR)(LPCTSTR)szParam
		, NULL
		, NULL
		, TRUE
		, CREATE_NO_WINDOW
		, NULL
		, NULL
		, &si
		, &pi
		))
	{
		DWORD dwErrNo = GetLastError();

		LPVOID lpMsgBuf = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_IGNORE_INSERTS
			| FORMAT_MESSAGE_FROM_SYSTEM
			, NULL
			, dwErrNo
			, 0
			, (LPTSTR)&lpMsgBuf
			, 0
			, NULL);

		CString strErrMsg;
		if (!lpMsgBuf)
		{
			strErrMsg.Format(_T("Unknow Error - CreateProcess [%d]"), dwErrNo);
		}
		else
		{
			strErrMsg.Format(_T("%s [%d]"), (LPCTSTR)lpMsgBuf, dwErrNo);
		}

		LocalFree(lpMsgBuf);
		//TraceLog(("skpark face error : %s", strErrMsg));
		return "0 ";
	}

	int counter = 0;
	while (::WaitForSingleObject(pi.hProcess, 0))
	{
		//if (!m_bRunning)
		if (counter > 5)
		{
			//TraceLog(("Createprocess break!!!"));
			TerminateProcess(pi.hProcess, 0);
			::WaitForSingleObject(pi.hProcess, 2000);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return "0 ";
		}
		counter++;
		ProcessWindowMessage();
		Sleep(500);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(hFile);

	CString msg = "";
	FILE* fp = fopen(szOutputFilePath, "r");
	if (fp != NULL)
	{
		char buf[1024 + 1];
		memset(buf, 0x00, sizeof(buf));
		while (fgets(buf, 1024, fp) != 0)
		{
			if (strlen(buf) > 0)
			{
				msg += buf;
			}
			memset(buf, 0x00, sizeof(buf));
		}
		fclose(fp);
	}
	else
	{
		//TraceLog(("skpark face error : %s file read error", szOutputFilePath));
	}
	return msg;
}

char* UTF8ToAnsi(const char *szUTF8String)
{
#if defined(_WIN32) || defined(_WIN64)
	WCHAR* strSrc = NULL;
	char* szRes = NULL;

	int i = MultiByteToWideChar(CP_UTF8, 0, szUTF8String, -1, NULL, 0);

	strSrc = new WCHAR[(size_t)i + 1];
	MultiByteToWideChar(CP_UTF8, 0, szUTF8String, -1, strSrc, i);

	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);

	szRes = new char[(size_t)i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	delete[] strSrc;
	strSrc = NULL;

	return szRes;

#else
	char *outbuf = 0;
	if (szUTF8String)
	{
		iconv_t cd = iconv_open("EUC-CN", "UTF-8");
		if (cd)
		{
			size_t contlen = strlen(szUTF8String);
			size_t outbuflen = contlen * 3 + 1;
			outbuf = new char[outbuflen];
			memset(outbuf, 0, outbuflen);

			char *inptr = const_cast<char*>(szUTF8String);
			char *outptr = outbuf;

			size_t inlen = contlen;
			size_t outlen = outbuflen;
			if (iconv(cd, &inptr, &inlen, &outptr, &outlen) == (size_t)(-1))
			{
				outbuf = 0;
			}

			iconv_close(cd);
		}
	}
	return outbuf;
#endif
}

std::string UTF8ToANSIString(const char *pUtf8String)
{
	std::string strANSI;
	if (pUtf8String != NULL)
	{
		char *pAnsi = UTF8ToAnsi(pUtf8String);
		if (pAnsi != NULL)
		{
			strANSI = pAnsi;
			delete[]pAnsi;
		}
	}

	return strANSI;
}

std::string UTF8ToANSIString(std::string strRetData){
#if defined(_WIN32) || defined(_WIN64)
	strRetData = UTF8ToANSIString(strRetData.c_str());
#endif
	return strRetData;
}

char* AnsiToUTF8(const char *szAnsiString)
{
#if defined(_WIN32) || defined(_WIN64)
	if (szAnsiString == NULL || strlen(szAnsiString) == 0)
	{
		OutputDebugString("AnsiToUTF8 >>> input param is NULL!");
		return NULL;
	}
	// AnsiToUnicode
	// 1. ANSI(multibyte) Length
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsiString, (int)strlen(szAnsiString), NULL, 0);
	wchar_t* wszUnicodeString = new wchar_t[(size_t)wcsLen + 1];

	// 2. ANSI(multibyte) ---> unicode
	::MultiByteToWideChar(CP_ACP, NULL, szAnsiString, (int)strlen(szAnsiString), wszUnicodeString, wcsLen);
	wszUnicodeString[wcsLen] = '\0';

	// unicode to UTF8
	// 3. utf8 Length
	int UTF8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszUnicodeString, (int)wcslen(wszUnicodeString), NULL, 0, NULL, NULL);
	char* szUTF8 = new char[(size_t)UTF8Len + 1];

	//4. unicode ---> utf8
	::WideCharToMultiByte(CP_UTF8, NULL, wszUnicodeString, (int)wcslen(wszUnicodeString), szUTF8, UTF8Len, NULL, NULL);
	szUTF8[UTF8Len] = '\0';

	delete[] wszUnicodeString;
	wszUnicodeString = NULL;

	return szUTF8;

#else
	if (NULL == szAnsiString)
	{
		return NULL;
	}
	char *outbuf = 0;
	if (szAnsiString)
	{
		iconv_t cd = iconv_open("UTF-8", "EUC-CN");
		if (cd)
		{
			size_t contlen = strlen(szAnsiString);
			size_t outbuflen = contlen * 3 + 1;
			outbuf = new char[outbuflen];
			memset(outbuf, 0, outbuflen);

			char *inptr = const_cast<char*>(szAnsiString);
			char *outptr = outbuf;

			size_t inlen = contlen;
			size_t outlen = outbuflen;
			if (iconv(cd, &inptr, &inlen, &outptr, &outlen) == (size_t)(-1))
			{
				outbuf = 0;
			}

			iconv_close(cd);
		}
	}
	return outbuf;
#endif
}

std::string ANSIToUTF8String(const char *pAnsiString)
{
	std::string strUTF8;
	if (pAnsiString != NULL)
	{
		char *pBuf = AnsiToUTF8(pAnsiString);
		if (pBuf != NULL)
		{
			strUTF8 = pBuf;
			delete[] pBuf;
		}
	}

	return strUTF8;
}

std::string ANSIToUTF8String(std::string strRetData){
#if defined(_WIN32) || defined(_WIN64)
	strRetData = ANSIToUTF8String(strRetData.c_str());
#endif
	return strRetData;
}