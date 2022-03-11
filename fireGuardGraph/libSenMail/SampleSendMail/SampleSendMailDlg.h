// SampleSendMailDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CSampleSendMailDlg 대화 상자
class CSampleSendMailDlg : public CDialog
{
// 생성입니다.
public:
	CSampleSendMailDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAMPLESENDMAIL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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

	void	ReadConfigFile(void);		///<SMTP관련 설정이 되어있는 ini 파일을 읽는다.
};
