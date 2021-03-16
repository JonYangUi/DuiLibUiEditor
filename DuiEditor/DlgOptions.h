#pragma once

// CDlgOptions �Ի���

class CDlgOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOptions)

public:
	CDlgOptions(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgOptions();

// �Ի�������
	enum { IDD = IDD_DLG_OPTIONS };

	tagEditorConfig m_config;
	void SetXmlOptions(const tagEditorConfig &opt);
	void SetXmlOptions(CSciWnd *pSciWnd, const tagEditorConfig &opt);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CMFCColorButton m_ctrlBkColor;
	CMFCFontComboBox m_cbFont;
	CMFCColorButton m_ctrlBkColor2;
	CMFCColorButton m_ctrlBkColor3;
	CMFCColorButton m_ctrlBkColor4;
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeMfcfontcombo1();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnClickedMfccolorbutton2();
	afx_msg void OnClickedMfccolorbutton3();
	afx_msg void OnClickedMfccolorbutton4();
};
