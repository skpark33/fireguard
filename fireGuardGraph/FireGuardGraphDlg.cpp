// fireGuardGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fireGuardGraph.h"
#include "fireGuardGraphDlg.h"
#include <math.h>
#include <vector>
#include <sstream>
#include "preSocketSession.h"
#include "skpark/TraceLog.h"
#include "resource.h"
#include "mmsystem.h"
#include "SelectCameraDlg.h"
#include "ci/libAes256/ciElCryptoAes256.h"
#include "skpark/util.h"
#include "FireProcess.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define		TEMPER_BG		"t_bg.png"
#define		VELO_BG		"v_bg.png"
#define		SLOPE_BG		"s_bg.png"


#define		TEMPER_NORMAL_BG		"t_normal.png"
#define		TEMPER_ALARM_BG		"t_alarm.png"
#define		VELO_NORMAL_BG		"v_normal.png"
#define		VELO_ALARM_BG		"v_alarm.png"
#define		SLOPE_NORMAL_BG		"s_normal.png"


/////////////////////////////////////////////////////////////////////////////
// CfireGuardGraphDlg dialog

static const int DataRateTimer = 1;
static const int ChartUpdateTimer = 2;
static const int DataInterval = 1000;
static const int FrequencyTimer = 3;

//
// Constructor
//
CfireGuardGraphDlg::CfireGuardGraphDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CfireGuardGraphDlg::IDD, pParent)
	, m_session(NULL)
	, m_font(0)
	, m_isStopAlarm(false)
	, m_isStopTrend(false)
	, m_max_temperature(-999.0f)
	, m_logDuration(365)
	, m_smsAllowed(false)
{
	m_AlarmLogger = new LogManager("Alarm");
	m_TrendLogger = new LogManager("Trend");

    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_updatePeriod = 1000;

	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	char serviceUrl[1024];
	memset(serviceUrl, 0x00, 1024);
	GetPrivateProfileString("FIRE_WATCH", "SMS_URL", 
	//	"http://qrcode-dev.ap-northeast-2.elasticbeanstalk.com/smssend", serviceUrl, 1023, iniPath);
	"http://qrcode.ap-northeast-2.elasticbeanstalk.com/smssend", serviceUrl, 1023, iniPath);
	m_smsURL = serviceUrl;

	for (int i = 0; i < MAX_CAMERA; i++)
	{
		userCheck[i] = false;
	}
}

//
// Destructor
//
CfireGuardGraphDlg::~CfireGuardGraphDlg()
{
	delete m_ChartViewer.getChart();
	delete m_VelocViewer.getChart();
	delete m_SlopeViewer.getChart();
}

void CfireGuardGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CfireGuardGraphDlg)
	//DDX_Control(pDX, IDC_GammaValue, m_ValueC);
	//DDX_Control(pDX, IDC_BetaValue, m_ValueB);
	//DDX_Control(pDX, IDC_AlphaValue, m_ValueA);
	DDX_Control(pDX, IDC_ChartViewer, m_ChartViewer);
	DDX_Control(pDX, IDC_VelocityViewer, m_VelocViewer);
	DDX_Control(pDX, IDC_SlopeViewer, m_SlopeViewer);
	DDX_Control(pDX, IDC_RunPB, m_RunPB);
	DDX_Control(pDX, IDC_UpdatePeriod, m_CBUpdatePeriod);
	DDX_Control(pDX, IDC_TEMPER_8, m_TemperVal[7]);
	DDX_Control(pDX, IDC_TEMPER_7, m_TemperVal[6]);
	DDX_Control(pDX, IDC_TEMPER_6, m_TemperVal[5]);
	DDX_Control(pDX, IDC_TEMPER_5, m_TemperVal[4]);
	DDX_Control(pDX, IDC_TEMPER_4, m_TemperVal[3]);
	DDX_Control(pDX, IDC_TEMPER_3, m_TemperVal[2]);
	DDX_Control(pDX, IDC_TEMPER_2, m_TemperVal[1]);
	DDX_Control(pDX, IDC_TEMPER_1, m_TemperVal[0]);
	DDX_Control(pDX, IDC_CHECK_1, m_checkCamera[0]);
	DDX_Control(pDX, IDC_CHECK_2, m_checkCamera[1]);
	DDX_Control(pDX, IDC_CHECK_3, m_checkCamera[2]);
	DDX_Control(pDX, IDC_CHECK_4, m_checkCamera[3]);
	DDX_Control(pDX, IDC_CHECK_5, m_checkCamera[4]);
	DDX_Control(pDX, IDC_CHECK_6, m_checkCamera[5]);
	DDX_Control(pDX, IDC_CHECK_7, m_checkCamera[6]);
	DDX_Control(pDX, IDC_CHECK_8, m_checkCamera[7]);
	DDX_Control(pDX, IDC_CHECK_ALL1, m_checkCameraAll);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX, m_editThresholdMax[0]);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX2, m_editThresholdMax[1]);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX3, m_editThresholdMax[2]);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX4, m_editThresholdMax[3]);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX5, m_editThresholdMax[4]);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX6, m_editThresholdMax[5]);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX7, m_editThresholdMax[6]);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_MAX8, m_editThresholdMax[7]);
	DDX_Control(pDX, IDC_EDIT_VELOC_MAX, m_editVelocMax);
	DDX_Control(pDX, IDC_ANIMATE1, m_aniCtrl);
	DDX_Control(pDX, IDC_VELOC_ANIMATE, m_aniVelocCtrl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_VELOC_RANGE, m_editFrequency);
	DDX_Control(pDX, IDC_CHECK_ALARM, m_checkStopAlarm);
	DDX_Control(pDX, IDC_CHECK_TREND, m_checkStopTrend);
	DDX_Control(pDX, IDC_BUTTON_SMS_MGR, m_btSMS);
	DDX_Control(pDX, IDC_CHECK_SEND_MSG, m_checkSMS);
	DDX_Control(pDX, IDC_STATIC_COUNTER, m_stCounter);
	//DDX_Control(pDX, IDC_EDIT_VELOC_DEVI, m_editDevi);
	DDX_Control(pDX, IDC_EDIT_LOG_DURATION, m_editLogDuration);
	DDX_Control(pDX, IDC_STATIC_VELO_SEC, m_stVeloSec);
	DDX_Control(pDX, IDC_BT_SHOW_1, m_btShow[0]);
	DDX_Control(pDX, IDC_BT_SHOW_2, m_btShow[1]);
	DDX_Control(pDX, IDC_BT_SHOW_3, m_btShow[2]);
	DDX_Control(pDX, IDC_BT_SHOW_4, m_btShow[3]);
	DDX_Control(pDX, IDC_BT_SHOW_5, m_btShow[4]);
	DDX_Control(pDX, IDC_BT_SHOW_6, m_btShow[5]);
	DDX_Control(pDX, IDC_BT_SHOW_7, m_btShow[6]);
	DDX_Control(pDX, IDC_BT_SHOW_8, m_btShow[7]);
	DDX_Control(pDX, IDC_CHECK_TH_1, m_checkTH[0]);
	DDX_Control(pDX, IDC_CHECK_TH_2, m_checkTH[1]);
	DDX_Control(pDX, IDC_CHECK_TH_3, m_checkTH[2]);
	DDX_Control(pDX, IDC_CHECK_TH_4, m_checkTH[3]);
	DDX_Control(pDX, IDC_CHECK_TH_5, m_checkTH[4]);
	DDX_Control(pDX, IDC_CHECK_TH_6, m_checkTH[5]);
	DDX_Control(pDX, IDC_CHECK_TH_7, m_checkTH[6]);
	DDX_Control(pDX, IDC_CHECK_TH_8, m_checkTH[7]);
}

BEGIN_MESSAGE_MAP(CfireGuardGraphDlg, CDialog)
    //{{AFX_MSG_MAP(CfireGuardGraphDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_RunPB, OnRunPB)
    ON_BN_CLICKED(IDC_FreezePB, OnFreezePB)
    ON_CBN_SELCHANGE(IDC_UpdatePeriod, OnSelchangeUpdatePeriod)
    ON_CONTROL(CVN_ViewPortChanged, IDC_ChartViewer, OnViewPortChanged)
    ON_CONTROL(CVN_MouseMovePlotArea, IDC_ChartViewer, OnMouseMovePlotArea)
	ON_CONTROL(CVN_ViewPortChanged, IDC_VelocityViewer, OnViewPortChangedVeloc)
	ON_CONTROL(CVN_ViewPortChanged, IDC_SlopeViewer, OnViewPortChangedSlope)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_VelocityViewer, OnMouseMovePlotAreaVeloc)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_SlopeViewer, OnMouseMovePlotAreaSlope)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_ALL1, &CfireGuardGraphDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_MAX_APPLY, &CfireGuardGraphDlg::OnBnClickedButtonMaxApply)
	ON_BN_CLICKED(IDC_SHOW_LOG, &CfireGuardGraphDlg::OnBnClickedShowLog)
	ON_BN_CLICKED(IDC_OPEN_FOLDER, &CfireGuardGraphDlg::OnBnClickedOpenFolder)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_VELOC_APPLY, &CfireGuardGraphDlg::OnBnClickedButtonVelocApply)
	//ON_EN_KILLFOCUS(IDC_EDIT_THRESHOLD_MAX, &CfireGuardGraphDlg::OnEnKillfocusEditThresholdMax)
	ON_BN_CLICKED(IDC_CHECK_ALARM, &CfireGuardGraphDlg::OnBnClickedCheckAlarm)
	ON_BN_CLICKED(IDC_CHECK_TREND, &CfireGuardGraphDlg::OnBnClickedCheckTrend)
	ON_BN_CLICKED(IDC_BUTTON_SMS_MGR, &CfireGuardGraphDlg::OnBnClickedButtonSmsMgr)
	ON_BN_CLICKED(IDC_CHECK_SEND_MSG, &CfireGuardGraphDlg::OnBnClickedCheckSendMsg)
	ON_BN_CLICKED(IDC_CHECK_1, &CfireGuardGraphDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK_2, &CfireGuardGraphDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK_3, &CfireGuardGraphDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK_4, &CfireGuardGraphDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK_5, &CfireGuardGraphDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK_6, &CfireGuardGraphDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK_7, &CfireGuardGraphDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK_8, &CfireGuardGraphDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_BUTTON_LOG_DURATION_APPLY, &CfireGuardGraphDlg::OnBnClickedButtonLogDurationApply)
	ON_BN_CLICKED(IDC_BT_SHOW_1, &CfireGuardGraphDlg::OnBnClickedBtShow1)
	ON_BN_CLICKED(IDC_BT_SHOW_2, &CfireGuardGraphDlg::OnBnClickedBtShow2)
	ON_BN_CLICKED(IDC_BT_SHOW_3, &CfireGuardGraphDlg::OnBnClickedBtShow3)
	ON_BN_CLICKED(IDC_BT_SHOW_4, &CfireGuardGraphDlg::OnBnClickedBtShow4)
	ON_BN_CLICKED(IDC_BT_SHOW_5, &CfireGuardGraphDlg::OnBnClickedBtShow5)
	ON_BN_CLICKED(IDC_BT_SHOW_6, &CfireGuardGraphDlg::OnBnClickedBtShow6)
	ON_BN_CLICKED(IDC_BT_SHOW_7, &CfireGuardGraphDlg::OnBnClickedBtShow7)
	ON_BN_CLICKED(IDC_BT_SHOW_8, &CfireGuardGraphDlg::OnBnClickedBtShow8)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfireGuardGraphDlg message handlers

//
// Initialization
//
BOOL CfireGuardGraphDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
	
	m_font = new CFont();
	m_font->CreatePointFont(10 * 8, "Tahoma");

	SendMessageToDescendants(WM_SETFONT, (WPARAM)m_font->GetSafeHandle(), 1, TRUE, FALSE);

	Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

    // *** code automatically generated by VC++ MFC AppWizard ***
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    //
    // Initialize member variables
    //
    m_extBgColor = getDefaultBgColor();     // Default background color

    // Clear data arrays to Chart::NoValue
	for (int i = 0; i < sampleSize; ++i)
	{
		m_timeStamps[i] = Chart::NoValue;
		for (int j = 0; j < MAX_CAMERA; j++)
		{
			m_dataSeries[j][i] = Chart::NoValue;
			m_rateSeries[j][i] = 0;
			m_slopeSeries[j][i] = 0;
			m_countSeries[j][i] = 0;
		}
	}

	for (int j = 0; j < MAX_CAMERA; j++)
	{
		m_checkCamera[j].SetCheck(true);
		m_checkTH[j].SetCheck(false);
		m_btShow[j].EnableWindow(false);

	}
	m_checkCameraAll.SetCheck(false);

	m_currentIndex = 0;

    // Set m_nextDataTime to the current time. It is used by the real time random number 
    // generator so it knows what timestamp should be used for the next data point.
    SYSTEMTIME st;
    GetLocalTime(&st);
    m_nextDataTime = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, 
        st.wSecond) + st.wMilliseconds / 1000.0;
	

	CopyFileA(TEMPER_NORMAL_BG, TEMPER_BG, FALSE);
	CopyFileA(VELO_NORMAL_BG, VELO_BG, FALSE);
    //
    // Initialize controls
    //

    // Set up the data acquisition mechanism. In this demo, we just use a timer to get a 
    // sample every 250ms.
    SetTimer(DataRateTimer, DataInterval, 0);

    // The chart update rate (in ms)
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;
	char buf[512];
	memset(buf, 0x00, 512);
	GetPrivateProfileStringA("FIRE_WATCH", "UPDATE_PERIOD", "1000", buf, 511, iniPath);
	m_updatePeriod = _tcstol(buf, 0, 0);
	m_CBUpdatePeriod.SelectString(0, buf);

	memset(buf, 0x00, 512);
	GetPrivateProfileString("FIRE_WATCH", "CHECK_DONT_SMS", "1", buf, 511, iniPath);
	m_checkSMS.SetCheck((atoi(buf)));

	memset(buf, 0x00, 512);
	GetPrivateProfileString("FIRE_WATCH", "CHECK_DONT_ALARM", "1", buf, 511, iniPath);
	m_isStopAlarm = bool(atoi(buf));
	m_checkStopAlarm.SetCheck(int(m_isStopAlarm));

	memset(buf, 0x00, 512);
	GetPrivateProfileString("FIRE_WATCH", "CHECK_DONT_TREND", "1", buf, 511, iniPath);
	m_isStopTrend = bool(atoi(buf));
	m_checkStopTrend.SetCheck(int(m_isStopTrend));

	memset(buf, 0x00, 512);
	GetPrivateProfileString("FIRE_WATCH", "LOG_DURATION", "365", buf, 511, iniPath);
	m_logDuration = atoi(buf);
	m_editLogDuration.SetWindowText(buf);

	// FirmwareView 가  sosId 에 전화번호 끝에 4자리를 적은후, SMS_ALLOWED 를 true 로 자동으로 바꿔주게 되어있다.
	memset(buf, 0x00, 512);
	GetPrivateProfileString("FIRE_WATCH", "SMS_ALLOWED", "0", buf, 511, iniPath);
	m_smsAllowed = bool(atoi(buf));
	if (!m_smsAllowed) m_checkSMS.SetCheck(true);
	m_checkSMS.EnableWindow(m_smsAllowed);


	// Load icons for the Run/Freeze buttons
    loadButtonIcon(IDC_RunPB, IDI_RunPB, 100, 20);
    loadButtonIcon(IDC_FreezePB, IDI_FreezePB, 100, 20);

	m_temperThreshold = new CMultiThresholdHandler(
		TEMPER_OVER,
		&m_ChartViewer, m_editThresholdMax, m_checkTH,
		&m_aniCtrl);

	m_temperThreshold->ReadConfig("500.0", "-20.0");
	m_temperThreshold->InitAnimation();

	m_velociThreshold = new CThresholdHandler(
		VELOCITY_OVER,
		&m_VelocViewer, &m_editVelocMax,
		&m_aniVelocCtrl, &m_editFrequency); // , &m_editDevi);

	CString min, max;
	min.Format("%.2f", MIN_VELO);
	max.Format("%.2f", MAX_VELO);
	m_velociThreshold->ReadConfig(max, min);

	TraceLog(("skpark ----------------------:%f, %d", m_velociThreshold->m_thresholdMax, int(m_velociThreshold->m_thresholdMax / 4.0)));
	CString veloStr;
	veloStr.Format("= %d 초", int(m_velociThreshold->m_thresholdMax / 4.0));
	m_stVeloSec.SetWindowTextA(veloStr);
	m_stVeloSec.ShowWindow(SW_HIDE);


	m_slopeThreshold = new CThresholdHandler(
		VELOCITY_OVER,
		&m_SlopeViewer, NULL,
		NULL /*, NULL*/);

	//m_slopeThreshold->ReadConfig("50.0", "-50.0");
	//m_slopeThreshold->InitAnimation();

	m_session = new preSocketSession(14152);
	if (m_session->init() == true)
	{
		m_session->start();
	}
	else
	{
		TraceLog(("preSocketSession socket init failed."));
	}
	LicenseUtil::LicenseCheck();

	::SetWindowPos(this->m_hWnd, HWND_TOP, 632, 10, -1, -1, SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// Initially set the Run mode
	m_RunPB.SetCheck(1);
	
	OnRunPB();
	
	//SetTimer(FrequencyTimer, DataInterval + 10, 0);
	
	m_ChartViewer.SetRange(MAX_THRESHOLD, MIN_THRESHOLD);
	m_VelocViewer.SetRange(MAX_VELO, MIN_VELO);
	m_SlopeViewer.SetRange(5.0f, -5.0f);

	LoadSendData();


	TraceLog(("OnInitDialog End"));

	return TRUE;
}



// *** code automatically generated by VC++ MFC AppWizard ***
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
void CfireGuardGraphDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// *** code automatically generated by VC++ MFC AppWizard ***
// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CfireGuardGraphDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

//
// User clicks on the Run pushbutton
//
void CfireGuardGraphDlg::OnRunPB() 
{
    // Enable chart update timer
   CString s;
    m_CBUpdatePeriod.GetLBText(m_CBUpdatePeriod.GetCurSel(), s);
	m_updatePeriod = _tcstol(s, 0, 0);

	SetTimer(ChartUpdateTimer, m_updatePeriod, 0);

	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString buf;
	buf.Format("%d", m_updatePeriod);
	WritePrivateProfileStringA("FIRE_WATCH", "UPDATE_PERIOD", buf, iniPath);
}

//
// User clicks on the Freeze pushbutton
//
void CfireGuardGraphDlg::OnFreezePB() 
{
    // Disable chart update timer
    KillTimer(ChartUpdateTimer);    
}

//
// Handles timer events
//
void CfireGuardGraphDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case DataRateTimer:
		// Is data acquisition timer - get a new data sample
		//getData();
		//TraceLog(("OnTimer 1()"));
		OnDataTimer();
		//TraceLog(("OnTimer 1()"));

		break;
	case ChartUpdateTimer:
		// Is chart update timer - request chart update
		//TraceLog(("OnTimer 2()"));
		m_ChartViewer.updateViewPort(true, false);
		m_VelocViewer.updateViewPort(true, false);
		m_SlopeViewer.updateViewPort(true, false);
		//TraceLog(("OnTimer 2()"));
		break;
	
	case FrequencyTimer:
		//TraceLog(("OnTimer 3()"));
		OnFrequencyTimer();
		//TraceLog(("OnTimer 3()"));
		break;
	}
    CDialog::OnTimer(nIDEvent);
}

//
// View port changed event
//
void CfireGuardGraphDlg::OnViewPortChanged()
{
	drawChart(&m_ChartViewer);
}
void CfireGuardGraphDlg::OnViewPortChangedVeloc()
{
	drawVeloc(&m_VelocViewer);
}
void CfireGuardGraphDlg::OnViewPortChangedSlope()
{
	drawSlope(&m_SlopeViewer);
}
//
// User changes the chart update period
//
void CfireGuardGraphDlg::OnSelchangeUpdatePeriod() 
{
    if (m_RunPB.GetCheck())
    {
        // Call freeze then run to use the new chart update period
        OnFreezePB();
        OnRunPB();
    }   
}

//
// Draw track cursor when mouse is moving over plotarea
//
void CfireGuardGraphDlg::OnMouseMovePlotArea()
{
    trackLineLegend((XYChart *)m_ChartViewer.getChart(), m_ChartViewer.getPlotAreaMouseX()); 
    m_ChartViewer.updateDisplay();
}
void CfireGuardGraphDlg::OnMouseMovePlotAreaVeloc()
{
	trackLineLegend((XYChart *)m_VelocViewer.getChart(), m_VelocViewer.getPlotAreaMouseX());
	m_VelocViewer.updateDisplay();
}
void CfireGuardGraphDlg::OnMouseMovePlotAreaSlope()
{
	trackLineLegend((XYChart *)m_SlopeViewer.getChart(), m_SlopeViewer.getPlotAreaMouseX());
	m_SlopeViewer.updateDisplay();
}

/////////////////////////////////////////////////////////////////////////////
// CfireGuardGraphDlg methods

//
// A utility to shift a new data value into a data array
//
static void shiftData(double *data, int len, double newValue)
{
    memmove(data, data + 1, sizeof(*data) * (len - 1));
    data[len - 1] = newValue;
}

//
// The data acquisition routine. In this demo, this is invoked every 250ms.
//
bool isAllTrue(bool*  array, int max)
{
	for (int i = 0; i < max; i++) {
		if (array[i] == false) return false;
	}
	return true;
}
bool isAllFalse(bool*  array, int max)
{
	for (int i = 0; i < max; i++) {
		if (array[i] == true) return false;
	}
	return true;
}
bool isOneOfThemTrue(bool*  array, int max)
{
	for (int i = 0; i < max; i++) {
		if (array[i] == true) return true;
	}
	return false;
}
bool isOneOfThemFalse(bool*  array, int max)
{
	for (int i = 0; i < max; i++) {
		if (array[i] == false) return true;
	}
	return false;
}

void setAll(bool*  array, int max, bool value)
{
	for (int i = 0; i < max; i++) {
		 array[i] = value;
	}
}




void CfireGuardGraphDlg::OnDataTimer()
{
	static bool hasTemperAlarm[MAX_CAMERA] = { false, false, false, false, false, false, false, false };
	static bool hasVelociAlarm[MAX_CAMERA] = { false, false, false, false, false, false, false, false };

	static bool hasTemperBgChanged[MAX_CAMERA] = { false, false, false, false, false, false, false, false };
	static bool hasVelociBgChanged[MAX_CAMERA] = { false, false, false, false, false, false, false, false };

	// The current time in millisecond resolution
	SYSTEMTIME st;
	GetLocalTime(&st);
	double now = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,
		st.wSecond) + st.wMilliseconds / 1000.0;

	TraceLog(("message received , st.wSecond = %d,  %d", st.wSecond, DataInterval));

	static int timeCounter = 0;
	timeCounter++;
	SetCounter(timeCounter, -1.0f);

	// This is our formula for the random number generator
	do
	{
		//
		// In this demo, we use some formulas to generate new values. In real applications,
		// it may be replaced by some data acquisition code.
		//
		//double ppoint = m_nextDataTime * 4;

		// In this demo, if the data arrays are full, the oldest 5% of data are discarded.
		if (m_currentIndex >= sampleSize)
		{
			// 0 부터 시작해서, sampleSize 까지 하나씩 채우다가,  꽉 차면,
			// 다시 처음부터 채우는 것이 아니고,  전체 값을 좌측으로 shift 하고, 제일 마지막 배열에 최신값을 넣는다.
			// 따라서, 일정 시간이 지나면, m_currentIndex 는 항상 sampleSize -1 에 있게 된다.
			//m_currentIndex = sampleSize * 95 / 100 - 1;

			//for (int i = 0; i < m_currentIndex; ++i)
			//{
			//	for (int j = 0; j < MAX_CAMERA; ++j)
			//	{
			//		int srcIndex = i + sampleSize - m_currentIndex;
			//		m_timeStamps[i] = m_timeStamps[srcIndex];
			//		m_dataSeries[j][i] = m_dataSeries[j][srcIndex];
			//		m_rateSeries[j][i] = m_rateSeries[j][srcIndex];
			//		m_slopeSeries[j][i] = m_slopeSeries[j][srcIndex];
			//		m_countSeries[j][i] = m_countSeries[j][srcIndex];
			//	}
			//}
			m_currentIndex = sampleSize - 1;
			for (int i = 0; i < m_currentIndex ; ++i)
			{
				for (int j = 0; j < MAX_CAMERA; ++j)
				{
					m_timeStamps[i] = m_timeStamps[i+1];
					m_dataSeries[j][i] = m_dataSeries[j][i+1];
					m_rateSeries[j][i] = m_rateSeries[j][i+1];
					m_slopeSeries[j][i] = m_slopeSeries[j][i+1];
					m_countSeries[j][i] = m_countSeries[j][i+1];
				}
			}
		}

		m_max_temperature = -999.0f;

		// Store the new values in the current index position, and increment the index.
		m_timeStamps[m_currentIndex] = m_nextDataTime;
		for (int j = 0; j < MAX_CAMERA; ++j)
		{
			CString idStr;
			idStr.Format("%d", j + 1);
			double value = m_session->GetData(idStr, TEMPER_OVER);
			if (value == _NO_VALUE_ || value == Chart::NoValue)
			{
				continue;
			}
			m_dataSeries[j][m_currentIndex] = value;  // 최근 온도값을 넣고
			if (m_max_temperature < value)
			{
				m_max_temperature = value;  //지금까지 온도중 최고값을 항상 갱신하면서...
			}

			//if (j >= MAX_CAMERA){
			//	//value = m_session->GetData(idStr, VELOCITY_OVER);
			//	// m_rateSeries[j][m_currentIndex] = value;
			//}
			//else
			if (m_currentIndex == 0)
			{
				m_rateSeries[j][m_currentIndex] = 0;
				m_slopeSeries[j][m_currentIndex] = 0;
				m_countSeries[j][m_currentIndex] = 0;
			}
			else
			{
				// 면적을 구하기 위해서 기울기를 합산한다.
				// 면적이 지속적으로 양의 값을 기록한다면 온도가 지속적으로 상승하고 있다고 볼 수  있다.
				// 여기서  m_frequency 는 돗수를 의미하는데, 디폴트는 100 이다.  즉 100개 값을 합산한다.

				// 이 포뮬라의 결과로 기울기 값이 m_slopeSeries 에 들어있게 되고,
				// 이 포뮬라는 100개의 평균을 리턴하므로,   m_rateSeries 에는 평균값이 들어있게 된다.  그런데,,rate 는 사용되지 않는다.
				//double deviation = 0.0f;
				//m_rateSeries[j][m_currentIndex] = Formula(j, m_currentIndex, m_velociThreshold->m_frequency);
				
				// slopeSeries 에 기울기값을 넣는다.
				Formula1(j, m_currentIndex, m_velociThreshold->m_frequency);
				// countSeries 에 이동평균 값을 넣는다.
				Formula2(j, m_currentIndex, m_velociThreshold->m_frequency);
				// countSeries slope  를 모두 더한 값을 넣는다.
				//Formula3(j, m_currentIndex, m_velociThreshold->m_frequency);




				// slope를 돗수 만큼 다시 더해서, 양의 값을 기록하면,  다시  countSeries 에 넣고, 이게 임계치를 넘으면 상승으로 본다???

				
				//if (m_rateSeries[j][m_currentIndex] > 0)
				//{
					//if (m_velociThreshold->m_deviation > 0.0f)
					//{
					//	deviation = deviation * (m_velociThreshold->m_deviation/100.0f);
					//}
					//SetCounter(timeCounter, m_velociThreshold->m_deviation);

					//if ((m_rateSeries[j][m_currentIndex - 1] + deviation) <= m_rateSeries[j][m_currentIndex])  // 이전값보다 크거나 같다.
					//if (m_rateSeries[j][m_currentIndex - 1]  <= m_rateSeries[j][m_currentIndex])  // 이전값보다 크거나 같다.
				
				
				//if (m_rateSeries[j][m_currentIndex]  > 0 )  // 적분값이 0 보다 크면 상승중인 것이다.  값이 이전보다 줄어든 것은 다만, 상승률이 둔화된 것이다.
				//if (isIncrease(j, m_currentIndex, m_velociThreshold->m_frequency))
				//if (m_rateSeries[j][m_currentIndex] > 0)  // rate 에는  slope 를 다 더한 값이 들어있다.
				//{
				//	// 상승분은 시간 milli second 만큼으로 보는데, 현재는 Data 인터벌이 250 이므로 1/4 초가 하나의 돗수가 된다. 
				//	m_countSeries[j][m_currentIndex] = m_countSeries[j][m_currentIndex - 1] + (DataInterval / 1000.0f);
				//}
				//else
				//{
				//	m_countSeries[j][m_currentIndex] = 0.0f;
				//}
				// external !!!!
				double value = m_countSeries[j][m_currentIndex];
				double threshold = atof(FireProcess::getInstance()->trendThreshold);
				int level = value < threshold ? 0 : 1;
				CString  stream;
				stream.Format("TREND/%d/%.2f/%.1f", j+1, value, threshold);
				FireProcess::getInstance()->exPush(stream);
				
				
				//}
				//else
				//{
				//	m_countSeries[j][m_currentIndex] = 0.0f;
				//}
				
			}
		}



		if (!m_isStopAlarm)
		{
			for (int j = 0; j < MAX_CAMERA; ++j)
			{
				if (m_checkCamera[j].GetCheck() == 0)
				{
					continue;
				}
				CString idStr;
				idStr.Format("%d", j + 1);
				double value = m_session->GetData(idStr, TEMPER_OVER);
				//TraceLog(("RunAlarmSound temper %f", value));
				hasTemperAlarm[j] = this->m_temperThreshold->RunAlarmSound(j, value);
				
				if (hasTemperAlarm[j])
				{
					//if (!m_temperThreshold->hasAleadyPlayed())
					//{
					//	SendSMS(j + 1, TEMPER_OVER, value);
					//	CString command;
					//	command.Format("POPUP/%d/0", j);
					//	preSocketHandler::getInstance()->pushCommand("POPUP", j, 0); // popup 으로 화면을 띠울것을 요청한다.
					//}
					if (!hasTemperBgChanged[j])
					{
						// Change Chart Color
							CopyFileA(TEMPER_ALARM_BG, TEMPER_BG, FALSE);
							TraceLog(("setBgImage to alarm"));
							hasTemperBgChanged[j] = true;
							preSocketHandler::getInstance()->pushCommand("POPUP", j, 1); // popup 으로 화면을 띠울것을 요청한다.
							SendSMS(j + 1, TEMPER_OVER, value);
							
							//CString aData = EXTERNAL_MSG_PREFIX;
							//aData.Format("%sTEMP_OVER/%d/%.00f/%.00f", EXTERNAL_MSG_PREFIX, j, value, m_temperThreshold->m_thresholdMax[j]);
							//FireProcess::getInstance()->exDialog(aData);  // external
					}
					m_AlarmLogger->SaveLog(idStr, value);
					break;
				}
			}
		}
		if (!m_isStopTrend)
		{
			for (int j = 0; j < MAX_CAMERA; ++j)
			{
				if (m_checkCamera[j].GetCheck() == 0)
				{
					continue;
				}
				//double rate = m_rateSeries[j][m_currentIndex];
				double rate = m_countSeries[j][m_currentIndex];
				//TraceLog(("RunAlarmSound velo %f", rate));
				hasVelociAlarm[j] = this->m_velociThreshold->RunAlarmSound(rate);

				if (hasVelociAlarm[j])
				{
					//if (!m_velociThreshold->hasAleadyPlayed())
					//{
					//	SendSMS(j + 1, VELOCITY_OVER, rate);
					//}
					if (!hasVelociBgChanged[j]) {
						CopyFileA(VELO_ALARM_BG, VELO_BG, FALSE);
						hasVelociBgChanged[j] = true;
						preSocketHandler::getInstance()->pushCommand("POPUP", j, 2); //  popup 으로 화면을 띠울것을 요청한다.
						SendSMS(j + 1, VELOCITY_OVER, rate);
					}
					CString idStr;
					idStr.Format("%d", j + 1);
					m_TrendLogger->SaveLog(idStr, rate);
					break;
				}
			}
		}

		if (isAllFalse(hasTemperAlarm, MAX_CAMERA))
		{
			if (isAllFalse(hasVelociAlarm, MAX_CAMERA)) {  // 사운드 Stop 은 두가지 종류의 알람이 모두 없어져야만 한다.
				this->OnBnClickedBtStop();
			}
			// Change Chart Color
			if (isOneOfThemTrue(hasTemperBgChanged, MAX_CAMERA)) {
				setAll(hasTemperBgChanged, MAX_CAMERA, false);
				CopyFileA(TEMPER_NORMAL_BG, TEMPER_BG, FALSE);
				TraceLog(("setBgImage to normal"));
			}
		}

		if (isAllFalse(hasVelociAlarm, MAX_CAMERA))
		{
			if (isAllFalse(hasTemperAlarm, MAX_CAMERA)) {  // 사운드 Stop 은 두가지 종류의 알람이 모두 없어져야만 한다.
				this->OnBnClickedBtVelocStop();
			}
			if (isOneOfThemTrue(hasVelociBgChanged, MAX_CAMERA)) {
				setAll(hasVelociBgChanged, MAX_CAMERA, false);
				CopyFileA(VELO_NORMAL_BG, VELO_BG, FALSE);
			}
		}

		++m_currentIndex;
		m_nextDataTime += DataInterval / 1000.0;

		//TraceLog(("message received gap=%f", now-m_nextDataTime));
	} while (m_nextDataTime < now);
	TraceLog(("message received gap=%f", now - m_nextDataTime));
	//
	// We provide some visual feedback to the latest numbers generated, so you can see the
	// data being generated.
	//
	char buffer[1024];

	for (int i = 0; i < MAX_CAMERA; i++)
	{
		double value = m_dataSeries[i][m_currentIndex - 1];
		if (value != _NO_VALUE_ && value != Chart::NoValue)
		{
			sprintf(buffer, " %.2f", value);
			m_TemperVal[i].SetWindowText(CString(buffer));
			if (!userCheck[i])
			{
				m_checkCamera[i].SetCheck(true);
				m_checkTH[i].SetCheck(true);
				m_editThresholdMax[i].EnableWindow(TRUE);
				m_btShow[i].EnableWindow(true);
			}
		}
		else
		{
			if (!userCheck[i])
			{
				m_checkCamera[i].SetCheck(false);
				m_btShow[i].EnableWindow(false);
			}
			m_checkTH[i].SetCheck(false);
			m_editThresholdMax[i].EnableWindow(FALSE);
		}
	}
}

//
// Draw the chart and display it in the given viewer
//
void CfireGuardGraphDlg::drawChart(CChartViewer *viewer)
{
	const int width = 720;
	const int height = 420;
    // Create an XYChart object 600 x 270 pixels in size, with light grey (f4f4f4) 
    // background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
	// XYChart *c = new XYChart(600, 270, 0xf4f4f4, 0x000000, 1);
	XYChart* tChart = new XYChart(width, height, 0xf4f4f4, 0x000000, 1);
    //tChart->setRoundedFrame(m_extBgColor);
    
    // Set the plotarea at (55, 55) and of size 520 x 185 pixels. Use white (ffffff) 
    // background. Enable both horizontal and vertical grids by setting their colors to 
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
	//tChart->setPlotArea(55, 55, 520, 185, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
	tChart->setPlotArea(55, 65, width-80, height-95, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
    tChart->setClipping();
	tChart->setBgImage(TEMPER_BG);
    // Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
    // grey (dddddd) background, black (000000) border, and a glass like raised effect.
	//CString title;
	//title.Format("Temperature Monitoring : [%.2f]", m_max_temperature);
	//tChart->addTitle(title, "tahoma.ttf", 15)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());
            
    // Set the reference font size of the legend box
    tChart->getLegend()->setFontSize(10);

    // Configure the y-axis with a 10pts Arial Bold axis title
    tChart->yAxis()->setTitle("Temperature(C)", "tahoma.ttf", 10);

    // Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
    // 15  pixels between minor ticks. This shows more minor grid lines on the chart.
    tChart->xAxis()->setTickDensity(75, 15);

    // Set the axes width to 2 pixels
    tChart->xAxis()->setWidth(2);
    tChart->yAxis()->setWidth(2);

	tChart->yAxis()->setLinearScale(m_ChartViewer.GetMin(), m_ChartViewer.GetMax());  //skpark disable autoScale


    // Now we add the data to the chart. 
    double firstTime  = m_timeStamps[0];
    if (firstTime != Chart::NoValue)
    {
        // Set up the x-axis to show the time range in the data buffer
        tChart->xAxis()->setDateScale(firstTime, firstTime + DataInterval * sampleSize / 1000);
        
        // Set the x-axis label format
        tChart->xAxis()->setLabelFormat("{value|hh:nn:ss}");

        // Create a line layer to plot the lines
        LineLayer *layer = tChart->addLineLayer();

        // The x-coordinates are the timeStamps.
        layer->setXData(DoubleArray(m_timeStamps, sampleSize));

		int color[MAX_CAMERA] = { 0xffa800, 0x9600ff, 0x30ff00, 0x007323, 0x00f0ff, 0x2764ff, 0x0a0572, 0xfff000 };

		for (int i = 0; i < MAX_CAMERA; i++)
		{
			if (!m_checkCamera[i].GetCheck()) continue;
			CString name;
			name.Format("Camera%d", i + 1);
			layer->addDataSet(DoubleArray(m_dataSeries[i], sampleSize), color[i], name);
		}
		//layer->addDataSet(DoubleArray(m_dataSeries[MAX_CAMERA], sampleSize), 0x000000, "MAX");
		//layer->addDataSet(DoubleArray(m_dataSeries[MAX_CAMERA + 1], sampleSize), 0x000000, "MIN");

	}

	// Include track line with legend. If the mouse is on the plot area, show the track 
    // line with legend at the mouse position; otherwise, show them for the latest data
    // values (that is, at the rightmost position).
	trackLineLegend(tChart, viewer->isMouseOnPlotArea() ? viewer->getPlotAreaMouseX() :
        tChart->getPlotArea()->getRightX());

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
	viewer->setChart(tChart);
}

void CfireGuardGraphDlg::drawVeloc(CChartViewer *viewer)
{
	const int width = 720;
	const int height = 250;
	// Create an XYChart object 600 x 270 pixels in size, with light grey (f4f4f4) 
	// background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
	// XYChart *c = new XYChart(600, 270, 0xf4f4f4, 0x000000, 1);
	XYChart* vChart = new XYChart(width, height, 0xf4f4f4, 0x000000, 1);
	//c->setRoundedFrame(m_extBgColor);

	// Set the plotarea at (55, 55) and of size 520 x 185 pixels. Use white (ffffff) 
	// background. Enable both horizontal and vertical grids by setting their colors to 
	// grey (cccccc). Set clipping mode to clip the data lines to the plot area.
	//c->setPlotArea(55, 55, 520, 185, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
	vChart->setPlotArea(55, 65, width - 80, height - 95, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
	vChart->setClipping();
	vChart->setBgImage(VELO_BG);

	// Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
	// grey (dddddd) background, black (000000) border, and a glass like raised effect.
	//CString title;
	//title.Format("UpTrend Monitoring");

	//vChart->addTitle(title, "tahoma.ttf", 15
	//	)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());

	// Set the reference font size of the legend box
	vChart->getLegend()->setFontSize(10);

	// Configure the y-axis with a 10pts Arial Bold axis title
	vChart->yAxis()->setTitle("moving average", "tahoma.ttf", 10);

	// Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
	// 15  pixels between minor ticks. This shows more minor grid lines on the chart.
	vChart->xAxis()->setTickDensity(75, 15);

	// Set the axes width to 2 pixels
	vChart->xAxis()->setWidth(2);
	vChart->yAxis()->setWidth(2);

	vChart->yAxis()->setLinearScale(m_VelocViewer.GetMin(), m_VelocViewer.GetMax());  //skpark disable autoScale

	// Now we add the data to the chart. 
	double firstTime = m_timeStamps[0];
	if (firstTime != Chart::NoValue)
	{
		// Set up the x-axis to show the time range in the data buffer
		vChart->xAxis()->setDateScale(firstTime, firstTime + DataInterval * sampleSize / 1000);

		// Set the x-axis label format
		vChart->xAxis()->setLabelFormat("{value|hh:nn:ss}");

		// Create a line layer to plot the lines
		LineLayer *layer = vChart->addLineLayer();

		// The x-coordinates are the timeStamps.
		layer->setXData(DoubleArray(m_timeStamps, sampleSize));

		int color[MAX_CAMERA] = { 0xffa800, 0x9600ff, 0x30ff00, 0x007323, 0x00f0ff, 0x2764ff, 0x0a0572, 0xfff000 };

		for (int i = 0; i < MAX_CAMERA; i++)
		{
			if (!m_checkCamera[i].GetCheck()) continue;
			CString name;
			name.Format("Camera%d", i + 1);
			//layer->addDataSet(DoubleArray(m_rateSeries[i], sampleSize), color[i], name);
			layer->addDataSet(DoubleArray(m_countSeries[i], sampleSize), color[i], name);
		}
		//layer->addDataSet(DoubleArray(m_rateSeries[MAX_CAMERA], sampleSize), 0x000000, "MAX");
		//layer->addDataSet(DoubleArray(m_rateSeries[MAX_CAMERA + 1], sampleSize), 0x000000, "MIN");

	}

	// Include track line with legend. If the mouse is on the plot area, show the track 
	// line with legend at the mouse position; otherwise, show them for the latest data
	// values (that is, at the rightmost position).
	trackLineLegend(vChart, viewer->isMouseOnPlotArea() ? viewer->getPlotAreaMouseX() :
		vChart->getPlotArea()->getRightX());

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
	viewer->setChart(vChart);
}

void CfireGuardGraphDlg::drawSlope(CChartViewer *viewer)
{
	const int width = 720;
	const int height = 250;
	// Create an XYChart object 600 x 270 pixels in size, with light grey (f4f4f4) 
	// background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
	// XYChart *c = new XYChart(600, 270, 0xf4f4f4, 0x000000, 1);
	XYChart* sChart = new XYChart(width, height, 0xf4f4f4, 0x000000, 1);
	//sChart->setRoundedFrame(m_extBgColor);

	// Set the plotarea at (55, 55) and of size 520 x 185 pixels. Use white (ffffff) 
	// background. Enable both horizontal and vertical grids by setting their colors to 
	// grey (cccccc). Set clipping mode to clip the data lines to the plot area.
	//sChart->setPlotArea(55, 55, 520, 185, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
	sChart->setPlotArea(55, 65, width - 80, height - 95, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
	sChart->setClipping();
	sChart->setBgImage(SLOPE_BG);

	// Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
	// grey (dddddd) background, black (000000) border, and a glass like raised effect.
	//sChart->addTitle("Change rate Monitoring", "tahoma.ttf", 15
	//	)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());

	// Set the reference font size of the legend box
	sChart->getLegend()->setFontSize(10);

	// Configure the y-axis with a 10pts Arial Bold axis title
	sChart->yAxis()->setTitle("Slope", "tahoma.ttf", 10);

	// Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
	// 15  pixels between minor ticks. This shows more minor grid lines on the chart.
	sChart->xAxis()->setTickDensity(75, 15);

	// Set the axes width to 2 pixels
	sChart->xAxis()->setWidth(2);
	sChart->yAxis()->setWidth(2);

	//sChart->yAxis()->setLinearScale(m_SlopeViewer.GetMin(), m_SlopeViewer.GetMax());  //skpark enable autoScale

	// Now we add the data to the chart. 
	double firstTime = m_timeStamps[0];
	if (firstTime != Chart::NoValue)
	{
		// Set up the x-axis to show the time range in the data buffer
		sChart->xAxis()->setDateScale(firstTime, firstTime + DataInterval * sampleSize / 1000);

		// Set the x-axis label format
		sChart->xAxis()->setLabelFormat("{value|hh:nn:ss}");

		// Create a line layer to plot the lines
		LineLayer *layer = sChart->addLineLayer();

		// The x-coordinates are the timeStamps.
		layer->setXData(DoubleArray(m_timeStamps, sampleSize));

		int color[MAX_CAMERA] = { 0xffa800, 0x9600ff, 0x30ff00, 0x007323, 0x00f0ff, 0x2764ff, 0x0a0572, 0xfff000 };

		for (int i = 0; i < MAX_CAMERA; i++)
		{
			if (!m_checkCamera[i].GetCheck()) continue;
			CString name;
			name.Format("Camera%d", i + 1);
			layer->addDataSet(DoubleArray(m_slopeSeries[i], sampleSize), color[i], name);
		}
		//layer->addDataSet(DoubleArray(m_rateSeries[MAX_CAMERA], sampleSize), 0x000000, "MAX");
		//layer->addDataSet(DoubleArray(m_rateSeries[MAX_CAMERA + 1], sampleSize), 0x000000, "MIN");

	}

	// Include track line with legend. If the mouse is on the plot area, show the track 
	// line with legend at the mouse position; otherwise, show them for the latest data
	// values (that is, at the rightmost position).
	trackLineLegend(sChart, viewer->isMouseOnPlotArea() ? viewer->getPlotAreaMouseX() :
		sChart->getPlotArea()->getRightX());

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
	viewer->setChart(sChart);
}

// Draw the track line with legend
//
void CfireGuardGraphDlg::trackLineLegend(XYChart *c, int mouseX)
{
    // Clear the current dynamic layer and get the DrawArea object to draw on it.
    DrawArea *d = c->initDynamicLayer();

    // The plot area object
    PlotArea *plotArea = c->getPlotArea();

    // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
    double xValue = c->getNearestXValue(mouseX);
    int xCoor = c->getXCoor(xValue);

    // Draw a vertical track line at the x-position
    d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, d->dashLineColor(0x000000, 0x0101));

    // Container to hold the legend entries
	vector<string> legendEntries;

    // Iterate through all layers to build the legend array
    for (int i = 0; i < c->getLayerCount(); ++i) {
        Layer *layer = c->getLayerByZ(i);

        // The data array index of the x-value
        int xIndex = layer->getXIndexOf(xValue);

        // Iterate through all the data sets in the layer
        for (int j = 0; j < layer->getDataSetCount(); ++j) {
            DataSet *dataSet = layer->getDataSetByZ(j);

            // We are only interested in visible data sets with names
            const char *dataName = dataSet->getDataName();
            int color = dataSet->getDataColor();
			if (dataName && *dataName && (color != Chart::Transparent)) {
                // Build the legend entry, consist of the legend icon, name and data value.
                double dataValue = dataSet->getValue(xIndex);
				ostringstream legendEntry;
				legendEntry << "<*block*>" << dataSet->getLegendIcon() << " " << dataName << ": " <<
					((dataValue == Chart::NoValue) ? "N/A" : c->formatValue(dataValue, "{value|P4}"))
					<< "<*/*>";
				legendEntries.push_back(legendEntry.str());

                // Draw a track dot for data points within the plot area
                int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());
                if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY())) {
                    d->circle(xCoor, yCoor, 4, 4, color, color);
                }
            }
        }
    }

    // Create the legend by joining the legend entries
	ostringstream legendText;
	legendText << "<*block,maxWidth=" << plotArea->getWidth() << "*><*block*><*font=arialbd.ttf*>["
		<< c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss") << "]<*/*>";
	for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
		legendText << "        " << legendEntries[i];
	
    // Display the legend on the top of the plot area
    TTFText *t = d->text(legendText.str().c_str(), "arial.ttf", 8);
	t->draw(plotArea->getLeftX() + 5, plotArea->getTopY() - 3, 0x000000, Chart::BottomLeft);
	t->destroy();
}

/////////////////////////////////////////////////////////////////////////////
// General utilities

//
// Get the default background color
//
int CfireGuardGraphDlg::getDefaultBgColor()
{
    LOGBRUSH LogBrush; 
    HBRUSH hBrush = (HBRUSH)SendMessage(WM_CTLCOLORDLG, (WPARAM)CClientDC(this).m_hDC, 
        (LPARAM)m_hWnd); 
    ::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush); 
    int ret = LogBrush.lbColor;
    return ((ret & 0xff) << 16) | (ret & 0xff00) | ((ret & 0xff0000) >> 16);
}

//
// Load an icon resource into a button
//
void CfireGuardGraphDlg::loadButtonIcon(int buttonId, int iconId, int width, int height)
{
    GetDlgItem(buttonId)->SendMessage(BM_SETIMAGE, IMAGE_ICON, (LPARAM)::LoadImage(
        AfxGetResourceHandle(), MAKEINTRESOURCE(iconId), IMAGE_ICON, width, height, 
        LR_DEFAULTCOLOR));  
}

void CfireGuardGraphDlg::OnFrequencyTimer()
{
	//TraceLog(("OnFrequencyTimer()"));
	//CString buf;
	//buf.Format("9/%.1f/0/%d", m_temperThreshold->m_thresholdMax, TEMPER_OVER);
	//preSocketHandler::getInstance()->push(buf);
	//buf.Format("10/%.1f/0/%d", m_temperThreshold->m_thresholdMin, TEMPER_OVER);
	//preSocketHandler::getInstance()->push(buf);

	//buf.Format("9/%.1f/0/%d", m_velociThreshold->m_thresholdMax, VELOCITY_OVER);
	//preSocketHandler::getInstance()->push(buf);
	//buf.Format("10/%.1f/0/%d", m_velociThreshold->m_thresholdMin, VELOCITY_OVER);
	//preSocketHandler::getInstance()->push(buf);
}


void CfireGuardGraphDlg::OnDestroy()
{
	if (m_session)
	{
		delete m_session;
	}
	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	delete m_temperThreshold;
	delete m_velociThreshold;
	delete m_slopeThreshold;

	delete m_AlarmLogger;
	delete m_TrendLogger;

	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CfireGuardGraphDlg::OnBnClickedCheckAll()
{
	bool  all = m_checkCameraAll.GetCheck();
	for (int j = 0; j < MAX_CAMERA; j++)
	{
		m_checkCamera[j].SetCheck(all);
		userCheck[j] = true;
	}
}

void CfireGuardGraphDlg::OnBnClickedShowLog()
{
	CSelectCameraDlg dlg(this);
	for (int i = 0; i < MAX_CAMERA; i++)
	{
		dlg.enabled[i] = bool(m_checkCamera[i].GetCheck());
	}

	if (dlg.DoModal() == IDOK)
	{
		OpenLog(dlg.checked, dlg.prefix, dlg.dateTime);
	}

}
void CfireGuardGraphDlg::OpenLog(int cameraIdx, LPCTSTR prefix, LPCTSTR dateTime)
{


	CString filter  ;
	filter.Format("fireGuardGraph Log Files (%s%s*.csv)|%s%s*.csv||", prefix, dateTime, prefix,dateTime);

	CFileDialog dlg(TRUE, NULL, UBC_UPLOADER_PATH, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString selected;
		selected.Format(" %s %d", dlg.GetFileName(), cameraIdx);
		//CString szSPathName = dlg.GetPathName();

		TraceLog(("Execute(%sLogViewerChart.exe %s)", UBC_GUARDIAN_PATH, selected));
		HINSTANCE nRet = ShellExecute(NULL, NULL, "LogViewerChart.exe", selected, UBC_GUARDIAN_PATH, SW_SHOWNORMAL);
		if (int(nRet) < 32)
		{
			TraceLog(("ShellExecute Error=%d", nRet));
		}
	}

}

void CfireGuardGraphDlg::OnBnClickedOpenFolder()
{
	::ShellExecute(NULL, "open", UBC_UPLOADER_PATH, NULL, NULL, SW_SHOW);
}
void CfireGuardGraphDlg::OnBnClickedButtonMaxApply()
{
	for (int i = 0; i < MAX_CAMERA; i++) {
		if (m_checkTH[i].GetCheck() == FALSE) {
			m_editThresholdMax[i].SetWindowTextA("-1");
			continue;
		}
		CString maxStr;
		m_editThresholdMax[i].GetWindowTextA(maxStr);

		int max = atoi(maxStr);

		if (max < MIN_THRESHOLD)
		{
			m_editThresholdMax[i].SetWindowTextA("0");
		}
		if (max > MAX_THRESHOLD)
		{
			m_editThresholdMax[i].SetWindowTextA("499");
		}
	}
	m_temperThreshold->OnApply();
}

void CfireGuardGraphDlg::OnBnClickedBtStop()
{
	//TraceLog(("OnBnClickedBtStop Sound"));
	m_temperThreshold->OnStop();
}
void CfireGuardGraphDlg::OnBnClickedButtonVelocApply()
{
	CString freqStr;
	m_editFrequency.GetWindowTextA(freqStr);

	int frequency = atoi(freqStr);
	if (frequency > sampleSize)
	{
		frequency = sampleSize - 1;
		freqStr.Format("%d", frequency);
		m_editFrequency.SetWindowTextA(freqStr);
	}

	CString maxStr;
	m_editVelocMax.GetWindowTextA(maxStr);

	double max = atof(maxStr);
	/*CString buf;
	buf.Format("= %.2f 초", max / 4);
	m_stVeloSec.SetWindowTextA(buf);*/

	if (max < MIN_VELO)
	{
		m_editVelocMax.SetWindowTextA("0");
		m_stVeloSec.SetWindowTextA("= 0 초");
	}
	if (max > MAX_VELO)
	{
		freqStr.Format("%d", MAX_VELO);
		m_editVelocMax.SetWindowTextA(freqStr);
	/*	CString buf;
		buf.Format("= %d 초", MAX_VELO/4);
		m_stVeloSec.SetWindowTextA(buf);*/
	}

	//m_editDevi.GetWindowTextA(maxStr);
	//max = atoi(maxStr);

	//if (max < 0)
	//{
	//	m_editDevi.SetWindowTextA("0");
	//}
	//if (max >= 100)
	//{
	//	freqStr.Format("%d", 99);
	//	m_editDevi.SetWindowTextA(freqStr);
	//}


	m_velociThreshold->OnApply();
	//m_slopeThreshold->OnApply();
	//m_checkSMS.SetCheck(true);
}
void CfireGuardGraphDlg::OnBnClickedBtVelocStop()
{
	//TraceLog(("OnBnClickedBtVelocStop Sound"));
	m_velociThreshold->OnStop();
	//m_slopeThreshold->OnStop();
	//m_checkSMS.SetCheck(true);
}
bool  CfireGuardGraphDlg::isIncrease(int cameraId, int currentIndex, int frequency)
{
	
	// 그냥,  앞의 숫자보다 크면 양의 값을 아니면 음의 값을 리턴한다.
	bool retval = (m_rateSeries[cameraId][currentIndex] - m_rateSeries[cameraId][currentIndex-1] > 0);
	//TraceLog(("2022: %d , rate[%d]=%0.2f, rate[%d]=%0.2f", (retval ? 1 : 0), currentIndex, m_rateSeries[cameraId][currentIndex], currentIndex - 1, m_rateSeries[cameraId][currentIndex - 1]));
	return retval;

}

void  CfireGuardGraphDlg::Formula1(int cameraId, int currentIndex, int frequency)
{
	static bool isFirstTime = true;

	// frequency 는 표본의 크기를 의미한다.
	int startIdx = ((currentIndex > frequency) ? currentIndex - frequency : 0);
	int endIdx = currentIndex;

	std::vector<double> temperVector;
	//한칸씩 이동하면서,  한칸씩 옆으로 이동하면서, 
	for (int i = startIdx; i <= endIdx; i++)
	{
		double curVal = GetDataValue(cameraId, i);  // 과거 100개 전부터 온도값을 하나씩 꺼내와서..
		temperVector.push_back(curVal);
	}
	// 표번 100개의 기울기를 구했다.
	double slope = getSlope(temperVector);
	
	if (currentIndex > frequency)  // 처음 frequency 개 값을 버린다.
	{
		isFirstTime = false;
	}
	if (!isFirstTime)  // 처음 frequency 개 값을 버린다.  currentIndex 값은 일정시간이 지나면 frequency 보다 항상 큰 값이다. ( m_currentIndex = sampleSize -1) 따라서 처음에 frequency 값만 버리게 된다.
	{
		// 기울기 값이 m_slopeSeries 에 들어 간다.
		m_slopeSeries[cameraId][currentIndex] = slope;
	}

}

void  CfireGuardGraphDlg::Formula2(int cameraId, int currentIndex, int frequency)
{
	static bool isFirstTime = true;

	// frequency 는 표본의 크기를 의미한다.
	int startIdx = ((currentIndex > frequency) ? currentIndex - frequency : 0);
	int endIdx = currentIndex;

	std::vector<double> slopeVector;
	//한칸씩 이동하면서,  한칸씩 옆으로 이동하면서, 
	double sum = 0;
	int count = 0;
	for (int i = startIdx; i <= endIdx; i++)
	{
		double curVal = m_slopeSeries[cameraId][i];  // 과거 100개 전부터 온도값을 하나씩 꺼내와서..
		sum += curVal;
		count++;
	}
	// 100개의
	//double avg = movingAverage(slopeVector, frequency);
	double avg = sum / count;

	if (currentIndex > frequency)  // 처음 frequency 개 값을 버린다.
	{
		isFirstTime = false;
	}
	if (!isFirstTime)  // 처음 frequency 개 값을 버린다.  currentIndex 값은 일정시간이 지나면 frequency 보다 항상 큰 값이다. ( m_currentIndex = sampleSize -1) 따라서 처음에 frequency 값만 버리게 된다.
	{
		// 이동 평균 값이 m_rateSeries 에 들어 간다.
		m_countSeries[cameraId][currentIndex] = slopeToAngle(avg);  // 값이 너무 작으므로 100배 증폭한다.
	}

}


double CfireGuardGraphDlg::slopeToAngle(double slope) {
	const double M_PI = 3.14159265358979323846;

	double radian = atan(slope);
	double degree = radian * (180.0 / M_PI);
	return degree;
}

void  CfireGuardGraphDlg::Formula3(int cameraId, int currentIndex, int frequency)
{
	static bool isFirstTime = true;

	// frequency 는 표본의 크기를 의미한다.
	int startIdx = ((currentIndex > frequency) ? currentIndex - frequency : 0);
	int endIdx = currentIndex;

	// 100개의
	double sum = 0;
		std::vector<double> slopeVector;
	//한칸씩 이동하면서,  한칸씩 옆으로 이동하면서, 
	for (int i = startIdx; i <= endIdx; i++)
	{
		double curVal = m_slopeSeries[cameraId][i];  // 과거 100개 전부터 온도값을 하나씩 꺼내와서..
		sum += curVal;  // 기울기를 다 더한다.
 	}

	if (currentIndex > frequency)  // 처음 frequency 개 값을 버린다.
	{
		isFirstTime = false;
	}
	if (!isFirstTime)  // 처음 frequency 개 값을 버린다.  currentIndex 값은 일정시간이 지나면 frequency 보다 항상 큰 값이다. ( m_currentIndex = sampleSize -1) 따라서 처음에 frequency 값만 버리게 된다.
	{
		// 이동 평균 값이 m_slopeSeries 에 들어 간다.
		// rate 에 넣을 예정이지만, 임시로 count 에 넣어본다.
		m_rateSeries[cameraId][currentIndex] = sum;
	}

}

double  CfireGuardGraphDlg::Formula(int cameraId, int currentIndex, int frequency)
{

	// 회귀분석 모델이 적분값을 리턴한다.
	static bool isFirstTime = true;


	// frequency 는 표본의 크기를 의미한다.
	int startIdx = ((currentIndex > frequency) ? currentIndex - frequency : 0);
	int endIdx = currentIndex;

	double sumX = 0.0f;
	double sumY = 0.0f;
	double count = 0.0f;

	std::vector<double> temperVector;
	//한칸씩 이동하면서,  한칸씩 옆으로 이동하면서, 
	for (int i = startIdx; i <= endIdx; i++)
	{
		double curVal = GetDataValue(cameraId, i);  // 과거 100개 전부터 온도값을 하나씩 꺼내와서..
		temperVector.push_back(curVal);
		//count += 1.0f;
		//sumY += curVal;  // 과거 100개 온도를 모두 더한다. .
		//sumX += m_timeStamps[i];    // timeStamp는  우상향하는 직선그래프이다.   DataInterval 초 단위로 상승하는 값이다. 
	}

	// 표번 100개의 기울기를 구했다.
	double slope = getSlope(temperVector);


	//// 표본 평균
	//double avgY = sumY / count;		// 과거 100개의 평균온도
	//double avgX = sumX / count;      // 평균은 측정표본의 중간시점이 된다.

	//// 표본 분산
	//double dYY = 0.0f;
	//double dXX = 0.0f;
	//double dXY = 0.0f;
	//for (int i = startIdx; i <= endIdx; i++)
	//{
	//	double dy = GetDataValue(cameraId, i) - avgY;
	//	double dx = abs(m_timeStamps[i] - avgX); // dx 는 절대값으로 처리해야 한다.
	//	dYY += pow(dy, 2);
	//	dXX += pow(dx, 2);  
	//	dXY += dy*dx;
	//}

	//if (dXY == 0 ||  dXX == 0)
	//{
	//	return 0.0f;
	//}

	//// 기울기 와 y절편
	//double slope =  dXY/dXX;
	//double bias = avgY - slope*avgX;

	// 이전의 frequency-1 개의 slope를 모두 더해서, 그 값을 range 번째에 넣는다.
	// 이 값이 양이면 range 기간동안 온도가 상승한 것이고, 음이면 하락한 것이다.
	if (currentIndex > frequency)  // 처음 frequency 개 값을 버린다.
	{
		isFirstTime = false;
	}
	if (!isFirstTime)  // 처음 frequency 개 값을 버린다.  currentIndex 값은 일정시간이 지나면 frequency 보다 항상 큰 값이다. ( m_currentIndex = sampleSize -1) 따라서 처음에 frequency 값만 버리게 된다.
	{
		// 기울기 값이 m_slopeSeries 에 들어 간다.
		m_slopeSeries[cameraId][currentIndex] = slope;
	}

	//TraceLog(("startIdx=%d,endIdx=%d", startIdx, endIdx));

	// 2022.08.198 . 그냥 이전값과의 차를 리턴하는 것으로...
	// slope 값을 모두 더한다.
	//double retval = 0.0f;
	//for (int i = startIdx; i <= endIdx; i++)
	//{
	//	retval += m_slopeSeries[cameraId][i];
	//}
	//return retval;

	// 그냥 표본평균을 리턴한다.
	return getAverage(temperVector);
}
double CfireGuardGraphDlg::getSlope(const std::vector<double>& temperatures) {
	// 표본 n 개의 기울기를 구하는 공식이다.
	int n = temperatures.size();
	if (n == 0) {
		// No data points
		return 0.0;
	}

	double sumX = 0.0;
	double sumY = 0.0;
	double sumXY = 0.0;
	double sumX2 = 0.0;

	for (int i = 0; i < n; i++) {
		sumX += i;
		sumY += temperatures[i];
		sumXY += i * temperatures[i];
		sumX2 += i * i;
	}

	double numerator = n * sumXY - sumX * sumY;
	double denominator = n * sumX2 - sumX * sumX;

	if (denominator == 0.0) {
		// Avoid division by zero
		return 0.0;
	}

	return numerator / denominator;
}


double CfireGuardGraphDlg::movingAverage(const std::vector<double>& data, int period) {
	int n = data.size();
	//std::vector<double> result;
	for (int i = 0; i < n - period + 1; i++) {
		double sum = 0.0;
		for (int j = i; j < i + period; j++) {
			sum += data[j];
		}
		return  (sum / period);
	}
}

double CfireGuardGraphDlg::getAverage(const std::vector<double>& values) {
	int n = values.size();
	if (n == 0) {
		// No data points
		return 0.0;
	}

	double sum = 0.0;
	for (double value : values) {
		sum += value;
	}

	return sum / n;
}

void CfireGuardGraphDlg::OnBnClickedCheckAlarm()
{
	if (m_checkStopAlarm.GetCheck())
	{
		m_isStopAlarm = true;
		m_temperThreshold->OnStop();
		//m_velociThreshold->OnStop();
		//m_slopeThreshold->OnStop();
	}
	else
	{
		m_isStopAlarm = false;
	}

	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString buf;
	buf.Format("%d", m_isStopAlarm);
	WritePrivateProfileString("FIRE_WATCH", "CHECK_DONT_ALARM",  buf, iniPath);

}

void CfireGuardGraphDlg::OnBnClickedCheckTrend()
{
	if (m_checkStopTrend.GetCheck())
	{
		m_isStopTrend = true;
		//m_temperThreshold->OnStop();
		TraceLog(("OnBnClickedCheckTrend Sound"));

		m_velociThreshold->OnStop();
		//m_slopeThreshold->OnStop();
	}
	else
	{
		m_isStopTrend = false;
	}

	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString buf;
	buf.Format("%d", m_isStopTrend);
	WritePrivateProfileString("FIRE_WATCH", "CHECK_DONT_TREND", buf, iniPath);

}


void CfireGuardGraphDlg::OnBnClickedButtonSmsMgr()
{
	SMSManagerDlg aDlg(this);
	for (int i = 0; i < MAX_DATA; i++)
	{
		m_sendData[i].Copy(aDlg.m_sendData[i]);
	}

	if (aDlg.DoModal() == IDOK)
	{
		for (int i = 0; i < MAX_DATA; i++)
		{
			aDlg.m_sendData[i].Copy(m_sendData[i]);
		}
	}
}

void CfireGuardGraphDlg::LoadSendData()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;
	char buf[MAX_DATA][512];

	for (int i = 0; i < MAX_DATA; i++)
	{
		memset(buf[i], 0x00, 512);
		CString name;
		name.Format("STR%d", i);
		GetPrivateProfileString("FIRE_WATCH", name, "", buf[i], 512, iniPath);
		if (strlen(buf[i]) == 0)
		{
			continue;
		}
		//CString decrypted = ciAes256Util::Decrypt(buf[i]).c_str();
		CString decrypted = buf[i];
		int pos = 0;
		m_sendData[i].alias = decrypted.Tokenize("/", pos);
		m_sendData[i].checkSMS = bool(atoi(decrypted.Tokenize("/", pos)));
		m_sendData[i].tel = decrypted.Tokenize("/", pos);
	}
}

int CfireGuardGraphDlg::SendSMS(int cameraId, int type, float threshold)
{
	if (m_checkSMS.GetCheck())
	{
		return 0;
	}

	int count = 0;
	for (int i = 0; i < MAX_DATA; i++)
	{
		if (!m_sendData[i].checkSMS)
		{
			continue;
		}
		if (m_sendData[i].tel.IsEmpty() || m_sendData[i].tel.GetLength() < 10 )
		{
			continue;
		}

		CString contents;
		if (type == VELOCITY_OVER)
		{
			contents.Format("경보 : Camera %d에서 온도가 지속적으로 상승하는 중입니다. 확인을 바랍니다.(%.1f)", cameraId, threshold);
		}
		else
		{
			contents.Format("경보 : Camera %d에서 높은 온도가 감지되었습니다.  확인을 바랍니다.(%.1f)", cameraId, threshold);
		}

		CString path = UBC_CONFIG_PATH;
		path += "sms";

		count += int(SendNaverSMS(m_smsURL, "FIRE WARNING", contents, path, m_sendData[i].tel, cameraId));
	}
	TraceLog(("%dth SMS sent", count));
	return count;
}

bool CfireGuardGraphDlg::SendNaverSMS(const char* serviceUrl, 
	const char* title, const char* content, const char* imgUrl, const char* phoneNo, int cameraId)
{
	// delete old 
	for (SMSLOGMAP::iterator jtr = _smsLogMap.begin(); jtr != _smsLogMap.end();) {
		SMSLog* alog = jtr->second;
		if (alog->isOld()) {
			delete alog;
			_smsLogMap.erase(jtr++);
		}
		else{
			++jtr;
		}
	}

	SMSLog* alog = new SMSLog(phoneNo, cameraId);
	CString key = alog->key();
	SMSLOGMAP::iterator itr = _smsLogMap.find(key);
	if (itr == _smsLogMap.end()) {
		_smsLogMap[key] = alog;
	}
	else {
		delete alog;
		alog = 0;
		alog = itr->second;
		if (alog->isOver()) {
			return false;
		}
		alog->add();
	}


	CString dataJson = "";
	dataJson.Format("{\r\n\t\"title\":\"%s\",\r\n\t\"content\":\"%s\",\r\n\t\"phone\":\"%s\"\r\n}",
		title, content, phoneNo);

	//CString utf8Str = UTF8ToANSIString(dataJson).c_str();
	CString utf8Str = ANSIToUTF8String(dataJson).c_str();

	CString jsonFile;
	jsonFile.Format("%s.json", imgUrl);

	FILE* fp = fopen(jsonFile, "wb");
	if (!fp)
	{
		TraceLog(("data.json file open error"));
		return false;
	}
	fwrite(utf8Str, 1, strlen(utf8Str), fp);
	fclose(fp);

	CString params = "--location --request POST ";
	params += serviceUrl;
	params += " --form \"dataJson=@";
	params += jsonFile;
	params += "\"";
	/*if (imgUrl && strlen(imgUrl) > 0)
	{
		params += " --form \"imgUrl=@";
		params += imgUrl;
		params += "\"";
	}*/
	TraceLog(("skpark face curl %s", params));

	CString strRetData = RunCLI("C:\\Windows\\System32\\", "curl.exe", params);
	TraceLog(("_SendNaverSMS:%s", strRetData));

	//::DeleteFile(jsonFile);

	if (strRetData.Find("true") >= 0)
	{
		return true;
	}
	return false;
}

void CfireGuardGraphDlg::OnBnClickedCheckSendMsg()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	int stopSMS = m_checkSMS.GetCheck();

	CString buf;
	buf.Format("%d", stopSMS);
	WritePrivateProfileString("FIRE_WATCH", "CHECK_DONT_SMS", buf, iniPath);
}

void CfireGuardGraphDlg::SetCounter(int timeCounter, double devi)
{
	//CString buf;
	//buf.Format(" FREQ : %d , Dev : %.2f", timeCounter, devi);
	//m_stCounter.SetWindowTextA(buf);
}

void CfireGuardGraphDlg::OnBnClickedCheck1() {	userCheck[0] = true; }
void CfireGuardGraphDlg::OnBnClickedCheck2() {	userCheck[1] = true; }
void CfireGuardGraphDlg::OnBnClickedCheck3() {	userCheck[2] = true; }
void CfireGuardGraphDlg::OnBnClickedCheck4() {	userCheck[3] = true; }
void CfireGuardGraphDlg::OnBnClickedCheck5() {	userCheck[4] = true; }
void CfireGuardGraphDlg::OnBnClickedCheck6() {	userCheck[5] = true; }
void CfireGuardGraphDlg::OnBnClickedCheck7() {	userCheck[6] = true; }
void CfireGuardGraphDlg::OnBnClickedCheck8() {	userCheck[7] = true; }


void CfireGuardGraphDlg::OnBnClickedButtonLogDurationApply()
{
	CString iniPath = UBC_CONFIG_PATH;
	iniPath += UBCBRW_INI;

	CString buf;
	m_editLogDuration.GetWindowText(buf);

	WritePrivateProfileStringA("FIRE_WATCH", "LOG_DURATION", buf, iniPath);
	m_logDuration = atoi(buf);

}


void CfireGuardGraphDlg::OnBnClickedBtShow1()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 0, 0); // popup 으로 화면을 띠울것을 요청한다.
}
void CfireGuardGraphDlg::OnBnClickedBtShow2()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 1, 0); // popup 으로 화면을 띠울것을 요청한다.
}
void CfireGuardGraphDlg::OnBnClickedBtShow3()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 2, 0); // popup 으로 화면을 띠울것을 요청한다.
}
void CfireGuardGraphDlg::OnBnClickedBtShow4()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 3, 0); // popup 으로 화면을 띠울것을 요청한다.
}
void CfireGuardGraphDlg::OnBnClickedBtShow5()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 4, 0); // popup 으로 화면을 띠울것을 요청한다.
}
void CfireGuardGraphDlg::OnBnClickedBtShow6()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 5, 0); // popup 으로 화면을 띠울것을 요청한다.
}
void CfireGuardGraphDlg::OnBnClickedBtShow7()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 6, 0); // popup 으로 화면을 띠울것을 요청한다.
}
void CfireGuardGraphDlg::OnBnClickedBtShow8()
{
	preSocketHandler::getInstance()->pushCommand("POPUP", 7, 0); // popup 으로 화면을 띠울것을 요청한다.
}
