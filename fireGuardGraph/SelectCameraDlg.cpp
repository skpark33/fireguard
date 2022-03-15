// SelectCameraDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "commctrl.h"
#include "fireGuardGraph.h"
#include "SelectCameraDlg.h"
#include "afxdialogex.h"
#include "skpark/TraceLog.h"



// CSelectCameraDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSelectCameraDlg, CDialogEx)

CSelectCameraDlg::CSelectCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectCameraDlg::IDD, pParent)
{
	prefix = "fireGuardGraph";
	//m_dateTimePicker = COleDateTime::GetCurrentTime();
	m_dateTimePicker.SetTime(COleDateTime::GetCurrentTime());
}

CSelectCameraDlg::~CSelectCameraDlg()
{
}

void CSelectCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO9, m_radioAll);
	DDX_Control(pDX, IDC_CHECK_DATE, m_checkDate);

	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateTimePicker);
}


BEGIN_MESSAGE_MAP(CSelectCameraDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CSelectCameraDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSelectCameraDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CSelectCameraDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CSelectCameraDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CSelectCameraDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CSelectCameraDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CSelectCameraDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CSelectCameraDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CSelectCameraDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CSelectCameraDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &CSelectCameraDlg::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_CHECK_DATE, &CSelectCameraDlg::OnBnClickedDate)
	ON_BN_CLICKED(IDOK, &CSelectCameraDlg::OnBnClickedOk)

END_MESSAGE_MAP()


// CSelectCameraDlg 메시지 처리기입니다.


BOOL CSelectCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_RADIO1)->EnableWindow(enabled[0]);
	GetDlgItem(IDC_RADIO2)->EnableWindow(enabled[1]);
	GetDlgItem(IDC_RADIO3)->EnableWindow(enabled[2]);
	GetDlgItem(IDC_RADIO4)->EnableWindow(enabled[3]);
	GetDlgItem(IDC_RADIO5)->EnableWindow(enabled[4]);
	GetDlgItem(IDC_RADIO6)->EnableWindow(enabled[5]);
	GetDlgItem(IDC_RADIO7)->EnableWindow(enabled[6]);
	GetDlgItem(IDC_RADIO8)->EnableWindow(enabled[7]);

	m_radioAll.SetCheck(true);
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSelectCameraDlg::OnBnClickedRadio1() { checked = 1; }
void CSelectCameraDlg::OnBnClickedRadio2() { checked = 2; }
void CSelectCameraDlg::OnBnClickedRadio3() { checked = 3; }
void CSelectCameraDlg::OnBnClickedRadio4() { checked = 4; }
void CSelectCameraDlg::OnBnClickedRadio5() { checked = 5; }
void CSelectCameraDlg::OnBnClickedRadio6() { checked = 6; }
void CSelectCameraDlg::OnBnClickedRadio7() { checked = 7; }
void CSelectCameraDlg::OnBnClickedRadio8() { checked = 8; }

void CSelectCameraDlg::OnBnClickedRadio9() { prefix = "fireGuardGraph"; }
void CSelectCameraDlg::OnBnClickedRadio10() { prefix = "Alarm"; }
void CSelectCameraDlg::OnBnClickedRadio11() { prefix = "Trend"; }

void CSelectCameraDlg::OnBnClickedDate() 
{ 
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(m_checkDate.GetCheck() == 1);
}


void
CSelectCameraDlg::OnBnClickedOk()
{
	if (m_checkDate.GetCheck() == true) {

		COleDateTime  currOleDate;
		m_dateTimePicker.GetTime(currOleDate);
		dateTime = currOleDate.Format("%Y%m%d");
		//dateTime.Format("%04d%02d%02d", m_dateTimePicker.GetYear(), m_dateTimePicker.GetMonth(), m_dateTimePicker.GetDay());
		
	}
	else{
		dateTime = "";
	}
	CDialogEx::OnOK();
}