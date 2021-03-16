#pragma once
#include "UIDlgLangTextEdit.h"

// CDlgLangTextEdit �Ի���

class CUIManager;
class CDlgLangTextEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLangTextEdit)

public:
	CDlgLangTextEdit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLangTextEdit();

// �Ի�������
	enum { IDD = IDD_DLG_LANG_TEXT_EDIT };
	CGridCtrl m_Grid;
	int m_currentid;
	CDlgLangTextEditUI *m_uidlg;
	CString m_strCurLangType;
public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
	CUIManager *m_pUIManager;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CTreeCtrl m_Tree;
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	void LoadText(LPCTSTR langtype);
	void InsertGridRow(LPCTSTR attrName, LPCTSTR attrValue);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnClickLangType();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
