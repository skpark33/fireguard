// SampleSendMail.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSampleSendMailApp:
// �� Ŭ������ ������ ���ؼ��� SampleSendMail.cpp�� �����Ͻʽÿ�.
//

class CSampleSendMailApp : public CWinApp
{
public:
	CSampleSendMailApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSampleSendMailApp theApp;