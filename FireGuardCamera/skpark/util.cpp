#include "stdAfx.h"
#include "util.h"

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

