#pragma once
#include "StaticImageView.h"

// CDlgTemplateSave �Ի���

class CDlgTemplateSave : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTemplateSave)

public:
	CDlgTemplateSave(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTemplateSave();

	CDuiEditorDoc *m_pDoc;
// �Ի�������
	enum { IDD = IDD_DLG_TEMPLATE_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStaticImageView m_staPicture;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strTemplateName;
};
