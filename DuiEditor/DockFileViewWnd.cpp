// DockFileViewWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockFileViewWnd.h"


// CDockFileViewWnd

IMPLEMENT_DYNAMIC(CDockFileViewWnd, CDockablePane)

CDockFileViewWnd::CDockFileViewWnd()
{

}

CDockFileViewWnd::~CDockFileViewWnd()
{
}


BEGIN_MESSAGE_MAP(CDockFileViewWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CDockFileViewWnd ��Ϣ�������
int CDockFileViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		return -1;      // δ�ܴ���
	}

	return 0;
}


void CDockFileViewWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);

	m_wndFileView.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}


void CDockFileViewWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴������Ϣ����������
	m_wndFileView.SetFocus();
}


void CDockFileViewWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{

}
