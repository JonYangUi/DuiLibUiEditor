// ImageEditorView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorView.h"

#include "ImageEditor.h"
#include "UITracker.h"
// CImageEditorView

IMPLEMENT_DYNCREATE(CImageEditorView, CScrollView)

CImageEditorView::CImageEditorView()
{
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_pManager = NULL;
	m_zoom = 1;

	m_tracker.m_rect = CRect(0,0,0,0);
	m_tracker.m_nStyle = 0;
	m_tracker.m_nStyle |= CRectTracker::dottedLine;
	m_tracker.m_nStyle |= CRectTracker::resizeOutside;
}

CImageEditorView::~CImageEditorView()
{
}


BEGIN_MESSAGE_MAP(CImageEditorView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CImageEditorView ��ͼ

void CImageEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageEditorView::OnDraw(CDC* pDC)
{
	CMemDCEx memDC(pDC);

	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcArea = rcClient;
	rcArea.OffsetRect(GetScrollPosition());
	memDC->FillSolidRect(rcArea, g_cfg.crDesignerBkColor);

	if(!m_pManager)
		return;

	CImageEditor *pDlgMain = (CImageEditor *)GetParent()->GetParent();
	
	if(pDlgMain->m_imgControlX.IsEnabled() && pDlgMain->m_imgControlX.IsValid())
	{
		pDlgMain->m_imgControlX.Draw(memDC->m_hDC, m_rcControl);
	}
	else
	{
		CBrush* pOldBrush=memDC->SelectObject(&afxGlobalData.brBlack);
		memDC->Rectangle(m_rcControl);
	}

#ifndef DUILIB_VERSION_ORIGINAL
	CRenderEngine::DrawImageInfo(memDC->m_hDC, m_pManager, m_rcControl, m_rcControl, &m_drawInfo, AfxGetResourceHandle());
#else
	CRenderEngine::DrawImage(memDC->m_hDC, m_pManager, m_rcControl, m_rcControl, m_drawInfo);
#endif

	if(g_pEditorImage->m_pFrame->m_bTrackerDest)
		m_tracker.Draw(&memDC);
}

// CImageEditorView ��Ϣ�������
BOOL CImageEditorView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}

void CImageEditorView::InitData()
{
	xml_node node = g_pEditorImage->m_nodedata;
	CString strImageInfo, temp;
	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		temp.Format(_T("%s='%s' "), XML2T(attr.name()), XML2T(attr.value()));
		strImageInfo += temp;
	}

	CString file = XML2T(node.attribute("file").as_string());
	if(!file.IsEmpty())
	{
		m_pManager->RemoveImage(file);
	}

	m_drawInfo.Clear();
#ifdef DUILIB_VERSION_ORIGINAL
	m_drawInfo.sDrawString = strImageInfo;
	m_drawInfo.bLoaded = false;
#else
	m_drawInfo.Parse(strImageInfo, NULL, m_pManager);
#endif

	if(!g_pEditorImage->m_rcControl.IsRectEmpty())
	{
		CSize sizeTotal;
		sizeTotal.cx = g_pEditorImage->m_rcControl.Width() + 20;
		sizeTotal.cy = g_pEditorImage->m_rcControl.Height() + 20;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

	RecalcImageRect();
}


void CImageEditorView::RecalcImageRect()
{
	CRect rcControl = g_pEditorImage->m_rcControl;
	if(rcControl.IsRectEmpty())
		rcControl = g_pEditorImage->m_rcImage;

	CRect rcClient;
	GetClientRect(&rcClient);
	CPoint pt = rcClient.CenterPoint();
	CRect rcTemp = rcControl;
	if(rcClient.Width() > rcControl.Width())
	{
		rcControl.left = pt.x - rcTemp.Width()/2;
		rcControl.right = pt.x + rcTemp.Width()/2;
	}
	if(rcClient.Height() > rcControl.Height())
	{
		rcControl.top = pt.y - rcTemp.Height()/2;
		rcControl.bottom = pt.y + rcTemp.Height()/2;
	}
	m_rcControl = rcControl;

	//ѡȡdest��
	xml_attribute attrDest = g_pEditorImage->m_nodeImage.child(XTEXT("IMAGE")).attribute(XTEXT("dest"));
	if(attrDest)
	{
		CDuiRect rc(XML2T(attrDest.as_string(XTEXT("0,0,0,0"))));
		m_tracker.m_rect = rc;
		m_tracker.m_rect.OffsetRect(m_rcControl.left, m_rcControl.top);
		CPoint pt = GetScrollPosition();
		m_tracker.m_rect.OffsetRect(-pt.x, -pt.y);
	}
	else
	{
		m_tracker.m_rect = m_rcControl;
		CPoint pt = GetScrollPosition();
		m_tracker.m_rect.OffsetRect(-pt.x, -pt.y);
	}	

	Invalidate();
}

void CImageEditorView::OnChangeRect()
{
	if(!g_pEditorImage->m_pFrame->m_bTrackerDest)
	{
		return;
	}

	//ѡȡdest��

	CRect rc = m_tracker.m_rect;
	rc.OffsetRect(-m_rcControl.left, -m_rcControl.top);
	rc.OffsetRect(GetScrollPosition());

	g_duiProp.AddAttribute(g_pEditorImage->m_nodedata, _T("dest"), RectToString(rc), NULL);

	g_pEditorImage->m_pFrame->m_wndView.InitData();
	g_pEditorImage->m_pFrame->m_pPropList->InitProp(g_pEditorImage->m_nodedata);
	g_pEditorImage->m_pFrame->m_wndProperty.OnExpandAllProperties();
	g_pEditorImage->m_pFrame->m_wndPaneAdjust.m_pForm->InitData();

	g_pEditorImage->m_pFrame->ParentPreview();

	Invalidate();
}

BOOL CImageEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(zDelta < 0)
	{
		m_zoom -= 0.2f;
		if(m_zoom <= 0.2f)
			m_zoom = 0.2f;
	}
	else
	{
		m_zoom += 0.2f;
	}
	Invalidate();
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CImageEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!g_pEditorImage->m_pFrame->m_bTrackerDest)
	{
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	SetFocus();

	BOOL bNewRect = FALSE;

	int htTest = m_tracker.HitTest(point);
	if(htTest > 0 )
	{
		if(m_tracker.Track(this, point, TRUE))
		{
			m_tracker.m_rect.NormalizeRect();

			switch (htTest)
			{
			case CRectTracker::hitTopLeft:
				//break;
			case CRectTracker::hitTopRight:
				//break;
			case CRectTracker::hitBottomRight:
				//break; 
			case CRectTracker::hitBottomLeft:
				//break;
			case CRectTracker::hitTop:
				//break;
			case CRectTracker::hitRight:
				//break; 
			case CRectTracker::hitBottom:
				//break;
			case CRectTracker::hitLeft:
				//break; 
			case CRectTracker::hitMiddle: //�ƶ�
				bNewRect = TRUE;
				break;
			}	
		}
	}
	else
	{
		CRectTracker tracker;
		if(tracker.TrackRubberBand(this, point))
		{
			tracker.m_rect.NormalizeRect();
			m_tracker.m_rect = tracker.m_rect;
			bNewRect = TRUE;
		}
	}
	
	if(bNewRect)
	{
		OnChangeRect();
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageEditorView::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	//CScrollView::PostNcDestroy();
}

BOOL CImageEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (g_pEditorImage->m_pFrame->m_bTrackerDest && m_tracker.SetCursor( pWnd, nHitTest )) 
		return TRUE;
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CImageEditorView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(GetSafeHwnd() == NULL) return;
	RecalcImageRect();
}


void CImageEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(!g_pEditorImage->m_pFrame->m_bTrackerDest)
	{
		CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	switch(nChar)
	{
	case VK_UP:
		m_tracker.m_rect.OffsetRect(0,-1);
		OnChangeRect();
		break;
	case VK_DOWN:
		m_tracker.m_rect.OffsetRect(0,1);
		OnChangeRect();
		break;
	case VK_LEFT:
		m_tracker.m_rect.OffsetRect(-1,0);
		OnChangeRect();
		break;
	case VK_RIGHT:
		m_tracker.m_rect.OffsetRect(1,0);
		OnChangeRect();
		break;
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CImageEditorView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: �ڴ����ר�ô����/����û���
	RecalcImageRect();
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}
