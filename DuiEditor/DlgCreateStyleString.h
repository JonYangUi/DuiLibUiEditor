#pragma once


// CDlgCreateStyleString �Ի���

class CDlgCreateStyleString : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCreateStyleString)

public:
	CDlgCreateStyleString(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCreateStyleString();

// �Ի�������
	enum { IDD = IDD_DLG_CREATE_STYLE_STRING };
	xml_node m_node;

	void BuildString();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_strStyleName;
	CString m_strClassName;
	BOOL m_bShared;
	CString m_strStyleString;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit7();
	afx_msg void OnClickedCheck1();
};
