#pragma once
#include "afxwin.h"


// CPPPoEInfoDlg ��ȭ �����Դϴ�.

class CPPPoEInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CPPPoEInfoDlg)

public:
	CPPPoEInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPPPoEInfoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PPPOE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

protected:
	CEdit m_edtID;
	CEdit m_edtPwd;
	CString m_strID;
	CString m_strPwd;
	CButton m_btnModify;

protected:
	void InitPPPoE();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonModify();
};
