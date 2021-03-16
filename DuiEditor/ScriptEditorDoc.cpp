// ScriptEditorDoc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ScriptEditorDoc.h"
#include "ScriptEditorView.h"
#include "MainFrm.h"

// CScriptEditorDoc

IMPLEMENT_DYNCREATE(CScriptEditorDoc, CDocument)

CScriptEditorDoc::CScriptEditorDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	m_pPropList = pMain->m_wndStack.CreateStackView();
}

BOOL CScriptEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
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


BEGIN_MESSAGE_MAP(CScriptEditorDoc, CDocument)
END_MESSAGE_MAP()

// CScriptEditorDoc ����


BOOL CScriptEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

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

void CScriptEditorDoc::OnDocumentEvent(DocumentEvent deEvent)
{
	__super::OnDocumentEvent(deEvent);

	switch (deEvent)
	{
	case onAfterNewDocument:
		{
			break;
		}

	case onAfterOpenDocument:
		{
			InitFileView(NULL);
			break;
		}

	case onAfterSaveDocument:
		{
			InitFileView(NULL);
			break;
		}

	case onAfterCloseDocument:
		{
			//InitFileView();
			break;
		}
	}
}

void CScriptEditorDoc::InitFileView(CDocument *pDocCurrentClose)
{
	CDocument *pDoc = NULL;
	int nCount = 0;
	POSITION pos = GetDocTemplate()->GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument *p = GetDocTemplate()->GetNextDoc(pos);
		if(p != pDocCurrentClose)
		{
			pDoc = p;
			nCount++;
		}
	}

	if(pDoc && nCount == 1)
	{
		CString strPath;
		CString strTemp = pDoc->GetPathName();
		int nPos = strTemp.ReverseFind(_T('\\'));
		if(nPos != -1)
		{
			strPath = strTemp.Left(nPos + 1);
		}

		if(!strPath.IsEmpty())
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndFileView.m_wndFileView.InitFolder(strPath);
		}
	}
}

void CScriptEditorDoc::OnCloseDocument()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->IsClosingNow()) //�����ر��ļ�����Ҫ����session
	{
		g_session.DeleteSession(GetPathName());
	}

	CDocument::OnCloseDocument();
	if(pMain->IsClosingNow()) return;

	POSITION pos = ((CDuiEditorApp *)AfxGetApp())->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		CDocTemplate *pDocTemplate = ((CDuiEditorApp *)AfxGetApp())->GetNextDocTemplate(pos);			
		POSITION pos1 = pDocTemplate->GetFirstDocPosition();
		if(pos1 != NULL)
		{
			return;
		}
	}

	//���û�д��κ��ĵ�������л����ļ��б�
	pMain->m_wndFileView.ShowPane(TRUE, FALSE,TRUE);
}

void CScriptEditorDoc::SetModifiedFlag(BOOL bModified)
{
	if(m_bModified != bModified)
	{
		CString strTitle;
		if(GetPathName().IsEmpty())
		{
			strTitle = m_strDefaultTitle;
		}
		else
		{
			LPCTSTR lpszPathName = (LPCTSTR)GetPathName();

			// always capture the complete file name including extension (if present)
			LPTSTR lpszTemp = (LPTSTR)lpszPathName;
			for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
			{
				// remember last directory/drive separator
				if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
					lpszTemp = (LPTSTR)_tcsinc(lpsz);
			}

			strTitle = lpszTemp;
		}

		if(bModified)
			SetTitle(strTitle + " *");
		else
			SetTitle(strTitle);
	}

	__super::SetModifiedFlag(bModified);
}