#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include "resource.h"


// CThermalDetailDlg ��ȭ �����Դϴ�.

class CThermalDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CThermalDetailDlg)

public:
	CThermalDetailDlg(const char* id = "", const  char* ip = "", const char* pwd = "", CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CThermalDetailDlg();

	void SetIdMap(std::map<CString, CString>* amap) { m_centerIdMap = amap; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
