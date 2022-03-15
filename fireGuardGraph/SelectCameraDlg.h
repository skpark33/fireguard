#pragma once
#include "resource.h"
#include "afxdtctl.h"


// CSelectCameraDlg 대화 상자입니다.

class CSelectCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectCameraDlg)

public:
	CSelectCameraDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelectCameraDlg();

	static const int MAX_CAMERA = 8;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_SELECT_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	bool  enabled[MAX_CAMERA];
	int checked;
	CString prefix;
	CString dateTime;

	CButton m_radioAll;
	CButton m_checkDate;

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();

	afx_msg void  OnBnClickedDate();
	afx_msg void OnBnClickedOk();
	CDateTimeCtrl m_dateTimePicker;
};
