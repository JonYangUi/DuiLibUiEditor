// DlgStringEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgStringEditor.h"
#include "afxdialogex.h"


// CDlgStringEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgStringEditor, CDialogEx)

CDlgStringEditor::CDlgStringEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStringEditor::IDD, pParent)
{

}

CDlgStringEditor::~CDlgStringEditor()
{
}

void CDlgStringEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_ctrlRichEdit);
}


BEGIN_MESSAGE_MAP(CDlgStringEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgStringEditor::OnBnClickedOk)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgStringEditor ��Ϣ�������


BOOL CDlgStringEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ctrlRichEdit.SetWindowText(m_string);

	SendMessage(WM_SIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgStringEditor::OnBnClickedOk()
{
	m_ctrlRichEdit.GetWindowText(m_string);
	CDialogEx::OnOK();
}


void CDlgStringEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(!m_ctrlRichEdit.GetSafeHwnd())	return;

	CRect rcClient;
	GetClientRect(rcClient);

	CWnd *pBtn1 = GetDlgItem(IDOK);
	CWnd *pBtn2 = GetDlgItem(IDCANCEL);
	CRect rcBtn1, rcBtn2;
	int btnSpace=20;
	pBtn1->GetWindowRect(rcBtn1);
	pBtn2->GetWindowRect(rcBtn2);

	CWnd *pWndEdit = GetDlgItem(IDC_RICHEDIT21);
	pWndEdit->MoveWindow(0,0, rcClient.Width(), rcClient.Height() - rcBtn1.Height()*2);

	pBtn1->MoveWindow(rcClient.Width()-rcBtn1.Width()*2-btnSpace*2, 
		rcClient.Height()-btnSpace-rcBtn1.Height(),
		rcBtn1.Width(),
		rcBtn1.Height());

	pBtn2->MoveWindow(rcClient.Width()-rcBtn2.Width()-btnSpace, 
		rcClient.Height()-btnSpace-rcBtn1.Height(),
		rcBtn2.Width(),
		rcBtn2.Height());
}
