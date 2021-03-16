// DlgCustomControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgCustomControl.h"
#include "afxdialogex.h"


// CDlgCustomControl �Ի���

IMPLEMENT_DYNAMIC(CDlgCustomControl, CDialogEx)

CDlgCustomControl::CDlgCustomControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCustomControl::IDD, pParent)
{

	m_strClassName = _T("");
}

CDlgCustomControl::~CDlgCustomControl()
{
}

void CDlgCustomControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strClassName);
	DDX_Control(pDX, IDC_COMBO1, m_cbParentControl);
}


BEGIN_MESSAGE_MAP(CDlgCustomControl, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCustomControl::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgCustomControl::OnChangeEdit1)
END_MESSAGE_MAP()


// CDlgCustomControl ��Ϣ�������


BOOL CDlgCustomControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	xml_node root = g_duiProp.GetRoot();
	for (xml_node node=root.first_child(); node; node=node.next_sibling())
	{
		if(g_duiProp.IsControlUI(node))
		{
			m_cbParentControl.AddString(XML2T(node.name()));
		}
	}
	m_cbParentControl.SelectString(-1, _T("Container"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgCustomControl::OnBnClickedOk()
{
	if(!UpdateData(TRUE))	return;

	m_cbParentControl.GetLBText(m_cbParentControl.GetCurSel(), m_strParentControl);

	if(m_strParentControl.IsEmpty())
		return;

	if(m_strClassName.IsEmpty())
	{
		AfxMessageBox(_T("�������Զ���ؼ�������."));
		return;
	}

	CDialogEx::OnOK();
}


void CDlgCustomControl::OnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
