#pragma once
#include "SciXmlWriter.h"
// CDuiEditorCode ��ͼ

class CUIManager;
class CDuiEditorViewCode : public CView
{
	DECLARE_DYNCREATE(CDuiEditorViewCode)
public:
	CDuiEditorViewCode();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDuiEditorViewCode();

	CDuiEditorDoc* GetDocument() const;
	void LoadDocument();
	BOOL ApplyDocument();	//ǿ�Ƹ���
	BOOL UpdateDocument(); //��������
	BOOL IsModify() { return sci.sci_GetModify(); }

	void SelectXmlNode(CControlUI *pControl);
	void SelectXmlNode(xml_node node);

	//��ƽ��������ӿؼ�����ʱ����XML
	void AddNode(xml_node node);
	//��ƽ�����ɾ���ؼ�����ʱ����XML
	void DeleteNode(xml_node node);

	//��ƽ������������ԣ���ʱ����XML
	void AddAttribute(xml_node node, xml_attribute attr);
	//��ƽ������޸����ԣ���ʱ����XML
	void ModifyAttribute(xml_node node, xml_attribute attr);
	//��ƽ�����ɾ�����ԣ���ʱ����XML
	void DeleteAttribute(xml_node node, xml_attribute attr);

public:
	CSciWnd *GetSciWnd() { return &sci; }
	BOOL m_bFirstLoading;
	xml_parse_result *GetParseResult() { return &m_xmlParseResult; }
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;
	DWORD m_dwLastClickCode;
protected:
	CSciWnd sci;
	int m_nTargetLine;					//�����ĵ��������Ŀ����
	BOOL m_bAutoUpdateDesign;			//�Ƿ���Ҫ���µ���ͼ
	BOOL m_bNeedUpdate;					//�Ƿ����������ͼ
	BOOL m_bUpdateDesignWhileModified; //���޸��ĵ�ʱ���Ƿ���������������ͼ
	xml_parse_result m_xmlParseResult;
protected:
	BOOL SelectControlUI(int pos, xml_node node);
	void UpdateFrameStatus();

	CString GetNodeName();
	void AutoCompleteNode(CString objectName);		//�Զ���ɿؼ���
	CString AutoCompleteProperty(CString objectName, CString AttrName);	//�Զ����������
public:
	void findMatchingBracePos(int & braceAtCaret, int & braceOpposite);
	bool braceMatch();
	bool BraceHighLightAttributes(int first, int second, int openTagTailLen);
public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg LRESULT OnSciClick(WPARAM WParam, LPARAM LParam);
	afx_msg LRESULT OnSciMouseMove(WPARAM WParam, LPARAM LParam);
	
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	virtual void OnInitialUpdate();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEditRefresh();
	afx_msg void OnEditSelectAll();
	afx_msg void OnSciUpdateDesign();
	afx_msg void OnUpdateSciUpdateDesign(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSciParseXml();
	afx_msg void OnSciApplyDesign();
	afx_msg void OnUpdateSciApplyDesign(CCmdUI *pCmdUI);
	afx_msg void OnSciWrapLine();
	afx_msg void OnUpdateSciWrapLine(CCmdUI *pCmdUI);
	afx_msg void OnSciUpdateWhileModify();
	afx_msg void OnUpdateSciUpdateWhileModify(CCmdUI *pCmdUI);
};
