
// ChildFrm2.h : CChildFrame2 ��Ľӿ�
//

#pragma once

class CChildFrame2 : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame2)
public:
	CChildFrame2();

// ����
public:
	//CMFCToolBar       m_wndToolBar;
// ����
public:

// ��д
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildFrame2();

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
