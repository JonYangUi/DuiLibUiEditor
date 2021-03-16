
// DuiEditorDoc.h : CDuiEditorDoc ��Ľӿ�
//
#pragma once

class CUIManager;
class CDuiEditorDoc : public CDocument
{
protected: // �������л�����
	CDuiEditorDoc();
	DECLARE_DYNCREATE(CDuiEditorDoc)

// ����
public:
	pugi::xml_document m_doc;		 //�����ļ�
	CStdStringPtrMap m_mLangPackage; //������԰�
	BOOL m_bMenuWnd;

	CString m_strMyTitle;
	CString m_strSessionFile;
public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;

// ����
public:
	void FilterDefaultValue(xml_node nodeDoc);	//�����ĵ�Ĭ������
	void FilterPosWidthHeight(xml_node nodeDoc);

	CString GetSkinPath();
	CString GetSkinFileName();

	void InitFileView(CDocument *pDocCurrentClose);

	void LoadLangPackage(LPCTSTR lpszPathName);
	void SaveLangPackage(LPCTSTR lpszPathName);

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnNewDocumentFromUiTemplate();
#ifdef SHARED_HANDLERS
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CDuiEditorDoc();

private:
	CString m_strDefaultTitle;
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDocumentEvent(DocumentEvent deEvent);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual BOOL IsModified();
	void SaveBackupFile();
	afx_msg void OnFileReopen();	
	afx_msg void OnEditInsertFont();
	afx_msg void OnEditInsertDefault();
	afx_msg void OnEditInsertStyleNode();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUiformDebug();
	afx_msg void OnUpdateUiformDebug(CCmdUI *pCmdUI);
	afx_msg void OnUiformDebugEnd();
	afx_msg void OnUpdateUiformDebugEnd(CCmdUI *pCmdUI);
	afx_msg void OnBuildLangPackage();
	afx_msg void OnBuildLangPackageEx();
	afx_msg void OnBuildLangStringTable();
};
