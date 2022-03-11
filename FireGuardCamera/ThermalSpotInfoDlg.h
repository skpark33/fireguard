#pragma once
#include "afxwin.h"


// CThermalSpotInfoDlg ��ȭ �����Դϴ�.

class CThermalSpotInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CThermalSpotInfoDlg)

public:
	CThermalSpotInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CThermalSpotInfoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_THERMAL_SPOT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	ST_SPIDER_THERMAL_SPOT_INFO m_stThermalSpotInfo;

	CString m_strMinCoordinate;
	CString m_strMaxCoordinate;

	CComboBox m_cbEnable[10];
	CButton m_btnModify[10];

protected:
	void CreateEnableComboBox();
	void ModifyThermalSpot(ST_SPIDER_THERMAL_SPOT* pSpot);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonGet2();
	afx_msg void OnBnClickedButtonModify1();
	afx_msg void OnBnClickedButtonModify2();
	afx_msg void OnBnClickedButtonModify3();
	afx_msg void OnBnClickedButtonModify4();
	afx_msg void OnBnClickedButtonModify5();
	afx_msg void OnBnClickedButtonModify6();
	afx_msg void OnBnClickedButtonModify7();
	afx_msg void OnBnClickedButtonModify8();
	afx_msg void OnBnClickedButtonModify9();
	afx_msg void OnBnClickedButtonModify10();
};
