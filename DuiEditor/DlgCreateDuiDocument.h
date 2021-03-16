#pragma once
#include "StaticImageView.h"

// CDlgCreateDuiDocument �Ի���

class CDlgCreateDuiDocument : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCreateDuiDocument)

public:
	CDlgCreateDuiDocument(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCreateDuiDocument();

// �Ի�������
	enum { IDD = IDD_DLG_CREATE_DUI_DOCUMENT };
	CString m_strModuleName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listBox;
	CStaticImageView m_staPicture;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeList1();
};
