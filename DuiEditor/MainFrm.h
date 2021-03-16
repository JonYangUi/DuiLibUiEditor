
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "DockFileViewWnd.h"
#include "DockControlTreeWnd.h"
#include "DockScriptStack.h"
#include "DockPropertyWnd.h"
#include "DockOutputWnd.h"
#include "ToolBoxWnd.h"
#include "MyStatusBar.h"

#include "DuiEditorViewDesign.h"
#include "DuiEditorDoc.h"
#include "SciFind.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:
	UINT_PTR m_uTimerReOpenFile;
	CString m_strReOpenFile;
	CDuiEditorDoc *m_pDocReOpen;
// ����
public:
	CDuiEditorViewDesign *GetActiveUIView();
	CUIManager *GetActiveUIManager();

	void ShowAllPane();
	void HideAllPane();

	BOOL IsClosingNow() { return m_bClosing; }
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ʵ��
public:
	virtual ~CMainFrame();

	CMFCToolBar			 m_wndToolBar2;
	CDockPropertyWnd	 m_wndProperty;
	CDockControlTreeWnd	 m_wndControl;
	CDockScriptStackWnd	 m_wndStack;
	CDockFileViewWnd	 m_wndFileView;
	CMyStatusBar		m_wndStatusBar;

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CDockOutputWnd	  m_wndOutput;
	CDockToolBoxWnd	  m_wndToolBox;

	CSciFind		*m_pDlgFind;
	HWND			m_hDlgFind;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg LRESULT OnGetTabTooltip(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnChangeActiveTab(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileCloseAll();
	afx_msg void OnUpdateFileCloseAll(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI *pCmdUI);
	afx_msg void OnEditOptions();
	afx_msg void OnClose();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg LRESULT OnReOpenFile(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewControlTree();
	afx_msg void OnViewOutputBar();
	afx_msg void OnUpdateViewOutputBar(CCmdUI *pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


