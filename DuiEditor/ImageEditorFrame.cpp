// ImageEditorFrame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorFrame.h"

#include "ImageEditor.h"
#include "MainFrm.h"
#include "UIManager.h"
//////////////////////////////////////////////////////////////////////////
// CImageEditorFrame

IMPLEMENT_DYNCREATE(CImageEditorFrame, CFrameWndEx)

CImageEditorFrame::CImageEditorFrame()
{
	m_bPreview = TRUE;	
	m_bTrackerDest = TRUE;
	m_bTrackSource = TRUE;
}

CImageEditorFrame::~CImageEditorFrame()
{
}


BEGIN_MESSAGE_MAP(CImageEditorFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, &CImageEditorFrame::OnIdleUpdateCmdUI)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_TB_EXIT, &CImageEditorFrame::OnTbExit)
	ON_UPDATE_COMMAND_UI(ID_TB_EXIT, &CImageEditorFrame::OnUpdateTbExit)
	ON_COMMAND(ID_TB_SAVE, &CImageEditorFrame::OnTbSave)
	ON_UPDATE_COMMAND_UI(ID_TB_SAVE, &CImageEditorFrame::OnUpdateTbSave)
	ON_COMMAND(ID_TB_UI_PREVIEW, &CImageEditorFrame::OnTbUiPreview)
	ON_UPDATE_COMMAND_UI(ID_TB_UI_PREVIEW, &CImageEditorFrame::OnUpdateTbUiPreview)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND(ID_TB_IMAGE_EDITOR_DEST, &CImageEditorFrame::OnTbImageEditorDest)
	ON_UPDATE_COMMAND_UI(ID_TB_IMAGE_EDITOR_DEST, &CImageEditorFrame::OnUpdateTbImageEditorDest)
	ON_COMMAND(ID_TB_IMAGE_EDITOR_SOURCE, &CImageEditorFrame::OnTbImageEditorSource)
	ON_UPDATE_COMMAND_UI(ID_TB_IMAGE_EDITOR_SOURCE, &CImageEditorFrame::OnUpdateTbImageEditorSource)
	ON_COMMAND(ID_TB_IMAGE_EDITOR_CORNER, &CImageEditorFrame::OnTbImageEditorCorner)
	ON_UPDATE_COMMAND_UI(ID_TB_IMAGE_EDITOR_CORNER, &CImageEditorFrame::OnUpdateTbImageEditorCorner)
END_MESSAGE_MAP()


// CImageEditorFrame ��Ϣ�������


int CImageEditorFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CImageEditor *pDlgMain = (CImageEditor *)GetParent();

	// ������ͼ
	//////////////////////////////////////////////////////////////////////////
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar.LoadToolBar(IDR_TB_IMAGE_EDITOR))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	m_wndToolBar.EnableTextLabels(TRUE);

	if (!m_wndPaneAdjust.Create(_T("����"), this, CRect(0, 0, 100, 180), TRUE, ID_OUTPUT_WND, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM,
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // δ�ܴ���
	}

	if (!m_wndList.Create(_T("�ļ�"), this, 
		CRect(0, 0, 350, 500), TRUE, ID_VIEW_CONTROL, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // δ�ܴ���
	}

	if (!m_wndImage.Create(_T("ԭʼͼƬ"), this, 
		CRect(0, 0, 350, 200), TRUE, ID_PROPERTY_WND, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // δ�ܴ���
	}
	m_wndImage.SetCaptionText(m_wndImage.m_strCaption);

	if (!m_wndProperty.Create(_T("����"), this, 
		CRect(0, 0, 300, 200), TRUE, ID_PROPERTY_WND, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT, 
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // δ�ܴ���
	}
	m_pPropList = m_wndProperty.CreatePropList();
	m_pPropList->SetOwner(this);

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndList.EnableDocking(CBRS_ALIGN_ANY);
	m_wndImage.EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperty.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPaneAdjust.EnableDocking(CBRS_ALIGN_BOTTOM);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndList);
	DockPane(&m_wndImage);
	DockPane(&m_wndProperty);
	DockPane(&m_wndPaneAdjust);

	m_wndImage.DockToWindow(&m_wndList, CBRS_BOTTOM);
	SetPaneHeight(200);

	m_wndView.m_pManager = g_pEditorImage->GetUIManager()->GetManager();

	m_pPropList->InitProp(g_pEditorImage->m_nodedata);
	m_wndProperty.OnExpandAllProperties();

	m_wndPaneAdjust.m_pForm->InitData();

	m_wndView.InitData();
	m_wndView.SetFocus();

	SetTimer(1, 10, NULL);
	return 0;
}

void CImageEditorFrame::SetPaneHeight(int nHeight)
{
	BOOL bLeftBar = FALSE;
	BOOL bAloneInContainer = FALSE;
	CPaneContainer* pContainer = NULL;
	CPaneDivider* pDivider = NULL;

	BOOL bResult = FindInternalDivider (&m_wndImage, pContainer, pDivider, bLeftBar, bAloneInContainer);

	if (bAloneInContainer)
	{
		MessageBox (_T ("DialogBar is docked alone in container."));
		return;
	}
	if (!bResult || pContainer == NULL || pDivider == NULL)
	{
		AfxMessageBox (_T ("The DialogBar is not docked."));
		return;
	}

	if (!pDivider->IsHorizontal ())
	{
		MessageBox (_T ("Internal pane divider is vertical, can change width only."));
		return;
	}

	CPaneContainer* pRootContainer = m_wndImage.GetDefaultPaneDivider ()->FindPaneContainer (&m_wndImage, bLeftBar);	

	while (pRootContainer->GetParentPaneContainer () != NULL)
	{
		pRootContainer = pRootContainer->GetParentPaneContainer ();
	}

	CRect rectContainer;
	pRootContainer->GetWindowRect (rectContainer, FALSE);

	if ((UINT)rectContainer.Height () - 4 < nHeight)
	{
		CString strFormat = _T ("Required height exceeds allowed height in the current container.\nAllowed height is %u pixels.");
		CString strMessage; 
		strMessage.Format (strFormat, rectContainer.Height () - 4);
		MessageBox (strMessage);
		return;
	}


	CRect rectDlgBar;
	m_wndImage.GetWindowRect (rectDlgBar);

	CPoint ptOffset (0, 0);
	ptOffset.y = rectDlgBar.Height () - nHeight;
	if (bLeftBar)
	{
		ptOffset.y = -ptOffset.y;
	}
	pDivider->Move (ptOffset);
}

BOOL CImageEditorFrame::FindInternalDivider (CDockablePane* pBar, 
	CPaneContainer*& pContainer, 
	CPaneDivider*& pDivider,
	BOOL& bLeftBar, 
	BOOL& bAloneInContainer)
{
	pContainer = NULL;
	pDivider = NULL;
	bLeftBar = FALSE;
	bAloneInContainer = FALSE;

	CPaneDivider* pDefaultPaneDivider = pBar->GetDefaultPaneDivider ();
	CMultiPaneFrameWnd* pParentMiniFrame = 
		DYNAMIC_DOWNCAST (CMultiPaneFrameWnd, pBar->GetParentMiniFrame ()) ;

	CPaneContainer* pStartContainer = NULL;

	if (pParentMiniFrame != NULL)
	{
		CPaneContainerManager& manager = pParentMiniFrame->GetPaneContainerManager ();
		pStartContainer = manager.FindPaneContainer (pBar, bLeftBar);
	}
	else if (pDefaultPaneDivider != NULL)
	{
		pStartContainer = pDefaultPaneDivider->FindPaneContainer (pBar, bLeftBar);
	}
	else
	{
		return FALSE;
	}

	if (pStartContainer != NULL)
	{
		pContainer = pStartContainer;

		while (pContainer != NULL)
		{
			if (!pContainer->IsRightPartEmpty (TRUE) && 
				!pContainer->IsLeftPartEmpty (TRUE))
			{
				break;	
			}
			pContainer = pContainer->GetParentPaneContainer ();
		}

		if (pContainer == NULL)
		{
			bAloneInContainer = TRUE;
			return FALSE;
		}
		else
		{
			pDivider = (CPaneDivider*) pContainer->GetPaneDivider ();

			bLeftBar = (pContainer->IsLeftPane (pBar) || 
				pContainer->GetLeftPaneContainer () != NULL && 
				((CPaneContainer*)pContainer->GetLeftPaneContainer ())-> 
				FindSubPaneContainer (pBar, CPaneContainer::BC_FIND_BY_LEFT_BAR) != NULL);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CImageEditorFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CFrameWndEx::OnEraseBkgnd(pDC);
}

BOOL CImageEditorFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;


	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LRESULT CImageEditorFrame::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	if(m_wndToolBar.GetSafeHwnd())
	{
		m_wndToolBar.SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
	}
	if(m_wndList.GetSafeHwnd())
	{
		m_wndList.SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
		m_wndList.m_wndToolBar.SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
	}
	CFrameWnd::OnIdleUpdateCmdUI();
	return 0;
}

void CImageEditorFrame::OnSelectedFile(LPCTSTR lpstrPathName)
{
	CImageEditor *pDlgMain = (CImageEditor *)GetParent();

	CString strDocPath = pDlgMain->GetUIManager()->GetDocument()->GetSkinPath(); //CPaintManagerUI::GetResourcePath();
	CString strFileName = lpstrPathName;
	
	//ֻ��ȡ��Ŀ¼���ļ�
	if(strFileName.Find(strDocPath) == 0)
	{
		strFileName = strFileName.Right(strFileName.GetLength() - strDocPath.GetLength());
	}
	else
	{
		::MessageBoxA(NULL, "ֻ��ʹ�õ�ǰĿ¼������Ŀ¼���ļ�", "ERROR", MB_OK);
		return;
	}

	g_pEditorImage->SetImageFile(lpstrPathName);
	m_wndView.InitData();
	m_wndImage.m_pView->InitData();

	m_pPropList->InitProp(g_pEditorImage->m_nodedata);
	m_wndProperty.OnExpandAllProperties();

	ParentPreview();
}

LRESULT CImageEditorFrame::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	
	m_wndPaneAdjust.m_pForm->InitData();
	m_wndView.InitData();
	m_wndImage.m_pView->InitData();
	ParentPreview();

	return 0;
}

void CImageEditorFrame::ParentPreview()
{
	if(m_bPreview)
	{
		//ˢ�¸�����Ԥ��
		if(g_pEditorImage->m_pParentGrid && g_pEditorImage->m_pParentProp)
		{
			g_pEditorImage->m_pParentProp->SetOriginalValue(g_pEditorImage->m_pParentProp->GetValue());
			g_pEditorImage->m_pParentProp->SetValue(_variant_t(g_pEditorImage->GetAttributeValue()));
			g_pEditorImage->m_pParentGrid->OnPropertyChanged(g_pEditorImage->m_pParentProp);
		}
	}
}

void CImageEditorFrame::OnTbExit()
{
	CImageEditor *pDlgMain = (CImageEditor *)GetParent();
	pDlgMain->SendMessage(WM_COMMAND, IDCANCEL, 0);
}


void CImageEditorFrame::OnUpdateTbExit(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CImageEditorFrame::OnTbSave()
{
	CImageEditor *pDlgMain = (CImageEditor *)GetParent();
	pDlgMain->SendMessage(WM_COMMAND, IDOK, 0);
}


void CImageEditorFrame::OnUpdateTbSave(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CImageEditorFrame::OnTbUiPreview()
{
	m_bPreview = !m_bPreview;
}


void CImageEditorFrame::OnUpdateTbUiPreview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bPreview);
}


void CImageEditorFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	delete m_pPropList;
}


void CImageEditorFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent == 1)
	{
		m_wndList.OnExplorerViewsLargeIcon();
		KillTimer(nIDEvent);
	}
	CFrameWndEx::OnTimer(nIDEvent);
}


void CImageEditorFrame::OnTbImageEditorDest()
{
	m_bTrackerDest = !m_bTrackerDest;
	m_wndImage.m_pView->Invalidate();
	Invalidate();
}


void CImageEditorFrame::OnUpdateTbImageEditorDest(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bTrackerDest);
}


void CImageEditorFrame::OnTbImageEditorSource()
{
	m_bTrackSource = TRUE;
	m_wndImage.m_pView->InitData();
}


void CImageEditorFrame::OnUpdateTbImageEditorSource(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bTrackSource);
}


void CImageEditorFrame::OnTbImageEditorCorner()
{
	m_bTrackSource = FALSE;
	m_wndImage.m_pView->InitData();
}


void CImageEditorFrame::OnUpdateTbImageEditorCorner(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bTrackSource);
}
