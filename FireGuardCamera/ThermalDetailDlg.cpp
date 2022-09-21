// ThermalDetailDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ThermalDetailDlg.h"
#include "skpark\util.h"
#include "skpark\TraceLog.h"

// CThermalDetailDlg ��ȭ �����Դϴ�.

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


// CThermalDetailDlg �޽��� ó�����Դϴ�.

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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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
			AfxMessageBox("�ߺ��� ID �� �����մϴ�. �ٸ� ���ڸ� ����Ͻʽÿ�");
			return;
		}
		int id = atoi(m_cameraId);
		if (id > 8 || id < 1) {
			m_hasDupCheck = false;
			AfxMessageBox("ID �� 1���� 8 ������ ���ڸ� ����� �� �ֽ��ϴ�. �ٸ� ���ڸ� ����Ͻʽÿ�");
			return;
		}
		m_hasDupCheck = true;
		AfxMessageBox("����� �� �ִ�  ID �Դϴ�.");
	}
}

void CThermalDetailDlg::OnBnClickedOk()
{

	if(m_hasDupCheck == false)
	{
		AfxMessageBox("ID �ߺ�üũ�� �����Ͽ� �ֽʽÿ�.");
		return;
	}
	
	this->m_editIp.GetWindowTextA(m_ipAddress);
	this->m_editCameraId.GetWindowTextA(m_cameraId);
	this->m_editPWD.GetWindowTextA(m_pwd);

	if(m_ipAddress.IsEmpty() || m_cameraId.IsEmpty() || m_pwd.IsEmpty())
	{
		AfxMessageBox("ī�޶� ID, IP �ּ�, �н������ ��� �ʼ� �Է��׸��Դϴ�.");
		return;
	}
	OnOK();
}
