// LogViewerChart.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LogViewerChart.h"
#include "LogViewerChartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogViewerChartApp

BEGIN_MESSAGE_MAP(CLogViewerChartApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLogViewerChartApp construction

CLogViewerChartApp::CLogViewerChartApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLogViewerChartApp object

CLogViewerChartApp theApp;


// CLogViewerChartApp initialization

BOOL CLogViewerChartApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored

	CString filename = "";
	int cameraIdx = 0;
	if (__argc > 2)
	{
		filename = __argv[1];
		cameraIdx = atoi(__argv[2]);
	}
	
	CLogViewerChartDlg dlg;
	dlg.SetLogFile(filename, cameraIdx);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
