#pragma once


// CDlgBuildLanguagePackageEx �Ի���
class CUIManager;
class CDlgBuildLanguagePackageEx : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBuildLanguagePackageEx)

public:
	CDlgBuildLanguagePackageEx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBuildLanguagePackageEx();

// �Ի�������
	enum { IDD = IDD_BUILD_LANG_PACKAGE_EX };
	CSciWnd sci;

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }

private:
	CUIManager *m_pUIManager;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void _buildLangPackage(xml_node rootLang, xml_node nodeSkin);
	CComboBox m_cbLang;
};
