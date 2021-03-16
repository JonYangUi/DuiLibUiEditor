#pragma once

class CUIManager;
class CDlgLangTextEditUI : public CUIDialog
{
public:
	CDlgLangTextEditUI(void);
	virtual ~CDlgLangTextEditUI(void);

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
	CUIManager *m_pUIManager;

public:
	virtual LPCTSTR GetWindowClassName() const  { return _T("DlgLangTextEdit"); }
	virtual CDuiString GetSkinFile() { return _T(""); }
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void InitWindow();
	virtual void OnNotifySelectChanged(TNotifyUI& msg);
	virtual void OnNotifyEndEdit(TNotifyUI& msg);

	void LoadText(LPCTSTR langtype);
	int InsertGridRow(LPCTSTR attrName, LPCTSTR attrValue);

	CGridListUI *m_pGrid;
	CDialog *m_pParentDlg;

	int m_currentid;
	CString m_strCurLangType;
};

