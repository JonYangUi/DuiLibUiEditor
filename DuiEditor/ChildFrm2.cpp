
// ChildFrm2.cpp : CChildFrame2 ���ʵ��
//

#include "stdafx.h"
#include "DuiEditor.h"

#include "ChildFrm2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame2

IMPLEMENT_DYNCREATE(CChildFrame2, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame2, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildFrame2 ����/����

CChildFrame2::CChildFrame2()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CChildFrame2::~CChildFrame2()
{
}


BOOL CChildFrame2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame2 ��Ϣ�������


void CChildFrame2::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// TODO: �ڴ˴������Ϣ����������
}


int CChildFrame2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(IDR_TB_UIDESIGNER_VIEW))
// 	{
// 		TRACE0("δ�ܴ���������\n");
// 		return -1;      // δ�ܴ���
// 	}
// 	m_wndToolBar.EnableTextLabels(TRUE);
// 
//  	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_VIEW), _T("���"));
//  	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_CODE), _T("����"));

// 	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TB_UIDESIGNER_VIEW);
// 	m_wndToolBar.LoadToolBar(IDR_TB_UIDESIGNER_VIEW, 0, 0, TRUE /* ������*/);
// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
// 
// 	m_wndToolBar.SetOwner(this);
// 	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
// 	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
// 
//   	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_VIEW), _T("���"));
// 	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_CODE), _T("����"));

	return 0;
}


void CChildFrame2::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
// 	if(!m_wndToolBar.GetSafeHwnd())
// 		return;

// 	CRect rectClient;
// 	GetClientRect(rectClient);
// 	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
// 
// 	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
// 
// 	CView *pView = GetActiveView();
// 	if(pView)
// 	{
// 		pView->SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOZORDER | SWP_NOACTIVATE);
// 	}
}
