// ServerRegiDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FireGuardCamera.h"
#include "ServerRegiDlg.h"
#include "afxdialogex.h"
#include "skpark/TraceLog.h"
#include "resource.h"

std::map<CString, RECEIVER_INFO> CServerRegiDlg::receiverMap;
// CServerRegiDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CServerRegiDlg, CDialogEx)

CServerRegiDlg::CServerRegiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerRegiDlg::IDD, pParent)
{

}

CServerRegiDlg::~CServerRegiDlg()
{
}

void CServerRegiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT__SERVER_NAME, m_editServerName);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_editServerIP);
	DDX_Control(pDX, IDC_EDIT_PORT_NO, m_editServerPort);
	DDX_Control(pDX, ID_BT_DELETE, m_btDeletre);
	DDX_Control(pDX, ID_BT_ADD, m_btAdd);
	DDX_Control(pDX, IDC_SERVER_LIST, m_list_Server);
}


BEGIN_MESSAGE_MAP(CServerRegiDlg, CDialogEx)
	ON_BN_CLICKED(ID_BT_DELETE, &CServerRegiDlg::OnBnClickedBtDelete)
	ON_BN_CLICKED(ID_BT_ADD, &CServerRegiDlg::OnBnClickedBtAdd)
	ON_NOTIFY(NM_CLICK, IDC_SERVER_LIST, &CServerRegiDlg::OnNMClickServerList)
END_MESSAGE_MAP()


// CServerRegiDlg 메시지 처리기입니다.


void CServerRegiDlg::OnBnClickedBtDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_selected == -1)
	{
		MessageBox(("선택된 카메라가 없습니다. 위 목록에서 카메라를 선택해주세요"));
		return;
	}

	int nSel = m_list_Server.GetSelectionMark();
	if (nSel == -1) return;

	char szId[100] = "";
	m_list_Server.GetItemText(nSel, 0, szId, 50);
	DelServer(szId);
}

void CServerRegiDlg::DelServer(const char* szId)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString newList;
	CString deleteId;
	
	std::map<CString, RECEIVER_INFO>::iterator itr = receiverMap.find(szId);
	if (itr == receiverMap.end()) {
		return;
	}
	receiverMap.erase(szId);
	std::map<CString, RECEIVER_INFO>::iterator jtr;
	for (jtr = receiverMap.begin(); jtr != receiverMap.end(); jtr++){
		if (!newList.IsEmpty()){
			newList += ",";
		}
		newList += jtr->first;
	}
	WritePrivateProfileString("SOCK_WATCH_SERVER", "IdList", newList, iniPath);

	insertItemAll();

}



void CServerRegiDlg::OnBnClickedBtAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString id;
	int len = receiverMap.size();
	id.Format("%d", len + 1);
	RECEIVER_INFO info;

	m_editServerName.GetWindowText(info.name);
	m_editServerIP.GetWindowText(info.ip);
	m_editServerPort.GetWindowText(info.port);

	if (info.name.IsEmpty()) {
		AfxMessageBox("수신할 서버의 이름을 입력하세요");
		return;
	}
	if (info.ip.IsEmpty()) {
		AfxMessageBox("수신할 서버의 ip를 입력하세요");
		return;
	}
	if (info.port.IsEmpty()) {
		AfxMessageBox("수신할 서버의 port를 입력하세요");
		return;
	}
	receiverMap[id] = info;
	SaveServerInfoListEntry();
	SaveServerInfo(id, info.name, info.ip, info.port);
	insertItem(id, info.name, info.ip, info.port);
	Invalidate();
}


void CServerRegiDlg::OnNMClickServerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSel = m_list_Server.GetSelectionMark();
	if (nSel == -1) return;

	//ST_SPIDER_DISCOVERY_CAMEAR_INFO stInfo;
	//SPIDER_GetDiscoveryCameraInfo(nSel, &stInfo);
	TraceLog(("show---1"));
	// 같은걸 또 찍은 경우, 하지 않는다.
	if (m_selected >= 0 && m_selected == nSel) {
		return;
	}
	m_selected = nSel;
	*pResult = 0;
}


BOOL CServerRegiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_foundedCount = 0;
	m_selected = -1;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_list_Server.SetExtendedStyle(m_list_Server.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int ncol = 0;
	m_list_Server.InsertColumn(ncol++, ("index"), 0, 50);
	m_list_Server.InsertColumn(ncol++, ("name"), 0, 200);
	m_list_Server.InsertColumn(ncol++, ("ip address"), 0, 100);
	m_list_Server.InsertColumn(ncol++, ("port"), 0, 100);


	GetInfoFromIni();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CServerRegiDlg::GetInfoFromIni()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	char buf[1024];
	memset(buf, 0x00, 1024);
	GetPrivateProfileStringA("SOCK_WATCH_SERVER", "IdList", "", buf, 1024, iniPath);

	CString newList = buf;
	if (newList.IsEmpty()) {
		return;
	}
	receiverMap.clear();
	m_foundedCount = 0;

	int pos = 0;
	CString id = newList.Tokenize(",", pos);
	while (!id.IsEmpty())
	{
		RECEIVER_INFO info;

		CString entry;
		entry.Format("SOCK_WATCH_SERVER_%s", id);

		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "NAME", "", buf, 1024, iniPath);
		info.name = buf;

		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "IP", "", buf, 1024, iniPath);
		info.ip = buf;

		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "PORT", "", buf, 1024, iniPath);
		info.port = buf;

		receiverMap[id] = info;
		insertItem(id, info.name, info.ip, info.port);
		id = newList.Tokenize(",", pos);
	}
}

void CServerRegiDlg::SaveServerInfoListEntry()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString newList;
	std::map<CString, RECEIVER_INFO>::iterator itr;
	for (itr = receiverMap.begin(); itr != receiverMap.end(); itr++){
		if (!newList.IsEmpty()){
			newList += ",";
		}
		newList += itr->first;
	}
	WritePrivateProfileString("SOCK_WATCH_SERVER", "IdList", newList, iniPath);
}

void CServerRegiDlg::SaveServerInfo(const char* id, const char* name, const char* ip, const char* port)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString entry;
	entry.Format("SOCK_WATCH_SERVER_%s", id);

	WritePrivateProfileString(entry, "NAME", name, iniPath);
	WritePrivateProfileString(entry, "IP", ip, iniPath);
	WritePrivateProfileString(entry, "PORT",port, iniPath);

}

void CServerRegiDlg::insertItemAll()
{
	m_list_Server.DeleteAllItems();
	m_foundedCount = 0;

	std::map<CString, RECEIVER_INFO>::iterator itr;
	for (itr = receiverMap.begin(); itr != receiverMap.end(); itr++){
		insertItem(itr->first, itr->second.name, itr->second.ip, itr->second.port);
	}
}
void CServerRegiDlg::insertItem(const char* id, const char* name, const char* ip, const char* port)
{
	m_list_Server.InsertItem(m_foundedCount, id);

	int ncol = 1;
	/*CString sztext = id;
	m_list_Server.SetItemText(m_foundedCount, ncol++, sztext);*/

	CString sztext = name;
	m_list_Server.SetItemText(m_foundedCount, ncol++, sztext);

	sztext = ip;
	m_list_Server.SetItemText(m_foundedCount, ncol++, sztext);

	sztext = port;
	m_list_Server.SetItemText(m_foundedCount, ncol++, sztext);

	m_foundedCount++;
	TraceLog(("insertServerInfo %d", m_foundedCount));

}
