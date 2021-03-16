// ImageEditorThumbnail.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorPaneImage.h"


// CImageEditorThumbnail

IMPLEMENT_DYNAMIC(CImageEditorPaneImage, CDockablePane)

CImageEditorPaneImage::CImageEditorPaneImage()
{
	m_pView = NULL;
	m_strCaption = _T("ԭʼͼƬ");
}

CImageEditorPaneImage::~CImageEditorPaneImage()
{
}


BEGIN_MESSAGE_MAP(CImageEditorPaneImage, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CImageEditorThumbnail ��Ϣ�������




int CImageEditorPaneImage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
// 	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
// 	{
// 		TRACE0("δ�ܴ�����ͼ����\n");
// 		return -1;
// 	}
	m_pView = new CImageEditorImagePreview;
	if (!m_pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}
	m_pView->OnInitialUpdate();
	return 0;
}

void CImageEditorPaneImage::SetCaptionText(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
	SetWindowText(lpszCaption);
}

void CImageEditorPaneImage::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_pView && m_pView->GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);
		m_pView->MoveWindow(rc);
	}
}


BOOL CImageEditorPaneImage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		if(m_pView && m_pView->GetSafeHwnd())
		{
			m_pView->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		}
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}
