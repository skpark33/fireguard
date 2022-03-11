#pragma once
#include "resource.h"


// CSelectCameraDlg ��ȭ �����Դϴ�.

class CSelectCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectCameraDlg)

public:
	CSelectCameraDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSelectCameraDlg();

	static const int MAX_CAMERA = 8;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_SELECT_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	bool  enabled[MAX_CAMERA];
	int checked;

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
};
