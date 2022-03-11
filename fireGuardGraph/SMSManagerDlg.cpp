// SMSManagerDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "fireGuardGraph.h"
#include "SMSManagerDlg.h"
#include "afxdialogex.h"
#include "LogManager.h"
#include "skpark/TraceLog.h"
#include "resource.h"
#include "ci/libAes256/ciElCryptoAes256.h"
//#include "EzvizDDNSAPI.h"
//#include "common/libSenMail/CSmtp.h"

// SMSManagerDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(SMSManagerDlg, CDialogEx)

SMSManagerDlg::SMSManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SMSManagerDlg::IDD, pParent)
{

}

SMSManagerDlg::~SMSManagerDlg()
{
}

void SMSManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TELNO1, m_editTelephone[0]);
	DDX_Control(pDX, IDC_EDIT_TELNO2, m_editTelephone[1]);
	DDX_Control(pDX, IDC_EDIT_TELNO3, m_editTelephone[2]);
	DDX_Control(pDX, IDC_EDIT_TELNO4, m_editTelephone[3]);
	DDX_Control(pDX, IDC_CHECK_SMS1, m_checkSMS[0]);
	DDX_Control(pDX, IDC_CHECK_SMS2, m_checkSMS[1]);
	DDX_Control(pDX, IDC_CHECK_SMS3, m_checkSMS[2]);
	DDX_Control(pDX, IDC_CHECK_SMS4, m_checkSMS[3]);
	DDX_Control(pDX, IDC_EDIT_NAME1, m_editName[0]);
	DDX_Control(pDX, IDC_EDIT_NAME2, m_editName[1]);
	DDX_Control(pDX, IDC_EDIT_NAME3, m_editName[2]);
	DDX_Control(pDX, IDC_EDIT_NAME4, m_editName[3]);
}


BEGIN_MESSAGE_MAP(SMSManagerDlg, CDialogEx)
	ON_BN_CLICKED(ID_BT_DEL1, &SMSManagerDlg::OnBnClickedBtDel1)
	ON_BN_CLICKED(ID_BT_DEL2, &SMSManagerDlg::OnBnClickedBtDel2)
	ON_BN_CLICKED(ID_BT_DEL3, &SMSManagerDlg::OnBnClickedBtDel3)
	ON_BN_CLICKED(ID_BT_DEL4, &SMSManagerDlg::OnBnClickedBtDel4)
	ON_BN_CLICKED(IDOK, &SMSManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SMSManagerDlg �޽��� ó�����Դϴ�.
void SMSManagerDlg::OnBnClickedBtDel1()
{
	UnSet(0);
}
void SMSManagerDlg::OnBnClickedBtDel2()
{
	UnSet(1);
}
void SMSManagerDlg::OnBnClickedBtDel3()
{
	UnSet(2);
}
void SMSManagerDlg::OnBnClickedBtDel4()
{
	UnSet(3);
}

void SMSManagerDlg::UnSet(int i)
{
	m_editTelephone[i].SetWindowTextA("");
	m_checkSMS[i].SetCheck(false);
	m_editName[i].SetWindowTextA("");
}

void SMSManagerDlg::Set(int i)
{
	m_editName[i].SetWindowTextA(m_sendData[i].alias);
	m_checkSMS[i].SetCheck(m_sendData[i].checkSMS);
	m_editTelephone[i].SetWindowTextA(m_sendData[i].tel);
}

void SMSManagerDlg::Get(int i, CString& outString)
{
	bool checkSMS = false;

	m_editName[i].GetWindowTextA(m_sendData[i].alias);
	if (m_sendData[i].alias.IsEmpty())
	{
		m_sendData[i].alias = "-";
	}
	m_sendData[i].checkSMS = m_checkSMS[i].GetCheck();
	m_editTelephone[i].GetWindowTextA(m_sendData[i].tel);
	if (m_sendData[i].tel.IsEmpty())
	{
		m_sendData[i].tel = "0";
	}

	outString.Format("%s/%d/%s", m_sendData[i].alias, m_sendData[i].checkSMS, m_sendData[i].tel);
	//std::string decryted = outString;
	//outString = ciAes256Util::Encrypt(decryted, false).c_str();
}

BOOL SMSManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (int i = 0; i < MAX_DATA; i++)
	{
		Set(i);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void SMSManagerDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;
	CString buf[MAX_DATA];

	for (int i = 0; i < MAX_DATA; i++)
	{
		Get(i, buf[i]);

		CString name;
		name.Format("STR%d", i);
		WritePrivateProfileStringA("FIRE_WATCH", name, buf[i], iniPath);

	}
	CDialogEx::OnOK();
}



//
//bool SMSManagerDlg::SendMail(LPCTSTR enterprise, LPCTSTR receiver, LPCTSTR title, LPCTSTR text)
//{
//	// Send Mail Here
//	TraceLog(("sendMail(%s,%s,%s)", receiver, title, text));
//	CString ini = UBC_CONFIG_PATH;
//	ini += "UBCTT.ini";
//
//	CString receiverStr = receiver;
//
//	CSmtp	aTT(std::string(ini), enterprise);
//	TraceLog(("before sendEmail(%s,%s,%s)", receiverStr, title, text));
//	return aTT.Send(receiverStr, title, text);
//}