#pragma once


// CDlgCustomControl �Ի���

class CDlgCustomControl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCustomControl)

public:
	CDlgCustomControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCustomControl();

// �Ի�������
	enum { IDD = IDD_DLG_CUSTOM_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strClassName;
	CString m_strParentControl;
	CComboBox m_cbParentControl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeEdit1();
};
