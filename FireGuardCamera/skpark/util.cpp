#include "stdAfx.h"
#include "util.h"
#include <fstream> 
#include <tlhelp32.h>
#include <shlwapi.h>
#include <winternl.h>
#include <afxinet.h>
#include "skpark/TraceLog.h"
#include "NetworkAdapter.h"


class authInfo {
public:
	unsigned char  host[20];
	unsigned char  mac[20];
	authInfo(){
	}
};

void OpenIExplorer(CString strParam, int cx/*=1024*/, int cy/*=768*/)
{
	HRESULT hr;
	IWebBrowser2* pWebBrowser = NULL;

	if (NULL != pWebBrowser)
	{
		pWebBrowser->Quit();
		pWebBrowser->Release();
		pWebBrowser = NULL;
	}

	if (FAILED(hr = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWebBrowser)))
	{
		//		CoUninitialize();
	}

	CString strHeader, strTarget, strUrl;
	strHeader = "Content-Type: application/x-www-form-urlencoded\r\n";
	strTarget = "_top"; // 새로운 창으로 띄움
	strUrl = strParam;

	VARIANT vtHeader = { 0 }, vtTarget = { 0 }, vtEmpty = { 0 }, vtPostData = { 0 };

	vtHeader.vt = VT_BSTR;
	vtHeader.bstrVal = strHeader.AllocSysString();

	vtTarget.vt = VT_BSTR;
	vtTarget.bstrVal = strTarget.AllocSysString();

	::VariantInit(&vtEmpty);

	pWebBrowser->put_ToolBar(VARIANT_FALSE);		// 익스플로어 툴바 없앰
	pWebBrowser->put_MenuBar(VARIANT_FALSE);		// 메뉴바 없앰
	pWebBrowser->put_AddressBar(VARIANT_FALSE);		// 주소창 없앰
	pWebBrowser->put_StatusBar(VARIANT_FALSE);		// 상태바 없앰
	pWebBrowser->put_Visible(VARIANT_TRUE);
	pWebBrowser->put_Top(0);
	pWebBrowser->put_Left(0);
	pWebBrowser->put_Width(cx);
	pWebBrowser->put_Height(cy);

	pWebBrowser->put_Visible(VARIANT_TRUE);
	hr = pWebBrowser->Navigate(strUrl.AllocSysString(), &vtEmpty, &vtTarget, &vtPostData, &vtHeader);

	SHANDLE_PTR IEHwnd;
	pWebBrowser->get_HWND(&IEHwnd);

	SetForegroundWindow((HWND)IEHwnd);

	if (!SUCCEEDED(hr))
	{
		CString msg = "HyperLink Error";
		if (E_INVALIDARG == hr)
			msg += ": Invalid Parameters.";
		else if (E_OUTOFMEMORY == hr)
			msg += ": Out of memory.";
	}

	SysFreeString(vtHeader.bstrVal);
	SysFreeString(vtTarget.bstrVal);
	pWebBrowser->Release();
}


boolean
ReadAuthFile(std::string& host, std::string& mac)
{
	ifstream inCredit("project.dat", ios::in);
	
	if (inCredit.fail()) {
		TraceLog(("project.dat open failed"));
		return false;
	}
	authInfo* aInfo = new authInfo();

	inCredit.read(reinterpret_cast<char*>(aInfo), sizeof(authInfo));
	if (inCredit.fail()) {
		TraceLog(("project.dat read failed"));
		inCredit.close();
		delete aInfo;
		return false;
	}


	char hval[20];
	memset(hval, 0, 20);
	for (int i = 0; i<20; i++){
		hval[i] = (char)(aInfo->host[i] / 2);
	}

	char mval[20];
	memset(mval, 0, 20);
	for (int i = 0; i<20; i++){
		mval[i] = (char)(aInfo->mac[i] / 2);
	}

	host = hval;
	mac = mval;

	delete aInfo;
	inCredit.close();
	return true;
}

bool IsExistmacaddr(const char* szMacaddr)
{
	//int nAdtCount = m_clsAdapterList.GetCount();
	//if(nAdtCount <= 0)
	//{
	//	nAdtCount = loadAdapter(E_ADT_TYPE_LOCAL);
	//	if(nAdtCount <= 0)
	//	{
	//		return false;
	//	}//if
	//}//if
	CNetworkAdapterList m_clsAdapterList;
	m_clsAdapterList.ClearAdtInfolist();

	int nAdtCount = m_clsAdapterList.LoadAdapter(E_ADT_TYPE_ALL);
	if (nAdtCount <= 0)
	{
		return false;
	}//if

	string strAddr;
	for (int i = 0; i<nAdtCount; i++)
	{
		strAddr = m_clsAdapterList.GetAdtMacAddr(i);
		if (strcmp(szMacaddr, strAddr.c_str()) == 0)
		{
			return true;
		}//if
	}//for

	return false;
}

unsigned long
getPid(const char* exename, bool likeCond/*=false*/)
{
	TraceLog(("getPid(%s)", exename));

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		TraceLog(("HANDLE을 생성할 수 없습니다"));
		return 0;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	char strProcessName[512];

	if (!Process32First(hSnapshot, &pe32))	{
		TraceLog(("Process32First failed."));
		::CloseHandle(hSnapshot);
		return 0;
	}


	do 	{
		//memset(strProcessName, 0, sizeof(strProcessName));
		//size_t stringLength = strlen(pe32.szExeFile);
		//for(int i=0; i<stringLength; i++) // 대문자로 전환
		//	strProcessName[i] = toupper( pe32.szExeFile[i] );

		if (likeCond)
		{
			string exename1 = pe32.szExeFile;
			string exename2 = exename;
			::strlwr((char*)exename1.c_str());
			::strlwr((char*)exename2.c_str());

			if (strstr(exename1.c_str(), exename2.c_str()) != NULL) {
				TraceLog(("process founded(%s)", exename));
				::CloseHandle(hSnapshot);
				return pe32.th32ProcessID;
			}
		}
		else if (stricmp(pe32.szExeFile, exename) == 0) {
			TraceLog(("process founded(%s)", exename));
			::CloseHandle(hSnapshot);
			return pe32.th32ProcessID;
		}

	} while (Process32Next(hSnapshot, &pe32));
	TraceLog(("process not founded(%s)", exename));
	::CloseHandle(hSnapshot);

	return 0;
}

static BOOL CALLBACK find_hwnd_from_pid_proc(HWND hwnd, LPARAM lParam)
{
	if (!IsWindowVisible(hwnd)) return TRUE;
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	find_hwnd_from_pid_t *pe = (find_hwnd_from_pid_t *)lParam;
	if (pe->pid != pid) return TRUE;
	pe->hwnd = hwnd;
	return FALSE;
}


HWND
getWHandle(unsigned long pid)
{
	TraceLog(("getWHandle(%ld)", pid));
	if (pid>0){
		find_hwnd_from_pid_t e;
		e.pid = pid;
		e.hwnd = NULL;
		EnumWindows(find_hwnd_from_pid_proc, (LPARAM)&e);

		HWND child = e.hwnd;
		HWND parent = NULL;
		while (1){
			parent = GetParent(child);
			if (!parent){
				return child;
			}
			child = parent;
		}
		return e.hwnd;
	}
	return (HWND)0;
}
HWND
getWHandle(const char* exename, bool likeCond/*=false*/)
{
	TraceLog(("getWHandle(%s)", exename));
	return getWHandle(getPid(exename, likeCond));
}




