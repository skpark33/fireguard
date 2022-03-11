// ConsoleSample.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "../CSmtp.h"

int _tmain(int argc, _TCHAR* argv[])
{

	bool bError = false;

	try
	{
		char cModule[MAX_PATH];
		::ZeroMemory(cModule, MAX_PATH);
		::GetModuleFileName(NULL, cModule, MAX_PATH);

		char cDrive[MAX_PATH], cPath[MAX_PATH], cFilename[MAX_PATH], cExt[MAX_PATH];
		_splitpath(cModule, cDrive, cPath, cFilename, cExt);

		std::string strConfigPath;
		strConfigPath = cDrive;
		strConfigPath += cPath;
		strConfigPath += "UBCTT.ini";

		CSmtp mail(strConfigPath, "KIA");
		//std::string strText = "This is\n a test\nmassage";
		//mail.Send("jwh184@gmail.com", "test", (char*)strText.c_str());
		//char Text[] = "This is\n a test\nmassage";
		//mail.Send("jwh184@gmail.com", "test", "This is\n a test\nmassage");

		std::list<string> listRecipient;
		listRecipient.push_back("jwh184@nate.com");
		listRecipient.push_back("jwh184@naver.com");
		listRecipient.push_back("jwh184@hanmail.com");
		
		mail.Send(listRecipient, "test", "This is\n a test\nmassage");
	}
	catch(ECSmtp e)
	{
		std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
		bError = true;
	}
	if(!bError)
		std::cout << "Mail was send successfully.\n";

	return 0;
}

