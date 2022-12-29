// fireGuardGraphDlg.h : header file
//

#pragma once

#include "ChartViewer.h"
#include <afxmt.h>
#include "resource.h"
#include <gdiplus.h>
#include "afxcmn.h"
#include "afxwin.h"
//#include "LogManager.h"
//#include "SMSManagerDlg.h"
//#include "LogManager.h"

#ifndef  MAX_CAMERA
#define MAX_CAMERA 8
#endif



class CMultiThresholdHandler
{
public:
	CMultiThresholdHandler(int type, CChartViewer* viewer, CEdit maxE[], CButton checkBt[],
		CAnimateCtrl* aniCtrl,  CEdit* frequency = NULL /*, CEdit* deviation = NULL */);
	virtual ~CMultiThresholdHandler();

	void ReadConfig(LPCTSTR maxDefault, LPCTSTR minDefault);
	void WriteConfig();
	void OnStop();
	void OnApply();

	bool  RunAlarmSound(int index, double value);
	bool	InitAnimation();

	CEdit* 	m_maxEditor[MAX_CAMERA];
	CButton* m_checkTH[MAX_CAMERA];
protected:
	UINT  _RunAlarmSound();
	//CEdit*	m_minEditor;
	CAnimateCtrl* m_aniCtrl;
	CChartViewer* m_ChartViewer;
	//CEdit*	m_editFrequency;
	//CEdit*	m_editDeviation;
	bool   m_isAniOpen;
	bool   m_isAniPlay;

public:
	double m_thresholdMax[MAX_CAMERA];
	//float m_thresholdMin;
	//int		m_frequency;
	//double m_deviation;
	int		m_type;
	CString	m_maxName;
	CString  m_minName;

	bool		hasAleadyPlayed() { return m_isAniPlay;  }

};

