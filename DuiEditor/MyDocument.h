#pragma once

// CMyDocument �ĵ�

class CMyDocument : public CDocument
{
	DECLARE_DYNCREATE(CMyDocument)

public:
	CMyDocument();
	virtual ~CMyDocument();

	CString m_strDefaultTitle;
public:
	virtual void OnDocumentEvent(DocumentEvent deEvent);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual BOOL SaveModified();

	//���滺���ļ�
	virtual void SaveBackupFile(){}

	DECLARE_MESSAGE_MAP()
};
