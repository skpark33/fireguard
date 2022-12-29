// fireGuardGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fireGuardGraph.h"
#include "fireGuardGraphDlg.h"
#include <math.h>
#include <vector>
#include <sstream>
#include "preSocketSession.h"
#include "skpark/TraceLog.h"
#include "resource.h"
#include "mmsystem.h"
#include "SelectCameraDlg.h"
#include "ci/libAes256/ciElCryptoAes256.h"
#include "skpark/util.h"
#include "ThresholdHandler.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




/////////////////////////////////////////////////////////////////////////////
// CThresholdHandler class

CThresholdHandler::CThresholdHandler(int type, CChartViewer* viewer, CEdit* maxE, 
	CAnimateCtrl* aniCtrl, CEdit*  frequency/*,CEdit* deviation*/)
{
	
	m_type = type;
	m_maxName = "MAX_THRESHOLD";
	m_minName = "MIN_THRESHOLD";
	if (m_type == VELOCITY_OVER)
	{
		m_maxName = "MAX_VELOC_THRESHOLD";
		m_minName = "MIN_VELOC_THRESHOLD";
	}
	m_ChartViewer = viewer;
	m_maxEditor = maxE;
	m_editFrequency = frequency;
	//m_editDeviation = deviation;
	m_aniCtrl = aniCtrl;
	m_isAniOpen = false;
	m_isAniPlay = false;

	m_thresholdMax = -1.0f;
	m_frequency = 0;;
	//m_deviation = 0;
}
CThresholdHandler::~CThresholdHandler()
{
	if (m_aniCtrl && m_isAniOpen)
	{
		m_aniCtrl->Stop();
		m_aniCtrl->Close();
	}
}

void CThresholdHandler::ReadConfig(LPCTSTR maxDefault,  LPCTSTR minDefault)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;
	char buf[512];
	memset(buf, 0x00, 512);
	GetPrivateProfileString("FIRE_WATCH", m_maxName, maxDefault, buf, 512, iniPath);
	m_thresholdMax = atoi(buf);
	if (m_maxEditor)
	{
		m_maxEditor->SetWindowTextA(buf);
	}
	if (m_editFrequency)
	{
		memset(buf, 0x00, 512);
		GetPrivateProfileString("FIRE_WATCH", "FREQUENCY", "100", buf, 512, iniPath);
		m_frequency = atoi(buf);
		m_editFrequency->SetWindowTextA(buf);
	}
	//if (m_editDeviation)
	//{
	//	memset(buf, 0x00, 512);
	//	GetPrivateProfileString("FIRE_WATCH", "DEVIATION", "1", buf, 512, iniPath);
	//	m_deviation = atof(buf);
	//	m_editDeviation->SetWindowTextA(buf);
	//}


	if (m_maxEditor)
	{
		m_ChartViewer->SetThreshold(m_thresholdMax, 0, m_type);
	}
}
void CThresholdHandler::WriteConfig()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString buf;
	buf.Format("%d", int(m_thresholdMax));
	WritePrivateProfileStringA("FIRE_WATCH", m_maxName, buf, iniPath);

	if (m_editFrequency)
	{
		buf.Format("%d", m_frequency);
		WritePrivateProfileStringA("FIRE_WATCH", "FREQUENCY", buf, iniPath);
	}
	//if (m_editDeviation)
	//{
	//	buf.Format("%.0f", m_deviation);
	//	WritePrivateProfileStringA("FIRE_WATCH", "DEVIATION", buf, iniPath);
	//}

}

void CThresholdHandler::OnApply()
{
	CString thresholdStr;
	if (m_maxEditor)
	{
		m_maxEditor->GetWindowTextA(thresholdStr);
		m_thresholdMax = atof(thresholdStr);
	}
	if (m_editFrequency)
	{
		m_editFrequency->GetWindowTextA(thresholdStr);
		m_frequency = atoi(thresholdStr);
	}
	//if (m_editDeviation)
	//{
	//	m_editDeviation->GetWindowTextA(thresholdStr);
	//	m_deviation = atof(thresholdStr);
	//}
	WriteConfig();

	m_ChartViewer->SetThreshold(m_thresholdMax, 0, m_type);
	m_ChartViewer->Invalidate();
}

void CThresholdHandler::OnStop()
{
	if (m_aniCtrl)
	{
		if (m_isAniPlay)
		{
			m_aniCtrl->Stop();

			TraceLog(("Sound Stop..."));
			PlaySound(NULL, 0, 0);
			m_isAniPlay = false;
			m_aniCtrl->ShowWindow(SW_HIDE);
		}
	}
}

bool  CThresholdHandler::InitAnimation()
{
	if (m_aniCtrl)
	{
		CString szSoundPath;
		szSoundPath.Format("%swarning%d.avi", UBC_EXECUTE_PATH, m_type);

		m_isAniOpen = this->m_aniCtrl->Open(szSoundPath);

		m_aniCtrl->ShowWindow(SW_HIDE);

		return m_isAniOpen;
	}
	return false;
}

bool  CThresholdHandler::RunAlarmSound(double value)
{
	//TraceLog(("RunAlarmSound1, m_thresholdMax = %f", m_thresholdMax));
	if ( m_aniCtrl && value > m_thresholdMax && value != _NO_VALUE_)
	{
		TraceLog(("RunAlarmSound2, m_thresholdMax = %f ,%f", value, m_thresholdMax));
		if (!m_isAniPlay)
		{
			TraceLog(("RunAlarmSound3, m_thresholdMax = %f ,%f", value, m_thresholdMax));
			this->_RunAlarmSound();
		}
		return true;
	}
	return false;
}


UINT  CThresholdHandler::_RunAlarmSound()
{
	CString szSoundPath;

	szSoundPath.Format("%salarmSound%d.wav", UBC_EXECUTE_PATH, m_type);

	if (IsLocalExist(szSoundPath))
	{
		//m_isAniOpen = m_aniCtrl.Open(szSoundPath);
		TraceLog(("RunAlarmSound(%s)", szSoundPath));
		//PlaySound(szSoundPath, AfxGetInstanceHandle(),  SND_ASYNC | SND_LOOP); // 무한
		//if(PlaySound(szSoundPath, AfxGetInstanceHandle(),  SND_ASYNC| 
		if (PlaySound(szSoundPath, AfxGetInstanceHandle(), SND_FILENAME | SND_ASYNC | SND_LOOP))
		{// 1회 재생
			TraceLog(("PlaySound(%s)", szSoundPath));
			//PlaySound(NULL, AfxGetInstanceHandle(),  NULL); // 정지
			m_isAniPlay = true;
		}
	}
	else
	{
		TraceLog(("RunAlarmSound(%s) file not exist", szSoundPath));
	}

	if (m_aniCtrl)
	{
		m_aniCtrl->ShowWindow(SW_SHOW);

		if (m_isAniOpen)
		{
			m_isAniPlay = m_aniCtrl->Play(0, -1, -1);
		}
	}
	return 0;
}

