// ImageEditorImagePreview.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorImagePreview.h"

#include "UIManager.h"
#include "ImageEditor.h"
#include "ImageEditorFrame.h"
#include "ImageEditorPaneImage.h"
// CImageEditorImagePreview

IMPLEMENT_DYNCREATE(CImageEditorImagePreview, CScrollView)

CImageEditorImagePreview::CImageEditorImagePreview()
{
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_tracker.m_rect = CRect(0,0,0,0);
	m_tracker.m_nStyle = 0;
	m_tracker.m_nStyle |= CRectTracker::dottedLine;
	m_tracker.m_nStyle |= CRectTracker::resizeOutside;

	m_tracker.m_rect = CRect(0,0,0,0);
	m_tracker.m_nStyle = 0;
	m_tracker.m_nStyle |= CRectTracker::dottedLine;
	m_tracker.m_nStyle |= CRectTracker::resizeOutside;

	m_zoom = 1;
}

CImageEditorImagePreview::~CImageEditorImagePreview()
{
}


BEGIN_MESSAGE_MAP(CImageEditorImagePreview, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CImageEditorImagePreview ��ͼ
int CImageEditorImagePreview::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

void CImageEditorImagePreview::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: �������ͼ�ĺϼƴ�С

	InitData();
}

void CImageEditorImagePreview::InitData()
{
	CSize sizeTotal;
	sizeTotal.cx = g_pEditorImage->m_image.GetWidth() + 20;
	sizeTotal.cy = g_pEditorImage->m_image.GetHeight() + 20;
	SetScrollSizes(MM_TEXT, sizeTotal);

	RecalcImageRect();

	Invalidate(TRUE);

	CString strCaption;
	strCaption.Format(_T("ԭʼͼƬ  %d * %d"), g_pEditorImage->m_image.GetWidth(), g_pEditorImage->m_image.GetHeight());
	CImageEditorPaneImage *pPane = (CImageEditorPaneImage *)GetParent();
	pPane->SetCaptionText(strCaption);
}

void CImageEditorImagePreview::RecalcImageRect()
{
	//ѡȡsource
	if(g_pEditorImage->m_pFrame->m_bTrackSource)
	{
		CRect rcControl = g_pEditorImage->m_rcImage;

		//��ͼƬ������ʾ
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
		m_rcImage = rcControl;

		xml_attribute attrSource = g_pEditorImage->m_nodedata.attribute(XTEXT("source"));
		CDuiRect rc(XML2T(attrSource.as_string(XTEXT("0,0,0,0"))));
		if(rc.IsNull()) 
			rc = m_rcImage;
		else
			rc.Offset(m_rcImage.left, m_rcImage.top);

		m_tracker.m_rect = rc;
		CPoint ptpos = GetScrollPosition();
		m_tracker.m_rect.OffsetRect(-ptpos.x, -ptpos.y);
	}
	else //ѡȡconrner
	{
		xml_attribute attrDest = g_pEditorImage->m_nodedata.attribute(XTEXT("source"));
		CDuiRect rcSource(XML2T(attrDest.as_string(XTEXT("0,0,0,0"))));
		if(rcSource.IsNull()) 
		{
			rcSource = g_pEditorImage->m_rcImage;
		}
		//��source���ֵ�ͼƬ��ȡ����������corner��ѡȡ
		m_imgSource.DestroyFrames(); m_imgSource.Destroy();
		g_pEditorImage->m_image.Crop(rcSource, &m_imgSource);

		CRect rcControl = rcSource;
		//��ͼƬ������ʾ
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
		m_rcImage = rcControl;


		xml_attribute attrCorner = g_pEditorImage->m_nodedata.attribute(XTEXT("corner"));
		CDuiRect rc(XML2T(attrCorner.as_string(XTEXT("0,0,0,0"))));

		CRect rc1;
		rc1.left = m_rcImage.left + rc.left;
		rc1.right = m_rcImage.right - rc.right;
		rc1.top = m_rcImage.top + rc.top;
		rc1.bottom = m_rcImage.bottom - rc.bottom;

		m_tracker.m_rect = rc1;
		CPoint ptpos = GetScrollPosition();
		m_tracker.m_rect.OffsetRect(-ptpos.x, -ptpos.y);
	}
}

void CImageEditorImagePreview::OnChangeRect()
{
	//ѡȡsource
	if(g_pEditorImage->m_pFrame->m_bTrackSource)
	{
		CRect rc = m_tracker.m_rect;
		rc.OffsetRect(-m_rcImage.left, -m_rcImage.top);
		rc.OffsetRect(GetScrollPosition());
		g_duiProp.AddAttribute(g_pEditorImage->m_nodedata, _T("source"), RectToString(rc), NULL);
	}
	else //ѡȡconrner
	{
		xml_attribute attrSource = g_pEditorImage->m_nodedata.attribute(XTEXT("source"));
		CDuiRect rcSource(XML2T(attrSource.as_string(XTEXT("0,0,0,0"))));
		if(rcSource.IsNull()) 
			rcSource = m_rcImage;
		else
			rcSource.Offset(m_rcImage.left, m_rcImage.top);

		CRect rc = m_tracker.m_rect;
		rc.OffsetRect(GetScrollPosition());
 		CRect rc1;
		rc1.left = rc.left - m_rcImage.left;
		rc1.right = m_rcImage.right - rc.right;
		rc1.top = rc.top - m_rcImage.top;
		rc1.bottom = m_rcImage.bottom - rc.bottom;
		g_duiProp.AddAttribute(g_pEditorImage->m_nodedata, _T("corner"), RectToString(rc1), NULL);
	}

	g_pEditorImage->m_pFrame->m_wndImage.m_pView->InitData();
	g_pEditorImage->m_pFrame->m_wndView.InitData();
	g_pEditorImage->m_pFrame->m_pPropList->InitProp(g_pEditorImage->m_nodedata);
	g_pEditorImage->m_pFrame->m_wndProperty.OnExpandAllProperties();
	g_pEditorImage->m_pFrame->m_wndPaneAdjust.m_pForm->InitData();

	g_pEditorImage->m_pFrame->ParentPreview();

	Invalidate();
}

void CImageEditorImagePreview::OnDraw(CDC* pDC)
{
	CMemDCEx memDC(pDC);

	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcArea = rcClient;
	rcArea.OffsetRect(GetScrollPosition());
	memDC->FillRect(rcArea, &afxGlobalData.brBlack);

	memDC->FillSolidRect(m_rcImage, g_cfg.crDesignerBkColor);

	if(g_pEditorImage->m_pFrame->m_bTrackSource)
	{
		g_pEditorImage->m_image.Draw(memDC->m_hDC, m_rcImage);
	}
	else
	{
		m_imgSource.Draw(memDC->m_hDC, m_rcImage);
	}

	m_tracker.Draw(&memDC);
}

BOOL CImageEditorImagePreview::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}


// CImageEditorImagePreview ��Ϣ�������


void CImageEditorImagePreview::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			CRect rc;
			rc.IntersectRect(tracker.m_rect, m_rcImage);

			m_tracker.m_rect = rc;
			bNewRect = TRUE;
		}
	}

	if(bNewRect)
	{
		OnChangeRect();
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


BOOL CImageEditorImagePreview::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_tracker.SetCursor( pWnd, nHitTest )) 
		return TRUE;
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CImageEditorImagePreview::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
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


void CImageEditorImagePreview::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(GetSafeHwnd() == NULL) return;
	RecalcImageRect();
}


BOOL CImageEditorImagePreview::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: �ڴ����ר�ô����/����û���
	RecalcImageRect();
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}


BOOL CImageEditorImagePreview::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
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
