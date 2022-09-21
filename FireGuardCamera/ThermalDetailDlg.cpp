// ThermalDetailDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThermalDetailDlg.h"
#include "skpark\util.h"
#include "skpark\TraceLog.h"

// CThermalDetailDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CThermalDetailDlg, CDialog)

CThermalDetailDlg::CThermalDetailDlg(const char* id, const  char* ip, const char* pwd, CWnd* pParent /*=NULL*/)
	: CDialog(CThermalDetailDlg::IDD, pParent)
	,m_centerIdMap(0)
	,m_hasDupCheck(false)
	, m_ipAddress(ip)
	, m_cameraId(id)
	, m_pwd(pwd)
{

}

CThermalDetailDlg::~CThermalDetailDlg()
{
}

void CThermalDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, m_editCameraId);
	DDX_Control(pDX, IDC_IPADDRESS, m_editIp);
	DDX_Control(pDX, IDC_BT_DUPCHECK, m_btDupCheck);
	DDX_Control(pDX, IDC_EDIT_PWD, m_editPWD);
}


BEGIN_MESSAGE_MAP(CThermalDetailDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_DUPCHECK, &CThermalDetailDlg::OnBnClickedBtDupcheck)
	ON_BN_CLICKED(IDOK, &CThermalDetailDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CThermalDetailDlg 메시지 처리기입니다.

BOOL CThermalDetailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_ipAddress.IsEmpty() && !m_cameraId.IsEmpty())
	{
		// ModifyMode
		m_editCameraId.EnableWindow(FALSE);
		m_btDupCheck.EnableWindow(FALSE);
		m_editCameraId.SetWindowTextA(m_cameraId);
		m_editIp.SetWindowTextA(m_ipAddress);
		m_hasDupCheck = true;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CThermalDetailDlg::OnBnClickedBtDupcheck()
{
	this->m_editCameraId.GetWindowTextA(m_cameraId);
	if (m_centerIdMap && !m_cameraId.IsEmpty())
	{
		std::map<CString, CString>::iterator itr = m_centerIdMap->find(m_cameraId);

		TraceLog(("OnBnClickedBtDupcheck %s", m_cameraId));
		
		if(itr!=m_centerIdMap->end())
		{
			m_hasDupCheck = false;
			AfxMessageBox("중복된 ID 가 존재합니다. 다른 숫자를 사용하십시오");
			return;
		}
		int id = atoi(m_cameraId);
		if (id > 8 || id < 1) {
			m_hasDupCheck = false;
			AfxMessageBox("ID 는 1에서 8 사이의 숫자만 사용할 수 있습니다. 다른 숫자를 사용하십시오");
			return;
		}
		m_hasDupCheck = true;
		AfxMessageBox("사용할 수 있는  ID 입니다.");
	}
}

void CThermalDetailDlg::OnBnClickedOk()
{

	if(m_hasDupCheck == false)
	{
		AfxMessageBox("ID 중복체크를 수행하여 주십시오.");
		return;
	}
	
	this->m_editIp.GetWindowTextA(m_ipAddress);
	this->m_editCameraId.GetWindowTextA(m_cameraId);
	this->m_editPWD.GetWindowTextA(m_pwd);

	if(m_ipAddress.IsEmpty() || m_cameraId.IsEmpty() || m_pwd.IsEmpty())
	{
		AfxMessageBox("카메라 ID, IP 주소, 패스워드는 모두 필수 입력항목입니다.");
		return;
	}
	OnOK();
}
