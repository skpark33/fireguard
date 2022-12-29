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
#include "MultiThresholdHandler.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




/////////////////////////////////////////////////////////////////////////////
// CMultiThresholdHandler class

CMultiThresholdHandler::CMultiThresholdHandler(int type, CChartViewer* viewer, CEdit maxE[], CButton checkBt[],
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
	//m_editFrequency = frequency;
	//m_editDeviation = deviation;
	m_aniCtrl = aniCtrl;
	m_isAniOpen = false;
	m_isAniPlay = false;

	for (int i = 0; i < MAX_CAMERA; i++) {
		m_maxEditor[i] = &maxE[i];
		m_checkTH[i] = &checkBt[i];
		m_thresholdMax[i] = -1.0f;
	}
	//m_frequency = 0;;
	//m_deviation = 0;
}
CMultiThresholdHandler::~CMultiThresholdHandler()
{
	if (m_aniCtrl && m_isAniOpen)
	{
		m_aniCtrl->Stop();
		m_aniCtrl->Close();
	}
}

void CMultiThresholdHandler::ReadConfig(LPCTSTR maxDefault,  LPCTSTR minDefault)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;
	char buf[512];

	for (int i = 0; i < MAX_CAMERA; i++) {
		CString name; 
		name.Format("%s%d", m_maxName, i);
		memset(buf, 0x00, 512);
		GetPrivateProfileString("FIRE_WATCH",(i > 0 ? name : m_maxName), "-1", buf, 512, iniPath);
		m_thresholdMax[i] = atoi(buf);
		if (m_thresholdMax[i] >= 0) {
			if (m_maxEditor[i])
			{
				m_maxEditor[i]->SetWindowTextA(buf);
				m_ChartViewer->SetThreshold(m_thresholdMax[i], i, m_type);
			} 
		}
		else {
			m_maxEditor[i]->SetWindowTextA("-1");
		}
	}
	//if (m_editFrequency)
	//{
	//	memset(buf, 0x00, 512);
	//	GetPrivateProfileString("FIRE_WATCH", "FREQUENCY", "100", buf, 512, iniPath);
	//	m_frequency = atoi(buf);
	//	m_editFrequency->SetWindowTextA(buf);
	//}
	//if (m_editDeviation)
	//{
	//	memset(buf, 0x00, 512);
	//	GetPrivateProfileString("FIRE_WATCH", "DEVIATION", "1", buf, 512, iniPath);
	//	m_deviation = atof(buf);
	//	m_editDeviation->SetWindowTextA(buf);
	//}



}
void CMultiThresholdHandler::WriteConfig()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;


	CString buf;
	
	for (int i = 0; i < MAX_CAMERA; i++) {
		CString name;
		name.Format("%s%d", m_maxName, i);
		buf.Format("%d", int(m_thresholdMax[i]));
		WritePrivateProfileStringA("FIRE_WATCH",(i> 0 ? name: m_maxName), buf, iniPath);
	}
	//if (m_editFrequency)
	//{
	//	buf.Format("%d", m_frequency);
	//	WritePrivateProfileStringA("FIRE_WATCH", "FREQUENCY", buf, iniPath);
	//}
	//if (m_editDeviation)
	//{
	//	buf.Format("%.0f", m_deviation);
	//	WritePrivateProfileStringA("FIRE_WATCH", "DEVIATION", buf, iniPath);
	//}

}

void CMultiThresholdHandler::OnApply()
{
	for (int i = 0; i < MAX_CAMERA; i++) {
		if (m_maxEditor[i] && m_checkTH[i] && m_checkTH[i]->GetCheck() == TRUE)
		{
			CString thresholdStr;
			m_maxEditor[i]->GetWindowTextA(thresholdStr);
			m_thresholdMax[i] = atof(thresholdStr);
			if (m_thresholdMax[i] >= 0) {
				m_ChartViewer->SetThreshold(m_thresholdMax[i], i, m_type);
			}
		}
	}
	//if (m_editFrequency)
	//{
	//	m_editFrequency->GetWindowTextA(thresholdStr);
	//	m_frequency = atoi(thresholdStr);
	//}
	//if (m_editDeviation)
	//{
	//	m_editDeviation->GetWindowTextA(thresholdStr);
	//	m_deviation = atof(thresholdStr);
	//}
	WriteConfig();
	m_ChartViewer->Invalidate();
}

void CMultiThresholdHandler::OnStop()
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

bool  CMultiThresholdHandler::InitAnimation()
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

bool  CMultiThresholdHandler::RunAlarmSound(int index, double value)
{
	//TraceLog(("RunAlarmSound1, m_thresholdMax = %f", m_thresholdMax));
	if (m_aniCtrl && value > m_thresholdMax[index] && value != _NO_VALUE_ && m_thresholdMax[index] >= 0)
		{
			TraceLog(("RunAlarmSound2, m_thresholdMax[%d] = %f ,%f", index, value, m_thresholdMax[index]));
			if (!m_isAniPlay)
			{
				TraceLog(("RunAlarmSound3, m_thresholdMax[%d] = %f ,%f", index, value, m_thresholdMax[index]));
				this->_RunAlarmSound();
			}
			return true;
		}
	return false;
}


UINT  CMultiThresholdHandler::_RunAlarmSound()
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

