// fireGuardGraphDlg.h : header file
//

#pragma once

#include "ChartViewer.h"
#include <afxmt.h>
#include "resource.h"
#include <gdiplus.h>
#include "afxcmn.h"
#include "afxwin.h"
#include "LogManager.h"
#include "SMSManagerDlg.h"
#include "LogManager.h"

class preSocketSession;

class CThresholdHandler
{
public:
	CThresholdHandler(int type, CChartViewer* viewer, CEdit* maxE, 
		CAnimateCtrl* aniCtrl,  CEdit* frequency = NULL /*, CEdit* deviation = NULL */);
	virtual ~CThresholdHandler();

	void ReadConfig(LPCTSTR maxDefault, LPCTSTR minDefault);
	void WriteConfig();
	void OnStop();
	void OnApply();

	bool  RunAlarmSound(double value);
	bool	InitAnimation();

protected:
	UINT  _RunAlarmSound();

	CEdit*	m_maxEditor;
	//CEdit*	m_minEditor;
	CAnimateCtrl* m_aniCtrl;
	CChartViewer* m_ChartViewer;
	CEdit*	m_editFrequency;
	//CEdit*	m_editDeviation;
	bool   m_isAniOpen;
	bool   m_isAniPlay;

public:
	double m_thresholdMax;
	//float m_thresholdMin;
	int		m_frequency;
	//double m_deviation;
	int		m_type;
	CString	m_maxName;
	CString  m_minName;

	bool		hasAleadyPlayed() { return m_isAniPlay;  }




};



// CfireGuardGraphDlg dialog
class CfireGuardGraphDlg : public CDialog
{
// Construction
public:
	CfireGuardGraphDlg(CWnd* pParent = NULL);	// standard constructor
	~CfireGuardGraphDlg();

	static const int sampleSize = 360;
	static const int MAX_CAMERA = 8;

	static const int MIN_VELO= -50;
	static const int MAX_VELO = 200;

	static const int MIN_THRESHOLD = -50;
	static const int MAX_THRESHOLD = 500;

	//static const int MAX_DATA = MAX_CAMERA + 2;

// Dialog Data
	enum { IDD = IDD_REALTIMETRACK_DIALOG };
	CStatic	m_Value[MAX_CAMERA];
	//CStatic	m_ValueC;
	//CStatic	m_ValueB;
	//CStatic	m_ValueA;
	CChartViewer m_ChartViewer;
	CChartViewer m_VelocViewer;
	CChartViewer m_SlopeViewer;

	//XYChart *tChart;
	//XYChart *vChart;
	//XYChart *sChart;


	CButton	m_RunPB;
	CSpinButtonCtrl	m_AlarmSpin;
	CComboBox	m_CBUpdatePeriod; 
	long m_updatePeriod;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRunPB();
	afx_msg void OnFreezePB();
	afx_msg void OnSelchangeUpdatePeriod();
	afx_msg void OnViewPortChanged();
	afx_msg void OnMouseMovePlotArea();
	afx_msg void OnViewPortChangedVeloc();
	afx_msg void OnViewPortChangedSlope();
	afx_msg void OnMouseMovePlotAreaVeloc();
	afx_msg void OnMouseMovePlotAreaSlope();
	DECLARE_MESSAGE_MAP()


private:
	CStatic	m_TemperVal[MAX_CAMERA];
	double m_timeStamps[sampleSize];	// The timestamps for the data series
	double m_dataSeries[MAX_CAMERA][sampleSize];	// The values for the data series A
	double m_rateSeries[MAX_CAMERA][sampleSize];	// The values for the data series A
	double m_slopeSeries[MAX_CAMERA][sampleSize];	// The values for the data series A
	double m_countSeries[MAX_CAMERA][sampleSize];	// The values for the data series A
	//double m_dataSeriesA[sampleSize];	// The values for the data series A
	//double m_dataSeriesB[sampleSize];	// The values for the data series B
	//double m_dataSeriesC[sampleSize];	// The values for the data series C

    // The index of the array position to which new data values are added.
    int m_currentIndex;

	double m_nextDataTime;	// Used by the random number generator to generate real time data.
	int m_extBgColor;		// The default background color.

	// Shift new data values into the real time data series 
	// void getData();
	
	// Draw chart
	void drawChart(CChartViewer *viewer);
	void drawVeloc(CChartViewer *viewer);
	void drawSlope(CChartViewer *viewer);
	void trackLineLegend(XYChart *c, int mouseX);

	// utility to get default background color
	int getDefaultBgColor();
	// utility to load icon resource to a button
	void loadButtonIcon(int buttonId, int iconId, int width, int height);

public:
	CFont*	m_font;
	preSocketSession* m_session;

	LogManager*		m_AlarmLogger;
	LogManager*		m_TrendLogger;

	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

	afx_msg void OnDestroy();
	CButton m_checkCamera[MAX_CAMERA];

	afx_msg void OnBnClickedCheckAll();
	CButton m_checkCameraAll;

	CThresholdHandler*  m_temperThreshold;
	CThresholdHandler*  m_velociThreshold;
	CThresholdHandler*  m_slopeThreshold;

	CEdit m_editThresholdMax;
	CEdit m_editVelocMax;
	afx_msg void OnBnClickedButtonMaxApply();

	CAnimateCtrl m_aniCtrl;
	CAnimateCtrl m_aniVelocCtrl;
	
	afx_msg void OnBnClickedBtStop();
	afx_msg void OnBnClickedShowLog();

	void OpenLog(int cameraIdx, LPCTSTR prefix, LPCTSTR dateTime);

	afx_msg void OnBnClickedOpenFolder();
	afx_msg void OnFrequencyTimer();
	afx_msg void OnDataTimer();

	afx_msg void OnBnClickedButtonVelocApply();
	afx_msg void OnBnClickedBtVelocStop();
	CEdit m_editFrequency;

	double GetDataValue(int i, int j)
	{
		double retval = m_dataSeries[i][j];
		return (retval == _NO_VALUE_ || retval == Chart::NoValue ? 0.0f : retval);
	}
	double  Formula(int cameraId, int currentIndex, int frequency);
	bool  isIncrease(int cameraId, int currentIndex, int frequency);

	CButton m_checkStopAlarm;
	CButton m_checkStopTrend;
	afx_msg void OnBnClickedCheckAlarm();
	afx_msg void OnBnClickedCheckTrend();

	bool m_isStopAlarm;
	bool m_isStopTrend;

	CButton m_btSMS;
	afx_msg void OnBnClickedButtonSmsMgr();
	CButton m_checkSMS;
	bool m_smsAllowed;

	SendData	m_sendData[MAX_DATA];

	void LoadSendData();
	CString m_smsURL;
	int SendSMS(int cameraId, int type, float threshold);
	bool SendNaverSMS(const char* serviceUrl, const char* title, const char* content, const char* imgUrl, const char* phoneNo);

	afx_msg void OnBnClickedCheckSendMsg();

	CStatic m_stCounter;
	double m_max_temperature;

	void SetCounter(int timeCounter, double devi);

	//CEdit m_editDevi;

	bool	userCheck[MAX_CAMERA];

	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	CEdit m_editLogDuration;
	int m_logDuration;
	afx_msg void OnBnClickedButtonLogDurationApply();
	CStatic m_stVeloSec;
	CButton m_btShow[MAX_CAMERA];
	afx_msg void OnBnClickedBtShow1();
	afx_msg void OnBnClickedBtShow2();
	afx_msg void OnBnClickedBtShow3();
	afx_msg void OnBnClickedBtShow4();
	afx_msg void OnBnClickedBtShow5();
	afx_msg void OnBnClickedBtShow6();
	afx_msg void OnBnClickedBtShow7();
	afx_msg void OnBnClickedBtShow8();
};
