
// DuiEditorView.h : CDuiEditorView ��Ľӿ�
//

#pragma once
#include "DuiEditorDoc.h"
#include <list>

class CUIManager;
class CDuiEditorViewDesign : public CScrollView
{
protected: // �������л�����
	CDuiEditorViewDesign();
	DECLARE_DYNCREATE(CDuiEditorViewDesign)

// ����
public:
	CDuiEditorDoc* GetDocument() const;

	int m_zoom;
	int m_nFormatInsert;	//����ģʽ��ͨ�Ļ��߾��Զ�λ��
	BOOL m_bShowUiPreview;	//�Ƿ��ڱ༭������ʾUIЧ��
	BOOL m_bViewRuleBar;	//�Ƿ���ʾ���
	BOOL m_bViewGrid;		//�Ƿ���ʾ����
	BOOL m_bViewMouse;		//�Ƿ���ʾ�����

	PROCESS_INFORMATION m_piProcInfo; 
	STARTUPINFO m_siStartInfo; 

	CStringArray m_arrTempStyles;

	struct tagFontMenu
	{
		UINT cmdID;
		CString attrName;
		TFontInfo *pFontInfo;
	};
	std::map<UINT, tagFontMenu *> m_mapFontMenu;
public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;
	int _nMaxResourceID;
// ����
public:
	void InitView();
	BOOL IsViewRuleBar() { return m_bViewRuleBar; }
// ��д
protected:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate(); // ������һ�ε���

	void ShowShadowUI(CDuiEditorViewDesign *pView, BOOL bShow);
// ʵ��
public:
	virtual ~CDuiEditorViewDesign();

protected:
	BOOL m_bMouseTracking;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);

	void CopyToClipboard(LPCTSTR strText);
	afx_msg void OnEditCopyName();
	afx_msg void OnUpdateEditCopyName(CCmdUI *pCmdUI);
	afx_msg void OnEditCopyNameEx();
	afx_msg void OnUpdateEditCopyNameEx(CCmdUI *pCmdUI);

	afx_msg void OnEditGenerateCode_FindControl();
	afx_msg void OnUpdateEditGenerateCode_FindControl(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_Members();
	afx_msg void OnUpdateEditGenerateCode_Members(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_BindControl();
	afx_msg void OnUpdateEditGenerateCode_BindControl(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_BindSubControl();
	afx_msg void OnUpdateEditGenerateCode_BindSubControl(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_UiCommand();
	afx_msg void OnUpdateEditGenerateCode_UiCommand(CCmdUI *pCmdUI);

	afx_msg void OnEditGenerateCode_ddxText();
	afx_msg void OnUpdateEditGenerateCode_ddxText(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_ddxCheckBox();
	afx_msg void OnUpdateEditGenerateCode_ddxCheckBox(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_ddxCombo();
	afx_msg void OnUpdateEditGenerateCode_ddxCombo(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_ddxComboItemData();
	afx_msg void OnUpdateEditGenerateCode_ddxComboItemData(CCmdUI *pCmdUI);

	afx_msg void OnEditGenerateCode_IsControl();
	afx_msg void OnUpdateEditGenerateCode_IsControl(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_IsMenuCommand();
	afx_msg void OnUpdateEditGenerateCode_IsMenuCommand(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_SaveControl();
	afx_msg void OnUpdateEditGenerateCode_SaveControl(CCmdUI *pCmdUI);
	afx_msg void OnEditGenerateCode_LoadControl();
	afx_msg void OnUpdateEditGenerateCode_LoadControl(CCmdUI *pCmdUI);

	afx_msg void OnEditCreateStyleString();
	afx_msg void OnUpdateEditCreateStyleString(CCmdUI *pCmdUI);

	afx_msg void OnCommandTabLayoutSetSel(UINT nID);

	afx_msg void OnFormatAlignLeft();
	afx_msg void OnUpdateFormatAlignLeft(CCmdUI *pCmdUI);
	afx_msg void OnFormatAlignRight();
	afx_msg void OnUpdateFormatAlignRight(CCmdUI *pCmdUI);
	afx_msg void OnFormatAlignTop();
	afx_msg void OnUpdateFormatAlignTop(CCmdUI *pCmdUI);
	afx_msg void OnFormatAlignBottom();
	afx_msg void OnUpdateFormatAlignBottom(CCmdUI *pCmdUI);
	afx_msg void OnFormatAlignSameSpaceVert();
	afx_msg void OnUpdateFormatAlignSameSpaceVert(CCmdUI *pCmdUI);
	afx_msg void OnFormatAlignSameSpaceHori();
	afx_msg void OnUpdateFormatAlignSameSpaceHori(CCmdUI *pCmdUI);
	afx_msg void OnFormatAlignCenterVert();
	afx_msg void OnUpdateFormatAlignCenterVert(CCmdUI *pCmdUI);
	afx_msg void OnFormatAlignCenterHori();
	afx_msg void OnUpdateFormatAlignCenterHori(CCmdUI *pCmdUI);
	afx_msg void OnFormatSameWidth();
	afx_msg void OnUpdateFormatSameWidth(CCmdUI *pCmdUI);
	afx_msg void OnFormatSameHeight();
	afx_msg void OnUpdateFormatSameHeight(CCmdUI *pCmdUI);
	afx_msg void OnFormatSameSize();
	afx_msg void OnUpdateFormatSameSize(CCmdUI *pCmdUI);
	afx_msg void OnEditZoomIn();
	afx_msg void OnUpdateEditZoomIn(CCmdUI *pCmdUI);
	afx_msg void OnEditZoomOut();
	afx_msg void OnUpdateEditZoomOut(CCmdUI *pCmdUI);
	afx_msg void OnEditZoomFull();
	afx_msg void OnUpdateEditZoomFull(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg void OnFormatInsertNormalControl();
	afx_msg void OnUpdateFormatInsertNormalControl(CCmdUI *pCmdUI);
	afx_msg void OnFormatInsertFloatControl();
	afx_msg void OnUpdateFormatInsertFloatControl(CCmdUI *pCmdUI);
	afx_msg void OnFormatShowRulerbar();
	afx_msg void OnUpdateFormatShowRulerbar(CCmdUI *pCmdUI);
	afx_msg void OnFormatShowGrid();
	afx_msg void OnUpdateFormatShowGrid(CCmdUI *pCmdUI);
	afx_msg void OnFormatShowMouse();
	afx_msg void OnUpdateFormatShowMouse(CCmdUI *pCmdUI);
	afx_msg void OnFormatShowUiPreview();
	afx_msg void OnUpdateFormatShowUiPreview(CCmdUI *pCmdUI);
	afx_msg void OnSetDPI(UINT id);
	afx_msg void OnUpdateSetDPI(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveTemplate();
	afx_msg void OnUpdateFileSaveTemplate(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	BOOL GetMaxResourceID(xml_node root, int &nMaxResourceID);
	afx_msg void OnEditCreateResourceId();
	afx_msg void OnUpdateEditCreateResourceId(CCmdUI *pCmdUI);
	afx_msg void OnEditCreateResourceidAuto();
	void _CreateResourceIDAuto(xml_node root);
	afx_msg void OnEditLangText();
	afx_msg void OnUpdateEditLangText(CCmdUI *pCmdUI);

	afx_msg void OnUpdateStyleList(CCmdUI *pCmdUI);
	afx_msg BOOL OnStyleListRange(UINT uID);
	afx_msg void OnUpdateStyleListRange(CCmdUI *pCmdUI);

	CString GetFontAttributeName(UINT uFontType);
	void UpdateFontPropertyMenu(CCmdUI *pCmdUI, UINT uFontType);
	afx_msg void OnUpdateFontList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHotFontList(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePushedFontList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFocusedFontList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSelectedFontList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemFontList(CCmdUI *pCmdUI);
	afx_msg BOOL OnFontListRange(UINT uID);
	afx_msg void OnUpdateFontListRange(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // DuiEditorView.cpp �еĵ��԰汾
inline CDuiEditorDoc* CDuiEditorViewDesign::GetDocument() const
   { return reinterpret_cast<CDuiEditorDoc*>(m_pDocument); }
#endif

