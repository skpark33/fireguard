#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#define  MAX_DATA 4

// SMSManagerDlg 대화 상자입니다.

class SendData
{
public:
	SendData() : checkSMS(false) {}
	void Copy(SendData& t) { t.alias = alias; t.checkSMS = checkSMS; t.tel = tel; }
	CString alias;
	bool checkSMS;
	CString tel;
};

class SMSManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SMSManagerDlg)

public:
	SMSManagerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SMSManagerDlg();


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_SMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtDel1();
	afx_msg void OnBnClickedBtDel2();
	afx_msg void OnBnClickedBtDel3();
	afx_msg void OnBnClickedBtDel4();
	virtual BOOL OnInitDialog();
	CEdit m_editTelephone[MAX_DATA];
	CButton m_checkSMS[MAX_DATA];
	CEdit m_editName[MAX_DATA];
	afx_msg void OnBnClickedOk();

	void UnSet(int i);
	void Set(int i);
	void Get(int i, CString& outString);

	SendData	m_sendData[MAX_DATA];

	bool SendNaverSMS(const char* serviceUrl, const char* title, const char* content, const char* imgUrl, const char* phoneNo);
	//bool SendMail(LPCTSTR enterprise, LPCTSTR receiver, LPCTSTR title, LPCTSTR text);

	

};
