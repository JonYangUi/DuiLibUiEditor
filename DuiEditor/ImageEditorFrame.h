#pragma once
#include "DockPropertyWnd.h"
#include "ImageEditorPaneFile.h"
#include "ImageEditorView.h"
#include "ImageEditorPane.h"
#include "ImageEditorPaneImage.h"
//////////////////////////////////////////////////////////////////////////
// CImageEditorFrame ���

class CImageEditorFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CImageEditorFrame)
protected:
	CImageEditorFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CImageEditorFrame();

public:
	CImageEditorView		m_wndView;			//�м����ͼ
	CImageEditorPaneFile	m_wndList;			//��ߵ��ļ��б�
	CDockPropertyWnd		m_wndProperty;
	CMFCToolBar				m_wndToolBar;
	CImageEditorPaneAdjust	m_wndPaneAdjust;	//�·���pane����
	CImageEditorPaneImage	m_wndImage;			//��ߵ�ԭͼ��ʾ����

	CUIPropertyGridCtrl		*m_pPropList;

	BOOL m_bPreview;
	BOOL m_bTrackerDest;
	BOOL m_bTrackSource;
public:
	void OnSelectedFile(LPCTSTR lpstrPathName);
	void ParentPreview();

	BOOL FindInternalDivider (CDockablePane* pBar, 
		CPaneContainer*& pContainer, 
		CPaneDivider*& pDivider, BOOL& bLeftSlider, 
		BOOL& bAloneInContainer);
	void SetPaneHeight(int nHeight);
protected:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM w, LPARAM l);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnPropertyChanged (WPARAM,LPARAM lParam);
	afx_msg void OnTbExit();
	afx_msg void OnUpdateTbExit(CCmdUI *pCmdUI);
	afx_msg void OnTbSave();
	afx_msg void OnUpdateTbSave(CCmdUI *pCmdUI);
	afx_msg void OnTbUiPreview();
	afx_msg void OnUpdateTbUiPreview(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTbImageEditorDest();
	afx_msg void OnUpdateTbImageEditorDest(CCmdUI *pCmdUI);
	afx_msg void OnTbImageEditorSource();
	afx_msg void OnUpdateTbImageEditorSource(CCmdUI *pCmdUI);
	afx_msg void OnTbImageEditorCorner();
	afx_msg void OnUpdateTbImageEditorCorner(CCmdUI *pCmdUI);
};


