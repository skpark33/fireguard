// fireGuardGraph.h : main header file for the REALTIMETRACK application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CfireGuardGraphApp:
// See fireGuardGraph.cpp for the implementation of this class
//

class CfireGuardGraphApp : public CWinApp
{
public:
	CfireGuardGraphApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CfireGuardGraphApp theApp;
