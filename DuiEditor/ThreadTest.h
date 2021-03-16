#pragma once

// CThreadTestForm
#include "DuiEditorDoc.h"

extern DWORD g_nThreadTestID;
extern HANDLE g_hThreadTestHandle;
class CThreadTest : public CWinThread
{
	DECLARE_DYNCREATE(CThreadTest)

public:
	CThreadTest();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CThreadTest();

	CDuiEditorDoc *m_pDoc;
	CString m_strSpacialFile;

	PROCESS_INFORMATION m_piProcInfo; 
	STARTUPINFO m_siStartInfo; 

	int m_nTestFrom;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};

extern CThreadTest *g_pThreadTest;

