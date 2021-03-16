#pragma once
#include "DuiEditorDoc.h"
#include "DefaultEditorFrame.h"
#include "DefaultEditorView.h"

// CDefaultEditor �Ի���

class CDefaultEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CDefaultEditor)

public:
	CDefaultEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDefaultEditor();

// �Ի�������
	enum { IDD = IDD_DLG_DEFAULT_EDITOR };
	CDuiEditorDoc *m_pDoc;
	CDefaultEditorFrame *m_pFrame;

	xml_document m_nodeControl;
	CString m_strClassName;
public:
	void SetAttributeValue(LPCTSTR szControlName, LPCTSTR szAttribute);
	CString GetAttributeValue();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
