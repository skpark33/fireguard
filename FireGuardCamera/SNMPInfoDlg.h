#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSNMPInfoDlg ��ȭ �����Դϴ�.

class CSNMPInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CSNMPInfoDlg)

public:
	CSNMPInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSNMPInfoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SNMP_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	CComboBox m_cbSNMPEnable;
	CComboBox m_cbCstEnable;
	CIPAddressCtrl m_ipctrlCst;
	CButton m_btnModify;

protected:
	void InitSNMP();

public:
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnCbnSelchangeComboSnmpEnable();
	afx_msg void OnCbnSelchangeComboCstEnable();
};
