#pragma once


// CDlgSelectFile �Ի���

class CDlgSelectFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelectFile)

public:
	enum eWorkType
	{
		eWORK_EXPORT_LANG = 0,
		eWORK_EXPORT_NAME,
		eWORK_EXPORT_RES
	};
public:
	CDlgSelectFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSelectFile();

// �Ի�������
	enum { IDD = IDD_DLG_SELECT_FILE };
	eWorkType m_eWorkType;
	CString m_fileTitle;
	void LoadList(LPCTSTR pstrPath);
	BOOL IsNeedLoadToList(CFileFind &finder);

	void OnExportRes();

	void OnExportLang();
	void _buildLangPackage(xml_node rootLang, xml_node nodeSkin);


	CDuiStringArray m_arrMacroName;
	void OnExportName();
	void _buildNameMacroFile(CFile &f, xml_node nodeSkin);
	void _WriteNameMacroFile(CFile &f, CString &sMacroName, CString &sName);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCheckListBox m_fileList;
	afx_msg void OnBnClickedOk();
};
