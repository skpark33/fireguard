#pragma once

#include "afxmt.h"


void OpenIExplorer(CString strParam, int cx = 1024, int cy = 768);

class LicenseUtil
{
public:
	static int LICENSE_ERR_CODE;

	static boolean ReadAuthFile(std::string& host, std::string& mac);
	static bool IsExistmacaddr(const char* szMacaddr);
	static void LicenseCheck();
};