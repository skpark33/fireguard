// SampleSendMailDlg.cpp : 구현 파일
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CSampleSendMailDlg 대화 상자




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


// CSampleSendMailDlg 메시지 처리기

BOOL CSampleSendMailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSampleSendMailDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSampleSendMailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSampleSendMailDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	try
	{
		CSmtp sendMail((LPCSTR)m_strConfigPath, "KIA");

		UpdateData(TRUE);
	
		///////////////////////////////////////////////////////////
		//수신자
		CString strTok;
		std::list<string> listRecipient;
		int nPos = 0;
		//';'를 기준으로 각 수신자를 분리
		m_strReceiver.Replace(" ", "");				//공백제거
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
/// SMTP관련 설정이 되어있는 ini 파일을 읽는다. \n
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
