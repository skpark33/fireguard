#include "stdAfx.h"
#include "util.h"
#include <fstream> 
#include "skpark/TraceLog.h"
#include "NetworkAdapter.h"


int LicenseUtil::LICENSE_ERR_CODE = 0;
std::string LicenseUtil::hostId = "";

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


bool deleteOldFile(const char* rootDir, int day, int hour, int min, const char* filter)
{
	TraceLog(("deleteOldFileByMin(%s,%d,%d,%d%s)", rootDir, day, hour, min, filter));

	std::string dirPath = rootDir;
	dirPath += filter;

	TraceLog(("target files=%s", dirPath.c_str()));

	HANDLE hFile = NULL;
	WIN32_FIND_DATA FileData;
	hFile = FindFirstFile(dirPath.c_str(), &FileData);
	if (hFile == INVALID_HANDLE_VALUE) {
		TraceLog(("screenshot file not found"));
		return false;
	}

	int deleted_counter = 0;

	CTime referTime = CTime::GetCurrentTime();
	CTimeSpan spanTime(day, hour, min, 0);
	referTime = referTime - spanTime;

	do {
		if (strncmp(FileData.cFileName, "..", 2) == 0 || strncmp(FileData.cFileName, ".", 1) == 0) {
			continue;
		}
		if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			std::string subDir = rootDir;
			subDir += FileData.cFileName;
			subDir += "//";
			deleteOldFile(subDir.c_str(), day, hour, min, filter);
			continue;
		}

		std::string filename = FileData.cFileName;

		CTime fileTime(FileData.ftLastWriteTime);
		if (fileTime < referTime) {
			std::string delFilePath = rootDir;
			delFilePath += filename;
			::remove(delFilePath.c_str());
			deleted_counter++;
			TraceLog(("%s file deleted", delFilePath.c_str()));
		}

	} while (FindNextFile(hFile, &FileData));

	FindClose(hFile);
	return true;
}

boolean
LicenseUtil::ReadAuthFile(std::string& host, std::string& mac)
{
	std::ifstream inCredit("..\\execute\\project.dat", std::ios::in);
	
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
	hostId = host;
	mac = mval;

	delete aInfo;
	inCredit.close();
	return true;
}

bool LicenseUtil::IsExistmacaddr(const char* szMacaddr)
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

	std::string strAddr;
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


void LicenseUtil::LicenseCheck()
{
	std::string host, mac;
	if (ReadAuthFile(host, mac)) {
		if (IsExistmacaddr(mac.c_str())) {
			LICENSE_ERR_CODE = 0;
			return;
		}
		TraceLog(("MacAddress is not matched"));
		LICENSE_ERR_CODE = 1;
	}
	else{
		TraceLog(("No License file founded"));
		LICENSE_ERR_CODE = 2;
	}
}




