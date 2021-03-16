// DlgInsertControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgInsertControl.h"
#include "afxdialogex.h"


// CDlgInsertControl �Ի���

IMPLEMENT_DYNAMIC(CDlgInsertControl, CDialogEx)

CDlgInsertControl::CDlgInsertControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInsertControl::IDD, pParent)
{
	m_nPosition = 0;
}

CDlgInsertControl::~CDlgInsertControl()
{
}

void CDlgInsertControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInsertControl, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgInsertControl::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInsertControl ��Ϣ�������


BOOL CDlgInsertControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CButton *pBtn1 = (CButton *)GetDlgItem(IDC_RADIO1);
	CButton *pBtn2 = (CButton *)GetDlgItem(IDC_RADIO2);
	CButton *pBtn3 = (CButton *)GetDlgItem(IDC_RADIO3);

	if( g_duiProp.IsContainerUI(nodeControl) || g_duiProp.IsWindowForm(nodeControl) )
	{
		pBtn1->SetCheck(TRUE);
		return TRUE;
	}

	CControlUI *pControl = (CControlUI *)nodeControl.get_tag();
	CContainerUI *pContainer = (CContainerUI *)pControl->GetInterface(DUI_CTR_CONTAINER);
	if(pContainer)
	{
		pBtn1->SetCheck(TRUE);
		return TRUE;
	}

	pBtn1->EnableWindow(FALSE);
	pBtn2->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgInsertControl::OnBnClickedOk()
{
	CButton *pBtn1 = (CButton *)GetDlgItem(IDC_RADIO1);
	CButton *pBtn2 = (CButton *)GetDlgItem(IDC_RADIO2);
	CButton *pBtn3 = (CButton *)GetDlgItem(IDC_RADIO3);

	if(pBtn1->GetCheck())
		m_nPosition = 0;
	else if(pBtn2->GetCheck())
		m_nPosition = 1;
	else if(pBtn3->GetCheck())
		m_nPosition = 2;
	
	CDialogEx::OnOK();
}
