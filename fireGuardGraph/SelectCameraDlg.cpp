// SelectCameraDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "fireGuardGraph.h"
#include "SelectCameraDlg.h"
#include "afxdialogex.h"


// CSelectCameraDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSelectCameraDlg, CDialogEx)

CSelectCameraDlg::CSelectCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectCameraDlg::IDD, pParent)
{

}

CSelectCameraDlg::~CSelectCameraDlg()
{
}

void CSelectCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
END_MESSAGE_MAP()


// CSelectCameraDlg �޽��� ó�����Դϴ�.


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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSelectCameraDlg::OnBnClickedRadio1() { checked = 1; }
void CSelectCameraDlg::OnBnClickedRadio2() { checked = 2; }
void CSelectCameraDlg::OnBnClickedRadio3() { checked = 3; }
void CSelectCameraDlg::OnBnClickedRadio4() { checked = 4; }
void CSelectCameraDlg::OnBnClickedRadio5() { checked = 5; }
void CSelectCameraDlg::OnBnClickedRadio6() { checked = 6; }
void CSelectCameraDlg::OnBnClickedRadio7() { checked = 7; }
void CSelectCameraDlg::OnBnClickedRadio8() { checked = 8; }
