// DuiEditorView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DuiEditorView.h"


// CDuiEditorView

IMPLEMENT_DYNCREATE(CDuiEditorView, CView)

CDuiEditorView::CDuiEditorView()
{

}

CDuiEditorView::~CDuiEditorView()
{
}

BEGIN_MESSAGE_MAP(CDuiEditorView, CView)
END_MESSAGE_MAP()


// CDuiEditorView ��ͼ

void CDuiEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CDuiEditorView ���

#ifdef _DEBUG
void CDuiEditorView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDuiEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDuiEditorView ��Ϣ�������
