
// FireGuardCameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FireGuardCamera.h"
#include "FireGuardCameraDlg.h"

#include "DeviceInfoDlg.h"
#include "StreamInfoDlg.h"
#include "MotionInfoDlg.h"
#include "FeatureInfoDlg.h"
#include "EventScheduleInfoDlg.h"
#include "EventTriggerInfoDlg.h"
#include "NetworkIPAddrInfoDlg.h"
#include "NotifyMailingInfoDlg.h"
#include "NetworkIPFilterInfoDlg.h"
#include "OverlayTextInfoDlg.h"
#include "PrivacyMaskInfoDlg.h"
#include "TimeInfoDlg.h"
#include "UserInfoDlg.h"
#include "CustomTriggerInfoDlg.h"
#include "CustomStrobeInfoDlg.h"
#include "SerialPortInfoDlg.h"
#include "CustomCtrlDlg.h"
#include "StreamROIInfoDlg.h"
#include "ThermalSpotInfoDlg.h"
#include "ThermalCtrlInfoDlg.h"
#include "DigitalIOInfoDlg.h"
#include "DynamicDNSInfoDlg.h"
#include "SNMPInfoDlg.h"
#include "PPPoEInfoDlg.h"
#include "skpark/TraceLog.h"
#include "skpark/FireProcess.h"
#include "skpark/util.h"
#include "ThermalDetailDlg.h"
#include "ServerRegiDlg.h"
#include "skpark/util.h"

static char gszCurDir[MAX_PATH] = "";


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CFireGuardCameraDlg dialog


CFireGuardCameraDlg::CFireGuardCameraDlg(bool hikvision , CWnd* pParent /*=NULL*/)
	: CDialog(CFireGuardCameraDlg::IDD, pParent)
	, m_isHikvision(hikvision)
	, m_foundedCount(0)  //skpark in your area
	, m_initDone(false)
	, m_stopPolling(false)
	, m_selected(-1)
{
	
	for (int i = 0; i < MAX_CAMERA; i++) {
		theApp.m_allCamera[i] = NULL;
		memset(rtspurl[i], 0x00, 500);
	}


	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hDiscoveryEvent = NULL;
	m_hThread_Discovery = NULL;
	m_dwThreadID_Discovery = 0;
	m_dwThreadID_Polling = 0;  //skpark in your area

	m_hTrigger = NULL;
	m_hSyslog = NULL;

	m_bInitTriggerServer = false;
	m_bInitSyslogServer = false;


	::GetModuleFileNameA(::AfxGetInstanceHandle(), gszCurDir, MAX_PATH);
	int nfind = strlen(gszCurDir);
	if(nfind)
	{
		nfind -= 1;
		while(gszCurDir[nfind] != '\\' && nfind > 0)
		{
			nfind--;
		}
		gszCurDir[nfind+1] = '\0';
	}

}

void CFireGuardCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_CAMERA, m_List_Camera);	
	DDX_Control(pDX, IDC_COMBO_CH_SNAPSHOT, m_cbChSnapShot);
}

BEGIN_MESSAGE_MAP(CFireGuardCameraDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DISCOVERY, &CFireGuardCameraDlg::OnBnClickedButtonDiscovery)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CAMERA, &CFireGuardCameraDlg::OnNMClickListCamera)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE, &CFireGuardCameraDlg::OnBnClickedButtonDevice)
	ON_BN_CLICKED(IDC_BUTTON_FEATURE, &CFireGuardCameraDlg::OnBnClickedButtonFeature)
	ON_BN_CLICKED(IDC_BUTTON_OVERLAY_TEXT, &CFireGuardCameraDlg::OnBnClickedButtonOverlayText)
	ON_BN_CLICKED(IDC_BUTTON_MOTION, &CFireGuardCameraDlg::OnBnClickedButtonMotion)
	ON_BN_CLICKED(IDC_BUTTON_PRIVACY_MASK, &CFireGuardCameraDlg::OnBnClickedButtonPrivacyMask)
	ON_BN_CLICKED(IDC_BUTTON_STREAM, &CFireGuardCameraDlg::OnBnClickedButtonStream)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &CFireGuardCameraDlg::OnBnClickedButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_USER, &CFireGuardCameraDlg::OnBnClickedButtonUser)
	ON_BN_CLICKED(IDC_BUTTON_NETWORK_IPADDRESS, &CFireGuardCameraDlg::OnBnClickedButtonNetworkIpaddress)
	ON_BN_CLICKED(IDC_BUTTON_NETWORK_IP_FILTER, &CFireGuardCameraDlg::OnBnClickedButtonNetworkIpFilter)
	ON_BN_CLICKED(IDC_BUTTON_NOTIFY_MAILING, &CFireGuardCameraDlg::OnBnClickedButtonNotifyMailing)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_TRIGGER, &CFireGuardCameraDlg::OnBnClickedButtonEventTrigger)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_SCHEDULE, &CFireGuardCameraDlg::OnBnClickedButtonEventSchedule)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_TRIGGER, &CFireGuardCameraDlg::OnBnClickedButtonCustomTrigger)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_STROBE, &CFireGuardCameraDlg::OnBnClickedButtonCustomStrobe)
	ON_BN_CLICKED(IDC_BUTTON_STORAGE, &CFireGuardCameraDlg::OnBnClickedButtonStorage)
	//ON_BN_CLICKED(IDC_BUTTON_CUSTOM_STORAGE, &CFireGuardCameraDlg::OnBnClickedButtonCustomStorage)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_STORAGE, &CFireGuardCameraDlg::OnBnClickedButtonIpManager)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_CTRL, &CFireGuardCameraDlg::OnBnClickedButtonCustomCtrl)
	ON_MESSAGE(WM_TRIGGER, OnTriggerData)
	ON_MESSAGE(WM_SYSLOG, OnSyslogData)
	ON_BN_CLICKED(IDC_BUTTON_STREAMING, &CFireGuardCameraDlg::OnBnClickedButtonStreaming)
	ON_BN_CLICKED(IDC_BUTTON_REBOOT, &CFireGuardCameraDlg::OnBnClickedButtonReboot)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CFireGuardCameraDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CFireGuardCameraDlg::OnBnClickedButtonImport)	
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, &CFireGuardCameraDlg::OnBnClickedButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_SERIALPORT, &CFireGuardCameraDlg::OnBnClickedButtonSerialport)	
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL_SNAPSHOT, &CFireGuardCameraDlg::OnBnClickedButtonChannelSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_STREAM_ROI_INFO, &CFireGuardCameraDlg::OnBnClickedButtonStreamRoiInfo)
	//ON_BN_CLICKED(IDC_BUTTON_THERMAL_SPOT, &CFireGuardCameraDlg::OnBnClickedButtonThermalSpot)
	ON_BN_CLICKED(IDC_BUTTON_THERMAL_SPOT, &CFireGuardCameraDlg::OnBnClickedButtonOpenWeb)
	ON_BN_CLICKED(IDC_BUTTON_DIGITAL_IO, &CFireGuardCameraDlg::OnBnClickedButtonDigitalIo)
	ON_BN_CLICKED(IDC_BUTTON_DDNS, &CFireGuardCameraDlg::OnBnClickedButtonDdns)
	ON_BN_CLICKED(IDC_BUTTON_SNMP, &CFireGuardCameraDlg::OnBnClickedButtonSnmp)
	ON_BN_CLICKED(IDC_BUTTON_PPPOE, &CFireGuardCameraDlg::OnBnClickedButtonPppoe)
	ON_BN_CLICKED(IDC_BUTTON_LOG_CLEAR, &CFireGuardCameraDlg::OnBnClickedButtonLogClear)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CAMERA, &CFireGuardCameraDlg::OnBnClickedButtonAddCamera)
	ON_BN_CLICKED(IDC_BUTTON_DEL_CAMERA, &CFireGuardCameraDlg::OnBnClickedButtonDelCamera)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_IP, &CFireGuardCameraDlg::OnBnClickedButtonChangeIp)
END_MESSAGE_MAP()


// CFireGuardCameraDlg message handlers

BOOL CFireGuardCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_List_Camera.SetExtendedStyle( m_List_Camera.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	int ncol = 0;
	m_List_Camera.InsertColumn(ncol++, ("index"), 0, 50);
	m_List_Camera.InsertColumn(ncol++, ("vendor name"), 0, 100);
	m_List_Camera.InsertColumn(ncol++, ("ip address"), 0, 100);
	m_List_Camera.InsertColumn(ncol++, ("model name"), 0, 100);
	m_List_Camera.InsertColumn(ncol++, ("mac address"), 0, 100);
	m_List_Camera.InsertColumn(ncol++, ("description"), 0, 160);
	
	m_bInitTriggerServer = true;
	SPIDER_InitTriggerServer();

	m_bInitSyslogServer = true;
	SPIDER_InitSyslogServer();

	CString strChannel = _T("");
	for (int i=0; i<MAX_CAMERA; i++) {
		strChannel.Format(_T("%d"), i+1);
		m_cbChSnapShot.InsertString(i, strChannel);
	}
	m_cbChSnapShot.SetCurSel(0);

	OnBnClickedButtonDiscovery();  //skpark in your area
	m_hThread_Polling = CreateThread(NULL, 0, ThreadProc_Polling, this, 0, &m_dwThreadID_Polling); //skpark in your area

	HWND desktop = ::GetDesktopWindow();
	CRect rcDesk, rcWindow;
	::GetWindowRect(desktop, &rcDesk);
	::GetWindowRect(this->m_hWnd,  &rcWindow);

	TraceLog(("Monitor Size = %d : %d ", rcDesk.Width(), rcDesk.Height()));
	TraceLog(("Windows Size = %d : %d ", rcWindow.Width(), rcWindow.Height()));  //621x590

	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_isHikvision);
	GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(!m_isHikvision);
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(!m_isHikvision);
	GetDlgItem(IDC_BUTTON_STREAMING)->EnableWindow(!m_isHikvision);

	GetDlgItem(IDC_BUTTON_ADD_CAMERA)->EnableWindow(m_isHikvision);
	GetDlgItem(IDC_BUTTON_DEL_CAMERA)->EnableWindow(m_isHikvision);
	GetDlgItem(IDC_BUTTON_CHANGE_IP)->EnableWindow(m_isHikvision);

	::SetWindowPos(this->m_hWnd, HWND_TOP, 10, 10, -1, -1, SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	LicenseUtil::LicenseCheck();
	
	//AfxBeginThread(CFireGuardCameraDlg::ProcessPopup, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFireGuardCameraDlg::OnClose()
{
	this->m_stopPolling = true;
}

void CFireGuardCameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFireGuardCameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFireGuardCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFireGuardCameraDlg::OnBnClickedButtonDiscovery()
{
	if(m_hThread_Discovery)
	{
		MessageBox(("Discovery is running..."));
		return;
	}


	if(WaitDlg.m_hEvent_InitDlg == NULL)
		WaitDlg.m_hEvent_InitDlg = CreateEvent(0,0,0,0);

	m_hThread_Discovery = CreateThread(NULL, 0, ThreadProc_Discovery, this, 0, &m_dwThreadID_Discovery );

	WaitDlg.DoModal();

}

DWORD CFireGuardCameraDlg::ThreadProc_Discovery(LPVOID lpParameter)
{
	((CFireGuardCameraDlg*)lpParameter)->StartDiscovery();

	//((CFireGuardCameraDlg*)lpParameter)->m_dwThreadID_Discovery;

	if(((CFireGuardCameraDlg*)lpParameter)->m_hThread_Discovery)
		CloseHandle(((CFireGuardCameraDlg*)lpParameter)->m_hThread_Discovery);
	((CFireGuardCameraDlg*)lpParameter)->m_hThread_Discovery = NULL;

	//skpark in your area start
	((CFireGuardCameraDlg*)lpParameter)->GetAllCamera();
	((CFireGuardCameraDlg*)lpParameter)->m_initDone = true;
	//skpark in your area end

	ExitThread(0x2345);
}

DWORD CFireGuardCameraDlg::ThreadProc_Polling(LPVOID lpParameter)
{
	((CFireGuardCameraDlg*)lpParameter)->Polling();

	//((CFireGuardCameraDlg*)lpParameter)->m_dwThreadID_Polling;
	if (((CFireGuardCameraDlg*)lpParameter)->m_hThread_Polling)
		CloseHandle(((CFireGuardCameraDlg*)lpParameter)->m_hThread_Polling);
	((CFireGuardCameraDlg*)lpParameter)->m_hThread_Polling = NULL;

	ExitThread(0x2345);
}

void CFireGuardCameraDlg::StartDiscovery()
{
	//wait for init dlg
	WaitForSingleObject(WaitDlg.m_hEvent_InitDlg, INFINITE);

	CString sztext;

	if(m_hDiscoveryEvent == NULL)
		SPIDER_InitDiscovery(&m_hDiscoveryEvent);
	
	m_foundedCount = SPIDER_GetDiscoveryCameraCount();

	WaitDlg.SetTimer(TIMER_CLOSE, 100, NULL);


	m_List_Camera.DeleteAllItems();
	m_idIpMap.clear();

	//ST_SPIDER_DISCOVERY_CAMEAR_INFO stInfo;
	for (int i = 0; i < m_foundedCount; i++)
	{
		//SPIDER_GetDiscoveryCameraInfo(i, &stInfo);
		memset(&(m_stInfo[i]), 0x00, sizeof(ST_SPIDER_DISCOVERY_CAMEAR_INFO));

		if (!m_isHikvision)
		{
			SPIDER_GetDiscoveryCameraInfo(i, &(m_stInfo[i]));
		}

		CString id;
		id.Format(("%d"), i+1);
		m_List_Camera.InsertItem(i, id);

		

		int ncol = 1;

		sztext = m_stInfo[i].strVendorName;
		m_List_Camera.SetItemText(i, ncol++, sztext);

		sztext = m_stInfo[i].strIpAddress;
		m_List_Camera.SetItemText(i, ncol++, sztext);
		this->m_idIpMap[id] = sztext; //skpark

		sztext = m_stInfo[i].strModelName;
		m_List_Camera.SetItemText(i, ncol++, sztext);

		sztext = m_stInfo[i].strMacAddr;
		m_List_Camera.SetItemText(i, ncol++, sztext);

		sztext = "";
		//if(m_stInfo[i].nsDiscoverySupport & DISCOVERY_UPNP)
		//	sztext += "upnp ";
		//if(m_stInfo[i].nsDiscoverySupport & DISCOVERY_ZEROCONF)
		//	sztext += "bonjour ";
		//if(m_stInfo[i].nsDiscoverySupport & DISCOVERY_ONVIF)
		//	sztext += "onvif ";
		sztext = m_stInfo[i].strDeviceDes != NULL ? m_stInfo[i].strDeviceDes : "not specified" ;
		m_List_Camera.SetItemText(i, ncol++, sztext);

		


	}

	GetCameraInfoFromIni();


	sztext.Format(("Total Discovered Camera Count : %d"), m_foundedCount);
	GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(sztext);
	//MessageBox(sztext);
}

// skpark in your area
void CFireGuardCameraDlg::Polling()
{
	
	int loop = 0;
	while (!m_stopPolling) {
		if (m_initDone) {
			for (int i = 0; i < m_foundedCount; i++) {
				if (theApp.m_allCamera[i]) {
					if (SPIDER_GetThermalRegionTemp(theApp.m_allCamera[i], &m_stTemp[i]) == E_CTRL_OK){
						
						CString strIndex= _T("");
						CString strValid = _T("");
						CString strAlarm = _T("");
						CString strCenter = _T("");
						CString strMin = _T("");
						CString strMax = _T("");
						CString strAvg = _T("");

						float temperature = m_stTemp[i].ThermalFullTemp.fMaxTempValue;

						strValid.Format(m_stTemp[i].ThermalFullTemp.bValid ? _T("O") : _T("X"));
						strAlarm.Format(m_stTemp[i].ThermalFullTemp.bAlarmOn ? _T("On") : _T("Off"));
						strIndex.Format(_T("%d"), i+1);
						strCenter.Format(_T("%.2f"), m_stTemp[i].ThermalFullTemp.fCenterTempValue);
						strMin.Format(_T("%.2f"), m_stTemp[i].ThermalFullTemp.fMinTempValue);
						strMax.Format(_T("%.2f"), temperature);
						strAvg.Format(_T("%.2f"), m_stTemp[i].ThermalFullTemp.fAvgTempValue);

						//TraceLog(("strValid=%s", strValid));
						//TraceLog(("strAlarm=%s", strAlarm));
						//TraceLog(("strCenter=%s", strCenter));
						//TraceLog(("strMin=%s", strMin));
						//TraceLog(("strMax=%s", strMax));
						//TraceLog(("strAvg=%s", strAvg));

						FireProcess::getInstance()->Push(temperature, m_stTemp[i].ThermalFullTemp.bAlarmOn, strIndex);

						m_msgText.Format("%s\r\nValid=%s, Alarm=%s, Center=%s, Min=%s, Max=%s, Avg=%s",
							m_msgText, strValid, strAlarm, strCenter, strMin, strMax, strAvg);

						GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(m_msgText);
					}
				}
			}
			if (loop == 14) {
				loop = 0;
				m_msgText = "";
			}
			loop++;
		}
		::Sleep(FireProcess::getInstance()->monitor_sec*1000);
	}
}

//skpark in your area
void CFireGuardCameraDlg::GetAllCamera()  
{
	m_initDone = false;
	if (!m_isHikvision) {
		for (int i = 0; i < m_foundedCount; i++) {
			SelectOneCamera(i);
		}
	}
	else {
		for (int i = 0; i < m_foundedCount; i++) {
			//RunGuadianCenter(i);  //Starter 에서 하므로 여기서는 Run 하지 않는다.
		}
	}
	FireProcess::getInstance()->SetDlg(this);
	m_initDone = true;
}

void CFireGuardCameraDlg::RunGuadianCenter(int nSel)
{
	CString ip = m_stInfo[nSel].strIpAddress;
	CString pwd = m_stInfo[nSel].strDeviceDes;
	CString id = m_stInfo[nSel].strDeviceName;

	CString arg;
	arg.Format("+id %s +ip %s +pwd %s +fire", id, ip, pwd);

	HINSTANCE nRet = ShellExecuteA(NULL, NULL, "GuardianCenter.exe", arg, gszCurDir, SW_SHOWNORMAL);
	if (int(nRet) < 32)
	{
		TraceLog(("ShellExecute GuardianCenter.exe %s Error=%d", arg, nRet));
		return;
	}

	TraceLog(("ShellExecute GuardianCenter.exe %s succeed=%d", arg, nRet));
}

void CFireGuardCameraDlg::SelectOneCamera(int nSel)
{
	if (nSel == -1 ||  nSel >= m_foundedCount ) return;

	//ST_SPIDER_DISCOVERY_CAMEAR_INFO stInfo;
	//SPIDER_GetDiscoveryCameraInfo(nSel, &stInfo);

	char szip[100] = "";
	m_List_Camera.GetItemText(nSel, 2, szip, 50);

	if (theApp.m_allCamera[nSel])
		SPIDER_DestroyCameraHandle(theApp.m_allCamera[nSel]);
	theApp.m_allCamera[nSel] = NULL;


	HCAMERA hCamera = SPIDER_CreateCameraHandle(szip, 80, FireProcess::getInstance()->id, FireProcess::getInstance()->pwd);
	theApp.m_allCamera[nSel] = hCamera;
	if (hCamera)
	{
		CString sztext;
		long lret = E_CTRL_OK;
		long nCount = 0;

		sztext = "Camera Info of ";
		sztext += szip;
		sztext += "\r\n\r\n";


		sztext += "* Device Info ------------------\r\n";
		ST_SPIDER_DEVICE_INFO stDevice = { 0 };
		lret = SPIDER_GetDeviceInfo(hCamera, &stDevice);
		if (lret != E_CTRL_OK)
		{
			sztext += "device info : ";
			sztext += GetErrorString(lret);
			sztext += "\r\n";
		}
		else
		{
			sztext += "name : ";
			sztext += stDevice.device_name;
			sztext += "\r\n";

			sztext += "model name : ";
			sztext += stDevice.model_name;
			sztext += "\r\n";

			sztext += "serial number : ";
			sztext += stDevice.serial_number;
			sztext += "\r\n";

			sztext += "mac address : ";
			sztext += stDevice.mac_address;
			sztext += "\r\n";

			sztext += "firmware version : ";
			sztext += stDevice.firmware_version;
			sztext += "\r\n";
		}
		sztext += "\r\n";

		sztext += "* Stream Info ------------------\r\n";
		ST_SPIDER_STREAM_INFO stStream = { 0 };
		nCount = SPIDER_GetStreamCount(hCamera);

		CString szInfo;
		szInfo.Format(("Stream Count : %d\r\n"), nCount);
		sztext += szInfo;
		for (int i = 0; i < nCount; i++)
		{
			szInfo.Format(("Stream [%d] "), i + 1);
			sztext += szInfo;

			lret = SPIDER_GetStreamInfo(hCamera, i, &stStream);
			if (lret != E_CTRL_OK)
			{
				sztext += "stream info : ";
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				switch (stStream.stVideo.nsVideoCodecType)
				{
				case E_COCEC_MPEG4: sztext += "mpeg4"; break;
				case E_COCEC_MJPEG: sztext += "mjpeg"; break;
				case E_COCEC_H264: sztext += "h.264"; break;
				default: sztext += "unknown"; break;
				}

				szInfo.Format(("( %d x %d )"), stStream.stVideo.nWidth, stStream.stVideo.nHeight);
				sztext += szInfo;

				char szurl[300] = "";
				SPIDER_GetStreamingURL(hCamera, i, szurl);

				sztext += " stream url : ";
				sztext += szurl;
			}
			sztext += "\r\n";
		}
		sztext += "\r\n";

		sztext += "* Time Info ------------------\r\n";
		ST_SPIDER_TIME_INFO stTime = { 0 };
		lret = SPIDER_GetTimeInfo(hCamera, &stTime);
		if (lret != E_CTRL_OK)
		{
			sztext += "time info : ";
			sztext += GetErrorString(lret);
			sztext += "\r\n";
		}
		else
		{
			sztext += "local time : ";
			sztext += stTime.strLocalTime;
			sztext += "\r\n";

			sztext += "time zone : ";
			sztext += stTime.strTimeZone;
			sztext += "\r\n";
		}
		sztext += "\r\n";



		sztext += "* Custom Info ------------------\r\n";
		ENUM_SPIDER_CUSTOM_ASPECTRATIO eAspectRatio;
		lret = SPIDER_GetCustomAspectRatio(hCamera, &eAspectRatio);
		if (lret != E_CTRL_OK)
		{
			sztext += "aspect ratio : ";
			sztext += GetErrorString(lret);
			sztext += "\r\n";
		}
		else
		{
			sztext += "aspect ratio : ";
			switch (eAspectRatio)
			{
			case E_ASPECT_RAIO_CROPPING: sztext += "cropping"; break;
			case E_ASPECT_RAIO_SQUEEZING: sztext += "squeezing"; break;
			default: sztext += "unknown"; break;
			}
			sztext += "\r\n";
		}
		sztext += "\r\n";


		ENUM_SPIDER_CUSTOM_LINKSPEED eLinkSpeed;
		lret = SPIDER_GetCustomLinkSpeed(hCamera, &eLinkSpeed);
		if (lret != E_CTRL_OK)
		{
			sztext += "link speed : ";
			sztext += GetErrorString(lret);
			sztext += "\r\n";
		}
		else
		{
			sztext += "link speed : ";
			switch (eLinkSpeed)
			{
			case E_LINK_SPEED_AUTO: sztext += "auto Nego"; break;
			case E_LINK_SPEED_FULL100: sztext += "100M full"; break;
			case E_LINK_SPEED_HALF100: sztext += "100M half"; break;
			case E_LINK_SPEED_FULL10: sztext += "10M full"; break;
			case E_LINK_SPEED_HALF10: sztext += "10M half"; break;
			default: sztext += "unknown"; break;
			}
			sztext += "\r\n";
		}
		sztext += "\r\n";

		bool bEnable;
		lret = SPIDER_GetCustomSuppressColorNightMode(hCamera, &bEnable);
		if (lret != E_CTRL_OK)
		{
			sztext += "suppress color night mode : ";
			sztext += GetErrorString(lret);
			sztext += "\r\n";
		}
		else
		{
			sztext += "suppress color night mode : ";
			sztext += bEnable ? "enable" : "disable";
			sztext += "\r\n";
		}
		sztext += "\r\n";


		sztext += "* Custom Trigger Info ------------------\r\n";
		ST_SPIDER_CUSTOM_TRIGGER_INFO stCustomTrigger = { 0 };
		lret = SPIDER_GetCustomTriggerInfo(hCamera, &stCustomTrigger);
		if (lret != E_CTRL_OK)
		{
			sztext += GetErrorString(lret);
			sztext += "\r\n";
		}
		else
		{
			sztext += "Source : ";
			if (stCustomTrigger.nsSource == 0)
				sztext += "software";
			else if (stCustomTrigger.nsSource == 1)
				sztext += "hardware";
			else if (stCustomTrigger.nsSource == 2)
				sztext += "periodic";
			else if (stCustomTrigger.nsSource == 3)
				sztext += "filterCtrl";
			else
				sztext += "NA";
			sztext += "\r\n";


			szInfo.Format(("%d"), stCustomTrigger.nDelay);
			sztext += "Delay : ";
			sztext += szInfo;
			sztext += "\r\n";

			sztext += "Polarity : ";
			if (stCustomTrigger.nsPolarity == 0)
				sztext += "high";
			else if (stCustomTrigger.nsPolarity == 1)
				sztext += "low";
			else
				sztext += "NA";
			sztext += "\r\n";

			szInfo.Format(("%d"), stCustomTrigger.nsTriggerNoiseFilter);
			sztext += "Noise Filter : ";
			sztext += szInfo;
			sztext += "\r\n";

			sztext += "Server On/Off : ";
			if (stCustomTrigger.nsOnOff == 0)
				sztext += "Off";
			else if (stCustomTrigger.nsOnOff == 1)
				sztext += "On";
			else
				sztext += "NA";
			sztext += "\r\n";

			sztext += "Server Addr : ";
			sztext += stCustomTrigger.strIpAddr;
			sztext += "\r\n";

			szInfo.Format(("%d"), stCustomTrigger.nsPortNo);
			sztext += "Server Port : ";
			sztext += szInfo;
			sztext += "\r\n";

			//szInfo.Format(("%d"), stCustomTrigger.nsParameter);
			//sztext += "Parameter Number : ";
			//sztext += szInfo; 
			//sztext += "\r\n";

			//compare trigger server ip with my computer ip

			if (stCustomTrigger.strIpAddr)
			{
				CString szIPList;
				static CString szMyComIPList;
				static char m_vHostIP[10][50] = { 0 };
				static int nMyComIPCount = 0;
				if (szMyComIPList.GetLength() == 0)
				{
#pragma comment(lib, "IPHlpAPI.lib")
					//get ip list of this computer.
					PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
					ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
					if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
					{
						free(pAdapterInfo);
						pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
					}

					int nIndex = 0;
					DWORD dwRetVal = 0;
					PIP_ADAPTER_INFO pAdapter = NULL;
					if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
					{
						pAdapter = pAdapterInfo;
						while (pAdapter)
						{
							CString szip;
							szip.Format(("[%d] %s\r\n"), nIndex++, pAdapter->IpAddressList.IpAddress.String);
							szIPList += szip;
							strcpy(m_vHostIP[nMyComIPCount], pAdapter->IpAddressList.IpAddress.String);
							nMyComIPCount++;

							pAdapter = pAdapter->Next;
						}
					}

					free(pAdapterInfo);

					if (szIPList.GetLength())
					{
						szMyComIPList = "My Computer IP Address is...\r\n\r\n";
						szMyComIPList += szIPList;
					}
					else
					{
						szMyComIPList = "Failed to get IP Address on this system.\r\n\r\n";
					}
				}

				int nFindIP = 0;
				while (nFindIP < nMyComIPCount)
				{
					if (strcmp(stCustomTrigger.strIpAddr, m_vHostIP[nFindIP]) == 0)
					{
						break;
					}

					nFindIP++;
				}

				if (nFindIP < nMyComIPCount)
				{
					if (m_hTrigger == NULL)
					{
						m_hTrigger = SPIDER_StartTriggerServer(GetSafeHwnd(), stCustomTrigger.nsPortNo);
					}
				}
			}
		}
		sztext += "\r\n";



		ST_SPIDER_CUSTOM_TRIGGER_PARAM_INFO stParam = { 0 };
		nCount = SPIDER_GetCustomTriggerParamCount(hCamera);

		szInfo.Format(("Param Count : %d\r\n"), nCount);
		sztext += szInfo;
		for (int i = 0; i < nCount; i++)
		{
			szInfo.Format(("Param [%d] "), i + 1);
			sztext += szInfo;

			lret = SPIDER_GetCustomTriggerParamInfo(hCamera, i, &stParam);
			if (lret != E_CTRL_OK)
			{
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				szInfo.Format(("Gain : %d"), stParam.nGain);
				sztext += szInfo;
				sztext += "\r\n";

				szInfo.Format(("Shutter : %d"), stParam.nShutter);
				sztext += szInfo;
				sztext += "\r\n";

				szInfo.Format(("Increment : %d"), stParam.nsIncrement);
				sztext += szInfo;
				sztext += "\r\n";
			}
			sztext += "\r\n";
		}
		sztext += "\r\n";


		GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(sztext);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(("Failed to create camera handle"));
	}


}


void CFireGuardCameraDlg::OnNMClickListCamera(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here

	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel == -1) return;
	

		//ST_SPIDER_DISCOVERY_CAMEAR_INFO stInfo;
		//SPIDER_GetDiscoveryCameraInfo(nSel, &stInfo);

	TraceLog(("show---1"));
		// 같은걸 또 찍은 경우, 하지 않는다.
		if (m_selected >= 0 && m_selected == nSel) {
			return;
		}
		
		char szip[100] = "";
		m_List_Camera.GetItemText(nSel, 2, szip, 50);		
		TraceLog(("show---2"));

		//if(theApp.m_hSelectCamera)
		//	SPIDER_DestroyCameraHandle(theApp.m_hSelectCamera);
		//theApp.m_hSelectCamera = NULL;
		TraceLog(("show---3"));

		theApp.m_hSelectCamera = theApp.m_allCamera[nSel];
		m_selected = nSel;
		TraceLog(("show---4"));
		/*
		HCAMERA hCamera = SPIDER_CreateCameraHandle(szip, 80, "admin", "-507263a");
		theApp.m_hSelectCamera = hCamera;
		if(hCamera)
		{
			CString sztext;
			long lret = E_CTRL_OK;
			long nCount = 0;

			sztext = "Camera Info of ";
			sztext += szip;
			sztext += "\r\n\r\n";


			sztext += "* Device Info ------------------\r\n";
			ST_SPIDER_DEVICE_INFO stDevice = {0};
			lret = SPIDER_GetDeviceInfo(hCamera, &stDevice);
			if(lret != E_CTRL_OK)
			{
				sztext += "device info : ";
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				sztext += "name : ";
				sztext += stDevice.device_name;
				sztext += "\r\n";

				sztext += "model name : ";
				sztext += stDevice.model_name;
				sztext += "\r\n";

				sztext += "serial number : ";
				sztext += stDevice.serial_number;
				sztext += "\r\n";

				sztext += "mac address : ";
				sztext += stDevice.mac_address;
				sztext += "\r\n";

				sztext += "firmware version : ";
				sztext += stDevice.firmware_version;
				sztext += "\r\n";
			}
			sztext += "\r\n";

			sztext += "* Stream Info ------------------\r\n";
			ST_SPIDER_STREAM_INFO stStream = {0};
			nCount = SPIDER_GetStreamCount(hCamera);

			CString szInfo;
			szInfo.Format(("Stream Count : %d\r\n"), nCount);
			sztext += szInfo;
			for(int i = 0; i < nCount; i++)
			{
				szInfo.Format(("Stream [%d] "), i+1);
				sztext += szInfo;

				lret = SPIDER_GetStreamInfo(hCamera, i, &stStream);
				if(lret != E_CTRL_OK)
				{
					sztext += "stream info : ";
					sztext += GetErrorString(lret);
					sztext += "\r\n";
				}
				else
				{
					switch(stStream.stVideo.nsVideoCodecType)
					{
					case E_COCEC_MPEG4: sztext += "mpeg4"; break;
					case E_COCEC_MJPEG: sztext += "mjpeg"; break;
					case E_COCEC_H264: sztext += "h.264"; break;
					default: sztext += "unknown"; break;
					}

					szInfo.Format(("( %d x %d )"), stStream.stVideo.nWidth, stStream.stVideo.nHeight);
					sztext += szInfo;

					char szurl[300] = "";
					SPIDER_GetStreamingURL(hCamera, i, szurl);

					sztext += " stream url : ";
					sztext += szurl;
				}
				sztext += "\r\n";
			}
			sztext += "\r\n";

			sztext += "* Time Info ------------------\r\n";
			ST_SPIDER_TIME_INFO stTime = {0};
			lret = SPIDER_GetTimeInfo(hCamera, &stTime);
			if(lret != E_CTRL_OK)
			{
				sztext += "time info : ";
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				sztext += "local time : ";
				sztext += stTime.strLocalTime;
				sztext += "\r\n";

				sztext += "time zone : ";
				sztext += stTime.strTimeZone;
				sztext += "\r\n";
			}
			sztext += "\r\n";



			sztext += "* Custom Info ------------------\r\n";
			ENUM_SPIDER_CUSTOM_ASPECTRATIO eAspectRatio;
			lret = SPIDER_GetCustomAspectRatio(hCamera, &eAspectRatio);
			if(lret != E_CTRL_OK)
			{
				sztext += "aspect ratio : ";
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				sztext += "aspect ratio : ";
				switch(eAspectRatio)
				{
				case E_ASPECT_RAIO_CROPPING: sztext += "cropping"; break;
				case E_ASPECT_RAIO_SQUEEZING: sztext += "squeezing"; break;
				default: sztext += "unknown"; break;
				}
				sztext += "\r\n";
			}
			sztext += "\r\n";


			ENUM_SPIDER_CUSTOM_LINKSPEED eLinkSpeed;
			lret = SPIDER_GetCustomLinkSpeed(hCamera, &eLinkSpeed);
			if(lret != E_CTRL_OK)
			{
				sztext += "link speed : ";
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				sztext += "link speed : ";
				switch(eLinkSpeed)
				{
				case E_LINK_SPEED_AUTO: sztext += "auto Nego"; break;
				case E_LINK_SPEED_FULL100: sztext += "100M full"; break;
				case E_LINK_SPEED_HALF100: sztext += "100M half"; break;
				case E_LINK_SPEED_FULL10: sztext += "10M full"; break;
				case E_LINK_SPEED_HALF10: sztext += "10M half"; break;
				default: sztext += "unknown"; break;
				}
				sztext += "\r\n";
			}
			sztext += "\r\n";

			bool bEnable;
			lret = SPIDER_GetCustomSuppressColorNightMode(hCamera, &bEnable);
			if(lret != E_CTRL_OK)
			{
				sztext += "suppress color night mode : ";
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				sztext += "suppress color night mode : ";
				sztext += bEnable?"enable":"disable";
				sztext += "\r\n";
			}
			sztext += "\r\n";


			sztext += "* Custom Trigger Info ------------------\r\n";
			ST_SPIDER_CUSTOM_TRIGGER_INFO stCustomTrigger = {0};
			lret = SPIDER_GetCustomTriggerInfo(hCamera, &stCustomTrigger);
			if(lret != E_CTRL_OK)
			{
				sztext += GetErrorString(lret);
				sztext += "\r\n";
			}
			else
			{
				sztext += "Source : ";
				if(stCustomTrigger.nsSource == 0)
					sztext += "software";
				else if(stCustomTrigger.nsSource == 1)
					sztext += "hardware";
				else if(stCustomTrigger.nsSource == 2)
					sztext += "periodic";
				else if(stCustomTrigger.nsSource == 3)
					sztext += "filterCtrl";
				else
					sztext += "NA";
				sztext += "\r\n";


				szInfo.Format(("%d"), stCustomTrigger.nDelay);
				sztext += "Delay : ";
				sztext += szInfo; 
				sztext += "\r\n";

				sztext += "Polarity : ";
				if(stCustomTrigger.nsPolarity == 0)
					sztext += "high";
				else if(stCustomTrigger.nsPolarity == 1)
					sztext += "low";
				else 
					sztext += "NA";
				sztext += "\r\n";

				szInfo.Format(("%d"), stCustomTrigger.nsTriggerNoiseFilter);
				sztext += "Noise Filter : ";
				sztext += szInfo; 
				sztext += "\r\n";

				sztext += "Server On/Off : ";
				if(stCustomTrigger.nsOnOff == 0)
					sztext += "Off";
				else if(stCustomTrigger.nsOnOff == 1)
					sztext += "On";
				else
					sztext += "NA";
				sztext += "\r\n";

				sztext += "Server Addr : ";
				sztext += stCustomTrigger.strIpAddr;
				sztext += "\r\n";

				szInfo.Format(("%d"), stCustomTrigger.nsPortNo);
				sztext += "Server Port : ";
				sztext += szInfo; 
				sztext += "\r\n";

				//szInfo.Format(("%d"), stCustomTrigger.nsParameter);
				//sztext += "Parameter Number : ";
				//sztext += szInfo; 
				//sztext += "\r\n";

				//compare trigger server ip with my computer ip

				if(stCustomTrigger.strIpAddr)
				{
					CString szIPList;
					static CString szMyComIPList;
					static char m_vHostIP[10][50] = {0};
					static int nMyComIPCount = 0;
					if(szMyComIPList.GetLength() == 0)
					{
#pragma comment(lib, "IPHlpAPI.lib")
						//get ip list of this computer.
						PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
						ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
						if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
						{
							free(pAdapterInfo);
							pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
						}			

						int nIndex = 0;
						DWORD dwRetVal = 0;
						PIP_ADAPTER_INFO pAdapter = NULL;
						if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
						{				
							pAdapter = pAdapterInfo;
							while (pAdapter) 
							{		
								CString szip;
								szip.Format(("[%d] %s\r\n"), nIndex++, pAdapter->IpAddressList.IpAddress.String);
								szIPList += szip;
								strcpy(m_vHostIP[nMyComIPCount], pAdapter->IpAddressList.IpAddress.String);
								nMyComIPCount++;

								pAdapter = pAdapter->Next;
							}
						}

						free(pAdapterInfo);	

						if(szIPList.GetLength())
						{
							szMyComIPList = "My Computer IP Address is...\r\n\r\n";
							szMyComIPList += szIPList;
						}
						else
						{
							szMyComIPList = "Failed to get IP Address on this system.\r\n\r\n";
						}
					}

					int nFindIP = 0;
					while(nFindIP < nMyComIPCount)
					{
						if(strcmp(stCustomTrigger.strIpAddr, m_vHostIP[nFindIP]) == 0)
						{
							break;
						}

						nFindIP++;
					}

					if(nFindIP < nMyComIPCount)
					{
						if(m_hTrigger == NULL)
						{
							m_hTrigger = SPIDER_StartTriggerServer(GetSafeHwnd(), stCustomTrigger.nsPortNo);
						}
					}
				}
			}
			sztext += "\r\n";

			

			ST_SPIDER_CUSTOM_TRIGGER_PARAM_INFO stParam = {0};
			nCount = SPIDER_GetCustomTriggerParamCount(hCamera);

			szInfo.Format(("Param Count : %d\r\n"), nCount);
			sztext += szInfo;
			for(int i = 0; i < nCount; i++)
			{
				szInfo.Format(("Param [%d] "), i+1);
				sztext += szInfo;

				lret = SPIDER_GetCustomTriggerParamInfo(hCamera, i, &stParam);
				if(lret != E_CTRL_OK)
				{
					sztext += GetErrorString(lret);
					sztext += "\r\n";
				}
				else
				{
					szInfo.Format(("Gain : %d"), stParam.nGain);
					sztext += szInfo; 
					sztext += "\r\n";

					szInfo.Format(("Shutter : %d"), stParam.nShutter);
					sztext += szInfo; 
					sztext += "\r\n";

					szInfo.Format(("Increment : %d"), stParam.nsIncrement);
					sztext += szInfo; 
					sztext += "\r\n";
				}
				sztext += "\r\n";
			}
			sztext += "\r\n";

			
			GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(sztext);
		}
		else
		{
			GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(("Failed to create camera handle"));
		}
	
	*/
	*pResult = 0;
}

void CFireGuardCameraDlg::OnBnClickedButtonDevice()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CDeviceInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonFeature()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CFeatureInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonOverlayText()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	COverlayTextInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonMotion()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CMotionInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonPrivacyMask()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CPrivacyMaskInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonStream()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CStreamInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonTime()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CTimeInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonUser()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CUserInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonNetworkIpaddress()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CNetworkIPAddrInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonNetworkIpFilter()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CNetworkIPFilterInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonNotifyMailing()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CNotifyMailingInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonEventTrigger()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CEventTriggerInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonEventSchedule()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CEventScheduleInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonCustomTrigger()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CCustomTriggerInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonCustomStrobe()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_hSelectCamera == NULL)
		return;

	CCustomStrobeInfoDlg dlg;
	dlg.DoModal();
}

void CFireGuardCameraDlg::OnBnClickedButtonStorage()
{
	CString sztext;
	long lcount = SPIDER_GetStorageVolumeCount(theApp.m_hSelectCamera);

	sztext.Format(("Storage Volume Count : %d\r\n"), lcount);

	if(lcount == 0)
	{
		GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(sztext);
		return;
	}

	ST_SPIDER_STORAGE_VOLUME_INFO stInfo;
	CString szVolume;
	long lret = 0;
	for(int nStorage = 0; nStorage < lcount; nStorage++)
	{
		lret = SPIDER_GetStorageVolumeInfo(theApp.m_hSelectCamera, nStorage, &stInfo);
		if(lret != E_CTRL_OK)
		{
			szVolume.Format(("[%d] Failed to get info\r\n"), nStorage+1);
			sztext += szVolume;
			//MessageBox(GetErrorString(lret), ("Fail"));
		}
		else
		{
			CString szName, szType, szDesc;
			szName = stInfo.strVolumeName;
			szType = stInfo.strStorageType;
			szDesc = stInfo.strStorageDesc;
			szVolume.Format(("[%d] Name : %s, Type : %s, Desc : %s\r\n"), nStorage+1, szName, szType, szDesc);
			sztext += szVolume;
		}
	}

	GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(sztext);
}

void CFireGuardCameraDlg::OnBnClickedButtonCustomStorage()
{
	CString sztext;

	ST_SPIDER_CUSTOM_STORAGE_VOLUME stInfo;
	CString szVolume;
	long lret = 0;
	lret = SPIDER_GetCustomStorageVolume(theApp.m_hSelectCamera, &stInfo);
	if(lret != E_CTRL_OK)
	{
		szVolume.Format(("Failed to get info\r\n"));
		sztext += szVolume;
		//MessageBox(GetErrorString(lret), ("Fail"));
	}
	else
	{
		CString szStatus;
		switch(stInfo.nsStatus)
		{
		case E_STORAGE_STATUS_OK: szStatus = "SD_OK"; break;
		case E_STORAGE_STATUS_DETACH: szStatus = "detached"; break;
		case E_STORAGE_STATUS_FULL: szStatus = "storageFull"; break;
		case E_STORAGE_STATUS_PROTECT: szStatus = "writeProtected"; break;
		case E_STORAGE_STATUS_UNMOUNT: szStatus = "unMounted"; break;
		}
		szVolume.Format(("Status : %s\r\nCapacity %.2f MB, Free %.2f MB, Used %.2f MB (%.2f%%)\r\n"), szStatus, stInfo.fCapacity, stInfo.fFreeSize, stInfo.fUsedSize, stInfo.fUsedPercentage);
		sztext += szVolume;
	}


	sztext += "\r\n";


	ST_SPIDER_CUSTOM_STORAGE_CTRL stCtrl;
	lret = SPIDER_GetCustomStorageCtrl(theApp.m_hSelectCamera, &stCtrl);
	if(lret != E_CTRL_OK)
	{
		szVolume.Format(("Failed to get info\r\n"));
		sztext += szVolume;
		//MessageBox(GetErrorString(lret), ("Fail"));
	}
	else
	{
		CString szPrefix;
		szPrefix = stCtrl.strPrefix;
		szVolume.Format(("Prefix : %s\r\nCyclic : %s\r\nRecording : %s\r\nAudio Recording : %s\r\nSave Time (5 ~ 300) : %d (seconds)\r\n"),
			szPrefix, 
			stCtrl.bCyclicEnabled? ("Enable") : ("Disable"), 
			stCtrl.bRecord? ("On") : ("Off"),  
			stCtrl.bAudioEnabled? ("On") : ("Off"), 
			stCtrl.nSaveTime);
		sztext += szVolume;
	}

	GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText(sztext);
}

void CFireGuardCameraDlg::OnBnClickedButtonCustomCtrl()
{
	if(theApp.m_hSelectCamera == NULL)
	{
		MessageBox(("Select camera to setup custom ctrl info."));
		return;
	}

	CCustomCtrlDlg dlg;
	dlg.DoModal();
}


LRESULT CFireGuardCameraDlg::OnTriggerData(WPARAM wParam, LPARAM lParam )
{
	char szCameraIP[30] = "";
	char szFileName[MAX_PATH] = "";
	DWORD dwFileSize = 0;

	HTRIGGER hTrigger = (HTRIGGER)wParam;
	if(hTrigger)
	{
		switch(lParam)
		{
		case E_SERVER_RECVDATA:
			{
				SPIDER_GetTriggerImage(hTrigger, szCameraIP, szFileName, (unsigned int*)&dwFileSize, NULL);
				BYTE* pSaveFile = new BYTE[dwFileSize];
				if(pSaveFile)
				{
					SPIDER_GetTriggerImage(hTrigger, szCameraIP, szFileName, (unsigned int*)&dwFileSize, pSaveFile);
					TRACE("Save Trigger File %s %s\r\n", szCameraIP, szFileName);

					CFile file;
					CString cName;
					cName = gszCurDir;
					cName += szCameraIP;
					cName += "_";
					cName += szFileName;
					file.Open(cName, file.modeCreate|file.modeWrite);
					file.Write(pSaveFile, dwFileSize);
					file.Close();


					//if trigger data is yuv format, save as bitmap
					if(strstr(szFileName, ".yuv"))
					{
						char* pszPos = strstr(szFileName, "Cap");
						if(pszPos)
						{
							int width = 0;
							int height = 0;
							sscanf(pszPos, "Cap%dx%d", &width, &height);
							if(width && height)
							{
								DWORD dwRGBLen = width*height*3;
								BYTE* pRGB = new BYTE[dwRGBLen];
								if(pRGB)
								{
									if(E_CTRL_OK == SPIDER_ConvertYUV420ToRGB24(pSaveFile, dwFileSize, width, height, pRGB))
									{
										cName += ".bmp";

										BITMAPFILEHEADER	fHeader = {0};
										BITMAPINFOHEADER	InfoHeader = {0};

										fHeader.bfType = ((WORD)('M'<<8) | 'B');
										fHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
										fHeader.bfSize = dwRGBLen + fHeader.bfOffBits;

										InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
										InfoHeader.biWidth = width;
										InfoHeader.biHeight = height;
										InfoHeader.biPlanes = 1;
										InfoHeader.biBitCount = 24;
										InfoHeader.biCompression = BI_RGB;
										InfoHeader.biSizeImage = dwRGBLen;

										file.Open(cName, file.modeCreate|file.modeWrite);
										file.Write(&fHeader, sizeof(BITMAPFILEHEADER));
										file.Write(&InfoHeader, sizeof(BITMAPINFOHEADER));
										file.Write(pRGB, dwRGBLen);

										file.Close();
									}
									delete [] pRGB;
								}
							}
						}
					}


					delete [] pSaveFile;
				}
			}
			break;
		case E_SERVER_CLOSED:
		case E_SERVER_FAIL_CREATE_SOCKET:
		case E_SERVER_FAIL_OPEN_PORT:
		case E_SERVER_FAIL_START:
			{
				int nPort = 0;
				SPIDER_GetTriggerPortNumber(hTrigger, &nPort);
				TRACE("close trigger server %d\r\n", nPort);

				SPIDER_StopTriggerServer(hTrigger);

				if(hTrigger == m_hTrigger)
				{					
					m_hTrigger = NULL;
				}
			}
			break;
		}
	}
	return 0;
}

LRESULT CFireGuardCameraDlg::OnSyslogData(WPARAM wParam, LPARAM lParam )
{
	char szCameraIP[30] = "";
	ENUM_SYSLOG_FACILITY fac;
	ENUM_SYSLOG_SEVERITY svr;
	DWORD msglen = 0;
	char* pszMsg = NULL;

	HSYSLOG hSyslog = (HSYSLOG)wParam;
	if(hSyslog)
	{
	switch(lParam)
	{
	case E_SERVER_RECVDATA:
		{
			SPIDER_GetSyslogData(hSyslog, szCameraIP, &fac, &svr, (unsigned int*)&msglen, &pszMsg);	//pszMsg is valid in OnSyslogData() only.
			TRACE("syslog : %s %s\r\n", szCameraIP, pszMsg);
		}
		break;
	case E_SERVER_CLOSED:
	case E_SERVER_FAIL_CREATE_SOCKET:
	case E_SERVER_FAIL_OPEN_PORT:
	case E_SERVER_FAIL_START:
		{
			int nPort = 0;
			SPIDER_GetSyslogPortNumber(hSyslog, &nPort);
			TRACE("close syslog server %d\r\n", nPort);

			SPIDER_StopSyslogServer(hSyslog);

			if(hSyslog == m_hSyslog)
			{					
				m_hSyslog = NULL;
			}
		}
		break;
	}
	}
	return 0;
}

void CFireGuardCameraDlg::OnBnClickedButtonStreaming()
{
	if (m_selected == -1)
	{
		MessageBox(("선택된 카메라가 없습니다. 위 목록에서 카메라를 선택해주세요"));
		return;
	}

	if(m_StreamingDlg[m_selected].m_hStream)
	{
		//if (IDYES != MessageBox(("현재 영상화면을 닫으시겠습니까 ?"), ("Streaming"), MB_YESNO))
		//{
		//	return;
		//}

		m_StreamingDlg[m_selected].StopStreaming();

		if (m_StreamingDlg[m_selected].GetSafeHwnd()) {
			m_StreamingDlg[m_selected].EndDialog(IDOK);
		}

		GetDlgItem(IDC_BUTTON_STREAMING)->SetWindowText(("화면 시작/종료"));
		return;
	}


	if(theApp.m_hSelectCamera == NULL)
	{
		MessageBox(("Select camera to play stream"));
		return;
	}

	int nChannelIndex = 0;

	if (strlen(rtspurl[m_selected]) == 0)
	{
		SPIDER_GetStreamingURL(theApp.m_hSelectCamera, nChannelIndex, rtspurl[m_selected]);
	}
	if (strlen(rtspurl[m_selected]) == 0)
	{
		MessageBox(("Failed to get url address. 1"));
		return;
	}


	CString title; 
	//title.Format("카메라 %s", rtspurl[m_selected]);  //skpark
	title.Format("카메라 [%d]", m_selected);  //skpark

	TraceLog(("skpark : title %s", title));


	if (m_StreamingDlg[m_selected].GetSafeHwnd() == NULL) {
		m_StreamingDlg[m_selected].Create(CStreamingDlg::IDD, this);
		m_StreamingDlg[m_selected].SetWindowText(title);  //skpark
	}
	m_StreamingDlg[m_selected].ShowWindow(SW_SHOW);

	if (false == m_StreamingDlg[m_selected].StartStreaming(rtspurl[m_selected]))
	{
		m_StreamingDlg[m_selected].StopStreaming();
		m_StreamingDlg[m_selected].EndDialog(IDOK);
		MessageBox(("Failed to start streaming."));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_STREAMING)->SetWindowText(("화면 시작/종료"));
	}
}


void CFireGuardCameraDlg::OnBnClickedButtonReboot()
{
	if(theApp.m_hSelectCamera == NULL)
	{
		MessageBox(("Select camera to reboot."));
		return;
	}

	int nSel = m_List_Camera.GetSelectionMark();
	if(nSel != -1)
	{
		char szaddr[100] = ("");
		m_List_Camera.GetItemText(nSel, 2, szaddr, 50);

		CString msg;
		msg.Format(("Reboot Camera %s ?"), szaddr);
		if(IDYES != MessageBox(msg, ("Reboot"), MB_YESNO))
			return;

		long lret = SPIDER_Reboot(theApp.m_hSelectCamera);
		if(lret != E_CTRL_OK)
		{
			MessageBox(GetErrorString(lret), ("Failed to reboot."));
		}
		else
		{
			MessageBox(("Reboot success. \r\n\r\nCamera will be accessable after complete to reboot."));
		}
	}
}



void CFireGuardCameraDlg::OnBnClickedButtonExport()
{
	if(theApp.m_hSelectCamera == NULL)
	{
		MessageBox(("Select camera to export configuration."));
		return;
	}

	int nSel = m_List_Camera.GetSelectionMark();
	if(nSel != -1)
	{
		char szaddr[100] = ("");
		m_List_Camera.GetItemText(nSel, 2, szaddr, 50);

		CString msg;
		msg.Format(("Export Config of Camera %s ?"), szaddr);
		if(IDYES != MessageBox(msg, ("Export"), MB_YESNO))
			return;

		BYTE* pFileData = new BYTE[1024 * 30];
		if(pFileData == NULL)
			return;

		int nFileLen = 0;
		long lret = SPIDER_ExportConfig(theApp.m_hSelectCamera, &nFileLen, pFileData);
		if(lret != E_CTRL_OK)
		{
			MessageBox(GetErrorString(lret), ("Failed to export."));
		}
		else
		{
			//save file data... 
			CString filename;
			filename += szaddr;
			filename += "_cfg.dat";
			CFileDialog FileDlg(FALSE, ("*.*"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, ("*.*(*.*)|*.*||"));//OFN_HIDEREADONLY|
			if(IDOK == FileDlg.DoModal())
			{
				filename = FileDlg.GetPathName();

				CFile file;
				file.Open(filename, file.modeCreate|file.modeWrite);
				file.Write(pFileData, nFileLen);
				file.Close();

				msg = ("Success to export : ");
				msg += filename;
				MessageBox(msg);
			}
		}
		delete [] pFileData;
	}
}



void CFireGuardCameraDlg::OnBnClickedButtonImport()
{
	if(theApp.m_hSelectCamera == NULL)
	{
		MessageBox(("Select camera to be imported configuration."));
		return;
	}

	int nSel = m_List_Camera.GetSelectionMark();
	if(nSel != -1)
	{
		char szaddr[100] = ("");
		m_List_Camera.GetItemText(nSel, 2, szaddr, 50);



		CString filename;

		CFileDialog FileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, ("cfg file (*.*)|*.*||"));
		int nRet = FileDlg.DoModal();
		if(IDOK != nRet)
			return;
		
		filename = FileDlg.GetPathName();
		
		CFile file;
		if(false == file.Open(filename, file.modeRead))
		{
			CString msg;
			msg.Format(("The file is not exist. : %s"), filename);

			MessageBox(msg);
			return;
		}

		CString msg;
		msg.Format(("Import the cfg file below into camera %s?\r\n\r\n%s"), szaddr, filename);
		if(IDYES != MessageBox(msg, ("Import config"), MB_YESNO))
			return;

		int nFileLen = (int)file.GetLength();
		BYTE* pFileData = new BYTE[nFileLen];
		if(pFileData == NULL)
			return;

		nFileLen = file.Read(pFileData, nFileLen);
		file.Close();

		long lret = SPIDER_ImportConfig(theApp.m_hSelectCamera, nFileLen, pFileData);
		if(lret != E_CTRL_OK)
		{
			MessageBox(GetErrorString(lret), ("Failed to import."));
		}
		else
		{
			MessageBox(("Success to import."));
		}

		delete [] pFileData;
	}
}

void CFireGuardCameraDlg::OnBnClickedButtonSnapshot()
{
	if(theApp.m_hSelectCamera == NULL)
	{
		MessageBox(("Select camera."));
		return;
	}

	int nSel = m_List_Camera.GetSelectionMark();
	if(nSel != -1)
	{
		char szaddr[100] = ("");
		m_List_Camera.GetItemText(nSel, 2, szaddr, 50);

		int nLen = 0;
		BYTE* pFileData = new BYTE[3000*3000*4];
		long lret = SPIDER_GetSnapShotImage(theApp.m_hSelectCamera, &nLen, pFileData);
		if(lret != E_CTRL_OK)
		{
			delete [] pFileData;
			MessageBox(GetErrorString(lret), ("Fail"));
			return;
		}

		CFile file;
		CString cName;
		cName = UBC_SCREENSHOT_PATH;
		cName += szaddr;
		cName += "_";
		cName += "Snapshot.jpg";
		file.Open(cName, file.modeCreate|file.modeWrite);
		file.Write(pFileData, nLen);
		file.Close();

		delete [] pFileData;


		CString sztext;
		sztext = "SnapShot 이 " + cName + " 에 저장되었습니다.!\r\n 저장된 폴더를 여시겠습니까 ?";
		
		//MessageBox(sztext);
		if (IDYES == MessageBoxA(sztext, ("SnapShot"), MB_YESNO)) {
			CString filter = "ScreenShot Files (*.jpg)|*.jpg||";
			CFileDialog dlg(TRUE, NULL, UBC_SCREENSHOT_PATH, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, this);
			if (dlg.DoModal() != IDOK)
			{
				return;
			}
		}
	}
}

void CFireGuardCameraDlg::OnBnClickedButtonSerialport()
{
	if(theApp.m_hSelectCamera == NULL)
	{
		MessageBox(("Select camera to connect serial port."));
		return;
	}

	CSerialPortInfoDlg dlg;
	dlg.DoModal();
}


void CFireGuardCameraDlg::OnBnClickedButtonChannelSnapshot()
{
	if (theApp.m_hSelectCamera == NULL) {
		MessageBox(("Select camera."));
		return;
	}

	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel != -1) {
		char szaddr[100] = ("");
		m_List_Camera.GetItemText(nSel, 2, szaddr, 50);

		int nItem = m_cbChSnapShot.GetCurSel();
		if (nItem != CB_ERR) {
			int nLen = 0;
			BYTE* pFileData = new BYTE[3000*3000*4];
			long lret = SPIDER_GetChannelSnapShotImage(theApp.m_hSelectCamera, &nLen, pFileData, nItem+1);
			if (lret != E_CTRL_OK) {
				delete [] pFileData;
				MessageBox(GetErrorString(lret), ("Fail"));
				return;
			}

			CFile file;
			CString cName;
			cName = gszCurDir;
			cName += szaddr;
			cName += "_";
			cName += "Snapshot.jpg";
			file.Open(cName, file.modeCreate|file.modeWrite);
			file.Write(pFileData, nLen);
			file.Close();

			delete [] pFileData;


			CString sztext;
			sztext = "SnapShot saved!!\r\n";
			sztext += cName;
			MessageBox(sztext);
		}
	}
}


void CFireGuardCameraDlg::OnBnClickedButtonStreamRoiInfo()
{
	if (theApp.m_hSelectCamera == NULL) {
		MessageBox(("Select camera."));
		return;
	}
	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel != -1) {
		CStreamROIInfoDlg Dlg;
		Dlg.DoModal();
	}
}


void CFireGuardCameraDlg::OnBnClickedButtonThermalSpot()
{
	if (theApp.m_hSelectCamera == NULL) {
		MessageBox(("Select camera."));
		return;
	}
	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel != -1) {
//		CThermalSpotInfoDlg Dlg; // for IMT-812N
		CThermalCtrlInfoDlg Dlg; // for IMT-813N
		Dlg.DoModal();
	}
}


void CFireGuardCameraDlg::OnBnClickedButtonDigitalIo()
{
	//skpark in your area 기존소스부분이다. [
	//if (theApp.m_hSelectCamera == NULL) {
	//	MessageBox(("Select camera."));
	//	return;
	//}
	//int nSel = m_List_Camera.GetSelectionMark();
	//if (nSel != -1) {
	//	CDigitalIOInfoDlg Dlg;
	//	Dlg.DoModal();
	//}
	//]  기존소스를 막고 연동할 서버를 등록하는 화면으로 바꾼다.

	CServerRegiDlg Dlg;
	Dlg.DoModal();


}


void CFireGuardCameraDlg::OnBnClickedButtonDdns()
{
	if (theApp.m_hSelectCamera == NULL) {
		MessageBox(("Select camera."));
		return;
	}
	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel != -1) {
		CDynamicDNSInfoDlg Dlg;
		Dlg.DoModal();
	}
}


void CFireGuardCameraDlg::OnBnClickedButtonSnmp()
{
	if (theApp.m_hSelectCamera == NULL) {
		MessageBox(("Select camera."));
		return;
	}
	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel != -1) {
		CSNMPInfoDlg Dlg;
		Dlg.DoModal();
	}
}


void CFireGuardCameraDlg::OnBnClickedButtonPppoe()
{
	if (theApp.m_hSelectCamera == NULL) {
		MessageBox(("Select camera."));
		return;
	}
	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel != -1) {
		CPPPoEInfoDlg Dlg;
		Dlg.DoModal();
	}
}

void CFireGuardCameraDlg::OnBnClickedButtonOpenWeb()
{
	if (m_isHikvision) {
		if (m_selected < 0) {
			MessageBox(("Select camera."));
			return;
		}
	}
	else {
		if (theApp.m_hSelectCamera == NULL) {
			MessageBox(("Select camera."));
			return;
		}
	}
	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel < 0) { return; }

	char szaddr[100] = ("");
	m_List_Camera.GetItemText(nSel, 2, szaddr, 50);
	
	
	CString url = "http://";
	url += szaddr;
	OpenIExplorer(url);

	
}

void CFireGuardCameraDlg::OnBnClickedButtonIpManager()
{
	HINSTANCE nRet = ShellExecuteA(NULL, NULL, "IPManager_v3.2.3.0.exe", "", gszCurDir, SW_SHOWNORMAL);
	if (int(nRet) < 32)
	{
		TraceLog(("ShellExecute Error=%d", nRet));
	}
}
void CFireGuardCameraDlg::OnBnClickedButtonLogClear()
{
	m_msgText = "";
	GetDlgItem(IDC_EDIT_CAMERA_INFO)->SetWindowText("");
}

//AfxBeginThread(CFireGuardCameraDlg::ProcessPopup, this);
//UINT  CFireGuardCameraDlg::ProcessPopup(LPVOID pParam)
//{
//	CFireGuardCameraDlg* dlg = (CFireGuardCameraDlg*)pParam;
//	while (1)
//	{
//		int cameraId = FireProcess::getInstance()->popPopup();
//		if (cameraId == -1) {
//			Sleep(1000);
//		}
//		else {
//			dlg->ShowScreen(cameraId);
//		}
//	}
//	return 0;
//}

BOOL CFireGuardCameraDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	switch (pCopyDataStruct->dwData)
	{
	case WM_TEMPERATURE_ALARM:
		// response.Format("POPUP/%d/%d", command, cameraId, alarmType);
		CString response = (LPCTSTR)pCopyDataStruct->lpData;
		if (response.GetLength() > 8) {
			int pos = 0;
			CString command = response.Tokenize("/", pos);
			CString cameraId = response.Tokenize("/", pos);
			CString alarmType = response.Tokenize("/", pos);

			if (m_isHikvision) {
				// hikvision 을 위한 showScreen code 가 이곳에 필요함.
			} else {
				ShowScreen(atoi(cameraId), atoi(alarmType));
			}
		}
	
	}

	BOOL retval = CDialog::OnCopyData(pWnd, pCopyDataStruct);
	TraceLog(("OnCopyData() end"));
	return retval;
}

void CFireGuardCameraDlg::ShowScreen(int cameraId, int alarmType)
{
	TraceLog(("ShowScreen(%d, %d)", cameraId, alarmType));

	/*if (theApp.m_hSelectCamera) 
		SPIDER_DestroyCameraHandle(theApp.m_hSelectCamera);
	theApp.m_hSelectCamera = NULL;*/
	TraceLog(("ShowScreen1(%d)", cameraId));
	
	CString title;
	if (alarmType == 0) {
		title.Format("카메라 [%d]", cameraId);  //skpark
	}
	else if (alarmType == 1) {
		CTime now = CTime::GetCurrentTime();
		title.Format("카메라 [%d] 온도 이상 알람 발생 :  %04d/%02d/%02d %02d:%02d:%02d",
			cameraId, now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond());  //skpark
	}
	else  if (alarmType == 2) {
		CTime now = CTime::GetCurrentTime();
		title.Format("카메라 [%d] 온도 상승 추세 발생 :  %04d/%02d/%02d %02d:%02d:%02d",
			cameraId, now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond());  //skpark
	}

	TraceLog(("show4 skpark : title %s", title));
	
	bool changed = false;
	if (m_selected < 0 || cameraId != m_selected) {
		theApp.m_hSelectCamera = theApp.m_allCamera[cameraId];
		changed = true;
		m_selected = cameraId;
	}

	if (changed == false && (m_StreamingDlg[cameraId].m_hStream || m_StreamingDlg[cameraId].GetSafeHwnd() == NULL)) {
		m_StreamingDlg[cameraId].ShowWindow(SW_SHOW);
		m_StreamingDlg[cameraId].SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		if (alarmType == 0) {
			m_StreamingDlg[cameraId].SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOMOVE);
		}
		else {
			m_StreamingDlg[cameraId].SetWindowPos(NULL, 0, 0, 960, 720, SWP_NOMOVE);
		}
		m_StreamingDlg[cameraId].SetWindowText(title);  //skpark
		return;
	}
	TraceLog(("ShowScreen2(%d)", cameraId));

	if (m_StreamingDlg[cameraId].m_hStream)
	{
		m_StreamingDlg[cameraId].StopStreaming();
		if (m_StreamingDlg[cameraId].GetSafeHwnd()) {
			m_StreamingDlg[cameraId].EndDialog(IDOK);
		}
	}
	TraceLog(("ShowScreen3(%d)", cameraId));

	int nChannelIndex = 0;

	if (strlen(rtspurl[cameraId]) == 0)
	{
		SPIDER_GetStreamingURL(theApp.m_hSelectCamera, nChannelIndex, rtspurl[cameraId]);
		TraceLog(("ShowScreen4(%d)", cameraId));
	}
	if (strlen(rtspurl[cameraId]) == 0)
	{
		MessageBox(("Failed to get url address. 2"));
		return;
	}



	if (m_StreamingDlg[cameraId].GetSafeHwnd() == NULL) {
		m_StreamingDlg[cameraId].Create(CStreamingDlg::IDD, NULL);
		m_StreamingDlg[cameraId].SetWindowText(title);  //skpark
	} 
	m_StreamingDlg[cameraId].ShowWindow(SW_SHOW);
	m_StreamingDlg[cameraId].SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	if (alarmType == 0) {
		m_StreamingDlg[cameraId].SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOMOVE);
	}
	else {
		m_StreamingDlg[cameraId].SetWindowPos(NULL, 0, 0, 960, 720, SWP_NOMOVE);
	}
	if (false == m_StreamingDlg[cameraId].StartStreaming(rtspurl[cameraId]))
	{
		m_StreamingDlg[cameraId].StopStreaming();
		m_StreamingDlg[cameraId].EndDialog(IDOK);

		MessageBox(("Failed to start streaming."));
	}
	TraceLog(("ShowScreen5(%d)", cameraId));

}

void CFireGuardCameraDlg::OnBnClickedButtonAddCamera()
{
	/*
	struct ST_SPIDER_DISCOVERY_CAMEAR_INFO
	{
		CHAR			strDeviceName[64];
		CHAR			strModelName[64];
		CHAR			strSerialNum[64];
		CHAR			strMacAddr[64];
		CHAR			strVendorName[64];
		CHAR			strFirmware[64];
		CHAR			strDeviceURL[64];
		__time32_t		DeviceStartTime;
		CHAR			strDeviceDes[64];
		USHORT			nsDiscoverySupport; // bitmask ; upnp 0x0001, zeroconf 0x0002, onvif 0x0004
		CHAR			strIpAddress[64];
		CHAR			strSubnetMask[64];
		CHAR			strGateway[64];
	};
	*/

	if (m_foundedCount >= MAX_CAMERA) {
		AfxMessageBox("최대 등록 카메라 수를 초과했습니다");
		return;
	}

	CThermalDetailDlg aDlg;
	aDlg.SetIdMap(&m_idIpMap);
	if (aDlg.DoModal() == IDOK)
	{
		CString ip = aDlg.m_ipAddress;
		CString id = aDlg.m_cameraId;
		CString pwd = aDlg.m_pwd;

		InsertCameraInfo(id, ip, pwd);
	}
}

void CFireGuardCameraDlg::InsertCameraInfo(const char* id, const char* ip, const char* pwd, bool save /* = true*/)
{
	int i = m_foundedCount;

	strcpy(m_stInfo[i].strDeviceName, id);
	strcpy(m_stInfo[i].strDeviceDes, pwd);
	strcpy(m_stInfo[i].strIpAddress, ip);
	strcpy(m_stInfo[i].strModelName, "DS-2TD2617-6P/A");
	strcpy(m_stInfo[i].strVendorName, "Hikvision");
	CString url = "http://";
	url += ip;
	strcpy(m_stInfo[i].strDeviceURL, url);

	m_List_Camera.InsertItem(i, id);

	int ncol = 1;
	CString sztext = m_stInfo[i].strVendorName;
	m_List_Camera.SetItemText(i, ncol++, sztext);

	sztext = m_stInfo[i].strIpAddress;
	m_List_Camera.SetItemText(i, ncol++, sztext);

	sztext = m_stInfo[i].strModelName;
	m_List_Camera.SetItemText(i, ncol++, sztext);

	sztext = m_stInfo[i].strMacAddr;
	m_List_Camera.SetItemText(i, ncol++, sztext);

	sztext = "";
	sztext = m_stInfo[i].strDeviceDes != NULL ? m_stInfo[i].strDeviceDes : "not specified";
	m_List_Camera.SetItemText(i, ncol++, sztext);


	m_foundedCount++;

	TraceLog(("OnBnClickedButtonAddCamera()\n"));

	this->m_idIpMap[id] = ip;
	m_idList.push_back(id);
	if (save) {
		SaveCameraInfo(id, ip, pwd);
	}
}

void CFireGuardCameraDlg::SaveCameraInfo(const char* id, const char* ip, const char* pwd)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString newList;
	std::list<CString>::iterator itr;
	for (itr = m_idList.begin(); itr != m_idList.end(); itr++){
		if (!newList.IsEmpty()){
			newList += ",";
		}
		newList += *itr;
	}
	WritePrivateProfileString("FIRE_WATCH_CAMERA", "IdList", newList, iniPath);

	CString entry;
	entry.Format("FIRE_WATCH_CAMERA_%s",id);

	WritePrivateProfileString(entry, "ID", id, iniPath);
	WritePrivateProfileString(entry, "IP", ip, iniPath);
	WritePrivateProfileString(entry, "PWD", pwd, iniPath);

	AddCenterToStarter(id,ip,pwd);

}

void CFireGuardCameraDlg::AddCenterToStarter(const char* id, const char* ip, const char* pwd)
{
	HWND sttHwnd = ::FindWindow(NULL, "Guardian Starter");
	if (!sttHwnd) {
		// Starter ini 파일에 직접 Write 해야 한다.		
		return ;
	}

	/*
	app_info.appId = token.nextToken().c_str();
					app_info.argument = token.nextToken().c_str();
					CString strBool = token.nextToken().c_str();
					app_info.autoStartUp = (strBool == "TRUE") ? 1:0;
					app_info.binaryName = token.nextToken().c_str();
					app_info.debug = token.nextToken().c_str();
					app_info.initSleepSecond = atoi(token.nextToken().c_str());
					strBool = token.nextToken().c_str();
					app_info.isPrimitive = (strBool == "TRUE") ? 1:0;
					strBool = token.nextToken().c_str();
					app_info.monitored = (strBool == "TRUE") ? 1:0;
					app_info.pid = atoi(token.nextToken().c_str());
					app_info.preRunningApp = token.nextToken().c_str();
					app_info.properties = token.nextToken().c_str();
					app_info.runningType = token.nextToken().c_str();
					strBool = token.nextToken().c_str();
					app_info.runTimeUpdate = (strBool == "TRUE") ? 1:0;
					app_info.startTime = token.nextToken().c_str();
					app_info.status = token.nextToken().c_str();
					app_info.adminState = true; // ON
	*/
	CString msgData = "ADD&";

	// appId
	msgData += "CENTER";
	msgData += id;
	msgData += "&";
	// argument
	msgData += " +id ";
	msgData += id;
	msgData += " +ip ";
	msgData += ip;
	msgData += " +pwd ";
	msgData += pwd;
	msgData += " +fire&";
	//
	msgData += "TRUE&";// autoStartup
	msgData += "GuardianCenter.exe&"; // binaryName
	msgData += "&";  // debug;
	msgData += "5&"; // initSleepSecond
	msgData += "TRUE&";// isPrimitive 
	msgData += "TRUE&";// monitored
	msgData += "0&";  // pid;
	msgData += "&";  // preRunningApp;
	msgData += "&";  // properties;
	msgData += "FG&";  // runningType;
	msgData += "FALSE&";// runTimeUpdate
	msgData += "&";  // startTime;
	msgData += "Inactive&";  // status;
	
	COPYDATASTRUCT appInfo;
	appInfo.dwData = 100;  // Add
	appInfo.lpData = (char*)msgData.GetBuffer();
	appInfo.cbData = msgData.GetLength() + 1;

	::SendMessage(sttHwnd, WM_COPYDATA, NULL, (LPARAM)&appInfo);

	TraceLog(("sendMessage to STT (ARG=%s)", msgData));
	::Sleep(1000);
	return;
}

void CFireGuardCameraDlg::GetCameraInfoFromIni()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	char buf[1024];
	memset(buf, 0x00, 1024);
	GetPrivateProfileStringA("FIRE_WATCH_CAMERA", "IdList", "", buf, 1024, iniPath);

	CString newList = buf;
	if (newList.IsEmpty()) {
		return;
	}
	m_idList.clear();

	int pos = 0;
	CString id = newList.Tokenize(",", pos);
	while (!id.IsEmpty())
	{
		CString entry;
		entry.Format("FIRE_WATCH_CAMERA_%s", id);

		
		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "IP", "", buf, 1024, iniPath);
		CString ip = buf;

		memset(buf, 0x00, 1024);
		GetPrivateProfileStringA(entry, "PWD", "", buf, 1024, iniPath);
		CString pwd = buf;

		InsertCameraInfo(id, ip, pwd, false);
		id = newList.Tokenize(",", pos);
	}
}


void CFireGuardCameraDlg::OnBnClickedButtonDelCamera()
{
	if (m_selected == -1)
	{
		MessageBox(("선택된 카메라가 없습니다. 위 목록에서 카메라를 선택해주세요"));
		return;
	}

	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel == -1) return;

	char szId[100] = "";
	m_List_Camera.GetItemText(nSel, 0, szId, 50);
	DelCamera(szId);
}

void CFireGuardCameraDlg::DelCamera(const char* szId)
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString newList;
	std::list<CString>::iterator itr;
	CString deleteId;
	for (itr = m_idList.begin(); itr != m_idList.end(); ){
		CString id = (*itr);
		if (id == szId) {
			TraceLog(("%d deleted", m_selected));
			m_idList.erase(itr++);
			deleteId = id;
			continue;
		}
		else{
			++itr;
		}
		if (!newList.IsEmpty()){
			newList += ",";
		}
		newList += id;
	}
	if (!deleteId.IsEmpty()) {
		WritePrivateProfileString("FIRE_WATCH_CAMERA", "IdList", newList, iniPath);
		m_idIpMap.erase(deleteId);
		DeleteCenterToStarter(deleteId);
		OnBnClickedButtonDiscovery();
	}
}


void CFireGuardCameraDlg::DeleteCenterToStarter(const char* id)
{
	HWND sttHwnd = ::FindWindow(NULL, "Guardian Starter");
	if (!sttHwnd) {
		// Starter ini 파일에 직접 Write 해야 한다.		
		return;
	}
	
	CString msgData = "DELETE&";

	// appId
	msgData += "CENTER";
	msgData += id;
	msgData += "&";
	
	COPYDATASTRUCT appInfo;
	appInfo.dwData = 100;  // DELETE
	appInfo.lpData = (char*)msgData.GetBuffer();
	appInfo.cbData = msgData.GetLength() + 1;

	::SendMessage(sttHwnd, WM_COPYDATA, NULL, (LPARAM)&appInfo);

	TraceLog(("sendMessage to STT (ARG=%s)", msgData));
	::Sleep(1000);
	return;
}


void CFireGuardCameraDlg::OnBnClickedButtonChangeIp()
{
	if (m_selected == -1)
	{
		MessageBox(("선택된 카메라가 없습니다. 위 목록에서 카메라를 선택해주세요"));
		return;
	}

	int nSel = m_List_Camera.GetSelectionMark();
	if (nSel == -1) return;

	char szId[100] = "";
	m_List_Camera.GetItemText(nSel, 0, szId, 50);
	char szIp[100] = "";
	m_List_Camera.GetItemText(nSel, 2, szIp, 50);

	CThermalDetailDlg aDlg(szId, szIp);
	aDlg.SetIdMap(&m_idIpMap);
	if (aDlg.DoModal() == IDOK)
	{
		CString ip = aDlg.m_ipAddress;
		CString id = aDlg.m_cameraId;
		CString pwd = aDlg.m_pwd;

		DelCamera(id);
		InsertCameraInfo(id, ip, pwd);
	}
}
