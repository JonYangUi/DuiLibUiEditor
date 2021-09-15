// ScriptEditorDoc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ScriptEditorDoc.h"
#include "ScriptEditorView.h"
#include "MainFrm.h"

// CScriptEditorDoc

IMPLEMENT_DYNCREATE(CScriptEditorDoc, CMyDocument)

CScriptEditorDoc::CScriptEditorDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	m_pPropList = pMain->m_wndStack.CreateStackView();
}

BOOL CScriptEditorDoc::OnNewDocument()
{
	if (!CMyDocument::OnNewDocument())
		return FALSE;

	m_strDefaultTitle = m_strTitle;
	return TRUE;
}

CScriptEditorDoc::~CScriptEditorDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->m_wndStack.RemoveStackView(m_pPropList);
	m_pPropList = NULL;
}


BEGIN_MESSAGE_MAP(CScriptEditorDoc, CMyDocument)
END_MESSAGE_MAP()

// CScriptEditorDoc ����


BOOL CScriptEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CMyDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_strSessionFile = g_session.GetSessionFile(lpszPathName);	
	if(m_strSessionFile != lpszPathName)
	{
		SetModifiedFlag(TRUE);
	}
	g_session.OpenSession(lpszPathName);
	// TODO:  �ڴ������ר�õĴ�������
	return TRUE;
}


BOOL CScriptEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���
	BOOL bRet = FALSE;
	POSITION pos = GetFirstViewPosition();
	CScriptEditorView *pFirstView = (CScriptEditorView*)GetNextView( pos ); // assume only one view
	if (pFirstView != NULL)
	{
		bRet = pFirstView->sci.SaveFile(lpszPathName);

		if(bRet) 
		{
			SetModifiedFlag(FALSE);
			pFirstView->sci.sci_SetSavePoint();
		}
	}
	return TRUE;
	//return CDocument::OnSaveDocument(lpszPathName);
}

void CScriptEditorDoc::OnCloseDocument()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->IsClosingNow()) //�����ر��ļ�����Ҫ����session
	{
		g_session.DeleteSession(GetPathName());
	}

	CMyDocument::OnCloseDocument();
}
