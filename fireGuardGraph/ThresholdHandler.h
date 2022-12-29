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


class CThresholdHandler
{
public:
	CThresholdHandler(int type, CChartViewer* viewer, CEdit* maxE, 
		CAnimateCtrl* aniCtrl,  CEdit* frequency = NULL /*, CEdit* deviation = NULL */);
	virtual ~CThresholdHandler();

	void ReadConfig(LPCTSTR maxDefault, LPCTSTR minDefault);
	void WriteConfig();
	void OnStop();
	void OnApply();

	bool  RunAlarmSound(double value);
	bool	InitAnimation();

protected:
	UINT  _RunAlarmSound();

	CEdit*	m_maxEditor;
	//CEdit*	m_minEditor;
	CAnimateCtrl* m_aniCtrl;
	CChartViewer* m_ChartViewer;
	CEdit*	m_editFrequency;
	//CEdit*	m_editDeviation;
	bool   m_isAniOpen;
	bool   m_isAniPlay;

public:
	double m_thresholdMax;
	//float m_thresholdMin;
	int		m_frequency;
	//double m_deviation;
	int		m_type;
	CString	m_maxName;
	CString  m_minName;

	bool		hasAleadyPlayed() { return m_isAniPlay;  }

};

