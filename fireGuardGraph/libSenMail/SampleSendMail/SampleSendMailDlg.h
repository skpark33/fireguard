// SampleSendMailDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CSampleSendMailDlg ��ȭ ����
class CSampleSendMailDlg : public CDialog
{
// �����Դϴ�.
public:
	CSampleSendMailDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SAMPLESENDMAIL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strSMTPServer;
	CString m_strSMTPPort;
	CString m_strSMTPID;
	CString m_strSMTPPWD;
	CString m_strSender;
	CString m_strReceiver;
	CString m_strTitle;
	CString m_strBody;
	CString m_strConfigPath;
	CString m_strSSL;
	CString m_strSenderName;
	CString m_strReply;

	void	ReadConfigFile(void);		///<SMTP���� ������ �Ǿ��ִ� ini ������ �д´�.
};
