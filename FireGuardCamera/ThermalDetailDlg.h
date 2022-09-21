#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include "resource.h"


// CThermalDetailDlg 대화 상자입니다.

class CThermalDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CThermalDetailDlg)

public:
	CThermalDetailDlg(const char* id = "", const  char* ip = "", const char* pwd = "", CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CThermalDetailDlg();

	void SetIdMap(std::map<CString, CString>* amap) { m_centerIdMap = amap; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editCameraId;
	CIPAddressCtrl m_editIp;
	CButton m_btDupCheck;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtDupcheck();
	afx_msg void OnBnClickedOk();

	bool m_hasDupCheck;
	std::map<CString, CString>* m_centerIdMap;
	CString m_ipAddress;
	CString m_cameraId;
	CString m_pwd;
	CEdit m_editPWD;
};
