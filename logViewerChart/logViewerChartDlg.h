// LogViewerChartDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ChartViewer.h"
#include "LogManager.h"
#include <gdiplus.h>

// CLogViewerChartDlg dialog
class CLogViewerChartDlg : public CDialog
{
public:
	// Constructor and Destructor
	CLogViewerChartDlg(CWnd* pParent = NULL);
	~CLogViewerChartDlg();

	static const int MAX_CAMERA = 8;
	static const int MAX_DATA = MAX_CAMERA + 2;

	// Dialog Data
	enum { IDD = IDD_VIEWPORTCONTROLDEMO_DIALOG };
	CButton m_PointerPB;
	CChartViewer m_ChartViewer;
	CViewPortControl m_ViewPortControl;
	void SetLogFile(LPCTSTR logfile, int cameraIdx) { _logFile = logfile;  _cameraIdx = cameraIdx; }

protected:
	HICON m_hIcon;

	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPointerPB();
	afx_msg void OnZoomInPB();
	afx_msg void OnZoomOutPB();
	afx_msg void OnSavePB();
	afx_msg void OnViewPortChanged();	
	afx_msg void OnMouseMovePlotArea();
	DECLARE_MESSAGE_MAP()


private:
	//
	// Data arrays for the scrollable / zoomable chart.
	// - In this demo, we just use a RanSeries object to generate random data for the chart.
	//
	RanSeries *m_ranSeries;

	CStatic	m_TemperVal[MAX_CAMERA];
	//DoubleArray m_dataSeries[MAX_DATA];	// The values for the data series A
	//DoubleArray* m_dataSeries[MAX_DATA];	// The values for the data series A
	DoubleArray* m_timeStamps;
	DoubleArray* m_dataSeries;	// The values for the data series A
	//DoubleArray m_minSeries;	// The values for the data series A
	//DoubleArray m_maxSeries;	// The values for the data series A

	//DoubleArray m_dataSeriesA;
	//DoubleArray m_dataSeriesB;
	//DoubleArray m_dataSeriesC;
	
 	// Load data into data arrays
	void loadData();
	
    // Initialize the CChartViewer
    void initChartViewer(CChartViewer *viewer);

	// Draw chart
	void drawChart(CChartViewer *viewer);
	void drawFullChart(CViewPortControl *vpc, CChartViewer *viewer);
    void trackLineLegend(XYChart *c, int mouseX);

	// utility to load icon resource to a button
	void loadButtonIcon(int buttonId, int iconId, int width, int height);

	LogManager _logger;
	CString _logFile;
	int _cameraIdx;
	//double  _thresholdMin, _thresholdMax;
public:
	afx_msg void OnBnClickedLogFolder();
	afx_msg void OnBnClickedShowAnother();

	void DrawAll();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtClose();

	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

};
