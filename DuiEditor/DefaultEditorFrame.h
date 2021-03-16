#pragma once
#include "DefaultEditorView.h"
#include "DockPropertyWnd.h"

// CDefaultEditorFrame ���

class CDefaultEditorFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CDefaultEditorFrame)
protected:
	CDefaultEditorFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDefaultEditorFrame();

public:
	CDefaultEditorView		m_wndView;
	CDockPropertyWnd		m_wndProperty;
	CMFCToolBar				m_wndToolBar;
	CUIPropertyGridCtrl *m_pPropList;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnPropertyChanged (WPARAM,LPARAM lParam);
	afx_msg void OnTbExit();
	afx_msg void OnUpdateTbExit(CCmdUI *pCmdUI);
	afx_msg void OnTbSave();
	afx_msg void OnUpdateTbSave(CCmdUI *pCmdUI);
	afx_msg void OnTbUiEffect();
	afx_msg void OnUpdateTbUiEffect(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
};


