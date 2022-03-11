// SampleSendMailDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SampleSendMail.h"
#include "SampleSendMailDlg.h"

#include "../CSmtp.h"
#include "CMN/libCommon/SecurityIni.h"

#define BUF_SIZE			(1024*100)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSampleSendMailDlg ��ȭ ����




CSampleSendMailDlg::CSampleSendMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleSendMailDlg::IDD, pParent)
	, m_strSMTPServer(_T(""))
	, m_strSMTPPort(_T(""))
	, m_strSMTPID(_T(""))
	, m_strSMTPPWD(_T(""))
	, m_strSender(_T(""))
	, m_strReceiver(_T(""))
	, m_strTitle(_T(""))
	, m_strBody(_T(""))
	, m_strConfigPath(_T(""))
	, m_strSSL(_T(""))
	, m_strSenderName(_T(""))
	, m_strReply(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleSendMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SMTP_SERVER_EDT, m_strSMTPServer);
	DDX_Text(pDX, IDC_SMTP_PORT_EDT, m_strSMTPPort);
	DDX_Text(pDX, IDC_SMTP_ID_EDT, m_strSMTPID);
	DDX_Text(pDX, IDC_SMTP_PWD_EDT, m_strSMTPPWD);
	DDX_Text(pDX, IDC_SENDER_EDT, m_strSender);
	DDX_Text(pDX, IDC_RECEIVER_EDT, m_strReceiver);
	DDX_Text(pDX, IDC_TITLE_EDT, m_strTitle);
	DDX_Text(pDX, IDC_BODY_EDT, m_strBody);
	DDX_Text(pDX, IDC_SMTP_SSL_EDT, m_strSSL);
	DDX_Text(pDX, IDC_SENDER_NAME_EDT, m_strSenderName);
	DDX_Text(pDX, IDC_REPLY_EDT, m_strReply);
}

BEGIN_MESSAGE_MAP(CSampleSendMailDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSampleSendMailDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSampleSendMailDlg �޽��� ó����

BOOL CSampleSendMailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
/*
	m_strSMTPServer = "smtp.gmail.com";
	m_strSMTPPort = "587";
	
	m_comboSSL.AddString("TLS");
	m_comboSSL.AddString("SSL");
	m_comboSSL.AddString("NONE");
	m_comboSSL.SetCurSel(2);

	UpdateData(FALSE);
*/
	ReadConfigFile();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSampleSendMailDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSampleSendMailDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSampleSendMailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSampleSendMailDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	try
	{
		CSmtp sendMail((LPCSTR)m_strConfigPath, "KIA");

		UpdateData(TRUE);
	
		///////////////////////////////////////////////////////////
		//������
		CString strTok;
		std::list<string> listRecipient;
		int nPos = 0;
		//';'�� �������� �� �����ڸ� �и�
		m_strReceiver.Replace(" ", "");				//��������
		strTok = m_strReceiver.Tokenize(";", nPos);
		while(strTok != "")
		{
			listRecipient.push_back((LPCSTR)strTok);
			strTok = m_strReceiver.Tokenize(";", nPos);		
		}//while

		TRACE(m_strBody);

		sendMail.Send(listRecipient, m_strTitle, (LPCSTR)m_strBody);
	}
	catch(ECSmtp e)
	{
		AfxMessageBox(e.GetErrorText().c_str());
		return;
	}//try

	AfxMessageBox("Mail was send successfully");

	OnOK();
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// SMTP���� ������ �Ǿ��ִ� ini ������ �д´�. \n
/////////////////////////////////////////////////////////////////////////////////
void CSampleSendMailDlg::ReadConfigFile()
{
	char cModule[MAX_PATH];
	::ZeroMemory(cModule, MAX_PATH);
	::GetModuleFileName(NULL, cModule, MAX_PATH);

	char cDrive[MAX_PATH], cPath[MAX_PATH], cFilename[MAX_PATH], cExt[MAX_PATH];
	_splitpath(cModule, cDrive, cPath, cFilename, cExt);

	m_strConfigPath = cDrive;
	m_strConfigPath += cPath;
	m_strConfigPath += "UBCTT.ini";

	int nSMTPPort, nSecurityType;
	CSecurityIni iniSec;

	//SMTP_SERVER
	m_strSMTPServer = iniSec.ReadValue("SMTP_KIA", "SMTP_SERVER", m_strConfigPath);

	//SMTP_PORT
	m_strSMTPPort = iniSec.ReadValue("SMTP_KIA", "SMTP_PORT", m_strConfigPath);

	//SECURITY_TYPE
	m_strSSL = iniSec.ReadValue("SMTP_KIA", "SECURITY_TYPE", m_strConfigPath);
	if(m_strSSL == 1)
	{
		m_strSSL = "TLS";
	}
	else if(m_strSSL == 2)
	{
		m_strSSL = "SSL";
	}
	else
	{
		m_strSSL = "UnKnown";
	}//if

	//SMTP_ID
	m_strSMTPID = iniSec.ReadValue("SMTP_KIA", "SMTP_ID", m_strConfigPath);

	//SMTP_PWD
	m_strSMTPPWD = iniSec.ReadValue("SMTP_KIA", "SMTP_PWD", m_strConfigPath);

	//SENDER_NAME
	m_strSenderName = iniSec.ReadValue("SMTP_KIA", "SENDER_NAME", m_strConfigPath);

	//SENDER_ADDR
	m_strSender = iniSec.ReadValue("SMTP_KIA", "SENDER_ADDR", m_strConfigPath);

	//REPLY_ADDR
	m_strReply = iniSec.ReadValue("SMTP_KIA", "REPLY_ADDR", m_strConfigPath);

	UpdateData(FALSE);
}
