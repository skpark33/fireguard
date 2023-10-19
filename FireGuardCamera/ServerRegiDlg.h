#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "map"
#include "list"

// CServerRegiDlg 대화 상자입니다.

class RECEIVER_INFO {
public:
	CString name;
	CString ip;
	CString port;
};

class CServerRegiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerRegiDlg)

public:
	CServerRegiDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CServerRegiDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SERVER_REGI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()



	int m_selected;
	int m_foundedCount;
	static std::map<CString, RECEIVER_INFO> receiverMap;
	void SaveServerInfo(const char* id, const char* name, const char* ip, const char* port);
	void GetInfoFromIni();
	void insertItemAll();
	void insertItem(const char* id, const char* name, const char* ip, const char* port);
	void DelServer(const char* szId);
	void SaveServerInfoListEntry();
	

public:
	CListCtrl m_list_Server;
	CEdit m_editServerName;
	CIPAddressCtrl m_editServerIP;
	CEdit m_editServerPort;

	CButton m_btDeletre;
	CButton m_btAdd;
	afx_msg void OnBnClickedBtDelete();
	afx_msg void OnBnClickedBtAdd();
	afx_msg void OnNMClickServerList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
