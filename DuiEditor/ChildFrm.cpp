
// ChildFrm.cpp : CChildFrame ���ʵ��
//

#include "stdafx.h"
#include "DuiEditor.h"

#include "ChildFrm.h"
#include "MainFrm.h"
#include "DuiEditorViewCode.h"
#include "UIManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_DESIGNER_VIEW, &CChildFrame::OnDesignerView)
	ON_UPDATE_COMMAND_UI(ID_DESIGNER_VIEW, &CChildFrame::OnUpdateDesignerView)
	ON_COMMAND(ID_DESIGNER_CODE, &CChildFrame::OnDesignerCode)
	ON_UPDATE_COMMAND_UI(ID_DESIGNER_CODE, &CChildFrame::OnUpdateDesignerCode)
	ON_COMMAND(ID_DESIGNER_SPLIT_UPDOWN, &CChildFrame::OnDesignerSplitUpdown)
	ON_UPDATE_COMMAND_UI(ID_DESIGNER_SPLIT_UPDOWN, &CChildFrame::OnUpdateDesignerSplitUpdown)
	ON_COMMAND(ID_DESIGNER_SPLIT_LEFTRIGHT, &CChildFrame::OnDesignerSplitLeftright)
	ON_UPDATE_COMMAND_UI(ID_DESIGNER_SPLIT_LEFTRIGHT, &CChildFrame::OnUpdateDesignerSplitLeftright)
END_MESSAGE_MAP()

// CChildFrame ����/����

CChildFrame::CChildFrame()
{
	m_pUIManager = NULL;
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
// CChildFrame ��Ϣ�������


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// TODO: �ڴ˴������Ϣ����������
	if(bActivate)
	{
		//InsertMsg(_T("CChildFrame::OnMDIActivate"));

		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		CDuiEditorDoc* pDoc = (CDuiEditorDoc *)GetActiveDocument();
		pMain->m_wndControl.SetActiveTreeView(pDoc->GetUIManager()->GetTreeView());
		pMain->m_wndProperty.SetActivePropList(pDoc->GetUIManager()->GetPropList());

		pMain->m_wndStatusBar.Invalidate();

		CView *pActiveView = GetActiveView();
		if(GetUIManager()->GetCodeView() == pActiveView && GetUIManager()->GetSplitterMode() == SPLIT_CODE)
		{
			pMain->HideAllPane();
		}
		else
		{
			pMain->ShowAllPane();
		}
	}
}


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	return 0;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_split.CreateStatic(this, 2, 1);
	m_split.SetScrollStyle(m_split.GetScrollStyle() & (~(WS_HSCROLL|WS_VSCROLL)) );
	m_split.CreateView(0,0, RUNTIME_CLASS(CDuiEditorViewDesign), CSize(0,300), pContext);
	m_split.CreateView(1,0, RUNTIME_CLASS(CDuiEditorViewCode), CSize(0,0), pContext);

	CDuiEditorDoc *pDoc = (CDuiEditorDoc *)pContext->m_pCurrentDoc;
	SetUIManager(pDoc->GetUIManager());
	return TRUE;
	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}


void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	
}


void CChildFrame::OnDesignerView()
{
	if(!GetUIManager()->GetCodeView()->ApplyDocument()) //UpdateDocument())
		return;

	GetUIManager()->SetSplitterMode(SPLIT_DESIGN);

	m_split.SetViewMode(SPLIT_DESIGN);
	m_split.RecalcLayout();

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->ShowAllPane();
}


void CChildFrame::OnUpdateDesignerView(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_split.GetViewMode() == SPLIT_DESIGN);
}


void CChildFrame::OnDesignerCode()
{
	GetUIManager()->SetSplitterMode(SPLIT_CODE);

	m_split.SetViewMode(SPLIT_CODE);
	m_split.RecalcLayout();


	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->HideAllPane();
}


void CChildFrame::OnUpdateDesignerCode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_split.GetViewMode() == SPLIT_CODE);
}

void CChildFrame::OnDesignerSplitUpdown()
{
	if(!GetUIManager()->GetCodeView()->ApplyDocument()) //UpdateDocument())
		return;

	GetUIManager()->SetSplitterMode(SPLIT_UPDOWN);

	m_split.SetViewMode(SPLIT_UPDOWN);
	m_split.RecalcLayout();

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->ShowAllPane();
}


void CChildFrame::OnUpdateDesignerSplitUpdown(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_split.GetViewMode() == SPLIT_UPDOWN);
}


void CChildFrame::OnDesignerSplitLeftright()
{
	if(!GetUIManager()->GetCodeView()->ApplyDocument()) //UpdateDocument())
		return;

	GetUIManager()->SetSplitterMode(SPLIT_LEFTRIGHT);

	m_split.SetViewMode(SPLIT_LEFTRIGHT);
	m_split.RecalcLayout();

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->ShowAllPane();
}


void CChildFrame::OnUpdateDesignerSplitLeftright(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_split.GetViewMode() == SPLIT_LEFTRIGHT);
}
