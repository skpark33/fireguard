#pragma once
#include "afxwin.h"


// CThermalCorrectionInfoDlg ��ȭ �����Դϴ�.

class CThermalCorrectionInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CThermalCorrectionInfoDlg)

public:
	CThermalCorrectionInfoDlg(ST_SPIDER_THERMAL_COMPONENTS* pThermalComponents, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CThermalCorrectionInfoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_THERMAL_CORRECTION_INFO };

protected:
	ST_SPIDER_THERMAL_COMPONENTS* m_pThermalComponents;
	ST_SPIDER_THERMAL_CORRECT_INFO m_thermalCorrectInfo;

	CButton m_btnEnable;
	CEdit m_edtEmissivity;
	CEdit m_edtTransmission;
	CEdit m_edtAtmosphere;
	CEdit m_edtZeroOffset;
	CEdit m_edtDistCorr;

	float m_fEmissivity;
	float m_fTransmission;
	float m_fAtmosphere;
	float m_fZeroOffset;
	long m_nlDistCorr;

protected:
	void Initialize();
	void SetCtrlValue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckEnable();
	BOOL m_bEnable;
	afx_msg void OnBnClickedButtonThermalModify();
};
