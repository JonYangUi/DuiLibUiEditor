// DefaultEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DefaultEditor.h"
#include "afxdialogex.h"


#include "MainFrm.h"
// CDefaultEditor �Ի���

IMPLEMENT_DYNAMIC(CDefaultEditor, CDialogEx)

CDefaultEditor::CDefaultEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDefaultEditor::IDD, pParent)
{
	m_pFrame = NULL;
}

CDefaultEditor::~CDefaultEditor()
{
}

void CDefaultEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDefaultEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDefaultEditor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDefaultEditor::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDefaultEditor ��Ϣ�������


BOOL CDefaultEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CDuiEditorViewDesign *pView = (CDuiEditorViewDesign *)pMain->GetActiveUIView();
	m_pDoc = pView->GetDocument();


// 	CString strTitle;
// 	strTitle.Format(_T("�༭ %s ����"), m_nodeImage.name());
// 	SetWindowText(strTitle);

	CRect rectDummy;
	GetClientRect(rectDummy);
	DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	m_pFrame = (CDefaultEditorFrame *)RUNTIME_CLASS(CDefaultEditorFrame)->CreateObject();
	m_pFrame->Create(NULL, _T(""), dwStyle, rectDummy, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDefaultEditor::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CDefaultEditor::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDefaultEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(!m_pFrame)	return;
	if(!m_pFrame->GetSafeHwnd())	return;
	m_pFrame->MoveWindow(0, 0, cx, cy);
}


void CDefaultEditor::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if(m_pFrame && m_pFrame->GetSafeHwnd())
	{
		m_pFrame->DestroyWindow();
	}
}


BOOL CDefaultEditor::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CDefaultEditor::SetAttributeValue(LPCTSTR szControlName, LPCTSTR szAttribute)
{
	m_strClassName = szControlName;

	CString strXML = _T("<");
	strXML += m_strClassName;
	strXML += " ";
	strXML += szAttribute;
	strXML += _T(" />");
	xml_parse_result ret = m_nodeControl.load(T2XML(strXML));
	if(ret.status != pugi::status_ok)//���ƥ�䲻�ɹ�, ����Ϊֻ���ļ���, ����������
	{
		strXML = _T("<");
		strXML += m_strClassName;
		strXML += _T(" />");
		ret = m_nodeControl.load(T2XML(strXML));
	}
}

CString CDefaultEditor::GetAttributeValue()
{
	//����Ĭ������
	g_duiProp.FilterDefaultValue(m_nodeControl.child(T2XML(m_strClassName)), NULL);
	g_duiProp.FilterPosWidthHeight(m_nodeControl.child(T2XML(m_strClassName)), NULL);

	CString strText, temp;
	xml_node root = m_nodeControl.child(T2XML(m_strClassName));
	for (xml_attribute attr=root.first_attribute(); attr; attr=attr.next_attribute())
	{
		temp.Format(_T("%s=\"%s\" "), XML2T(attr.name()), XML2T(attr.value()));
		strText += temp;
	}

	return strText;
}