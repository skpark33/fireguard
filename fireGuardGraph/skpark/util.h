#pragma once

#include "afxmt.h"


void OpenIExplorer(CString strParam, int cx = 1024, int cy = 768);
bool deleteOldFile(const char* rootDir, int day, int hour, int min, const char* filter);

class LicenseUtil
{
public:
	static int LICENSE_ERR_CODE;
	static std::string hostId;

	static boolean ReadAuthFile(std::string& host, std::string& mac);
	static bool IsExistmacaddr(const char* szMacaddr);
	static void LicenseCheck();

	
};