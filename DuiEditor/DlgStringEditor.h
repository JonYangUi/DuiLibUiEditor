#pragma once


// CDlgStringEditor �Ի���

class CDlgStringEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStringEditor)

public:
	CDlgStringEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgStringEditor();

// �Ի�������
	enum { IDD = IDD_DLG_STRING_EDITOR };

	CString m_string;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRichEditCtrl m_ctrlRichEdit;
};
