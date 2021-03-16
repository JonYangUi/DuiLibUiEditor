
// DuiEditor.h : DuiEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "DuiPropertyFile.h"
#include "EditorConfig.h"

#include "DuiProject.h"
extern CDuiProject g_proj;

extern CString g_strAppPath;							//Ӧ�ó���·��
extern CDuiPropertyFile g_duiProp;

#include "SessionList.h"

//extern IScriptEngine *g_pScriptEngine;
//extern IScriptHelper *ScriptHelper;

inline BOOL CompareString(const wchar_t *str1, const wchar_t *str2)
{
	return wcscmp(str1, str2) == 0;
}

inline BOOL CompareString(const char *str1, const char *str2)
{
	return strcmp(str1, str2) == 0;
}

inline BOOL CompareString(const wchar_t *str1, const char *str2)
{
	return strcmp(LSW2A(str1), str2) == 0;
}

inline BOOL CompareString(const char *str1, const wchar_t *str2)
{
	return strcmp(str1, LSW2A(str2)) == 0;
}


inline BOOL SplitCString(CStringArray &arrstr, LPCTSTR strtemp)
{
	CString str(strtemp);

	CString temp;
	for (int i=0; i<str.GetLength(); i++)
	{
		if(_istalnum(str[i]))
			temp += str[i];
		else
		{
			if(!temp.IsEmpty()) 
			{
				arrstr.Add(temp);
				temp.Empty();
			}
		}
	}

	if(!temp.IsEmpty())
	{
		arrstr.Add(temp);
	}

	return TRUE;
}

inline CString RectToString(LPRECT rc)
{
	CString temp;
	temp.Format(_T("%d,%d,%d,%d"), rc->left, rc->top, rc->right, rc->bottom);
	return temp;
}

inline CString NumberToString(int n)
{
	CString temp;
	temp.Format(_T("%d"), n);
	return temp;
}

extern HWND g_hWndMsg;
#define WM_INSERT_MSG		WM_USER + 345
inline void InsertMsg(LPCTSTR strLog)
{
	if(::IsWindow(g_hWndMsg))
	{
		::SendMessage(g_hWndMsg, WM_INSERT_MSG, (WPARAM)strLog, 0);
	}
}
inline void InsertMsgV(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	CString strText;

	va_list argList;
	va_start(argList, lpszFormat);
	strText.FormatV(lpszFormat, argList);
	va_end(argList);

	InsertMsg(strText);
}

class CalcTimeEscape
{
public:
	CalcTimeEscape()
	{
		m_dt = GetTickCount();
	}
	~CalcTimeEscape()
	{
		InsertMsgV(_T("�ķ�ʱ��: %d"), GetTickCount() - m_dt);
	}
	DWORD m_dt;
};

#define WM_REOPEN_FILE			WM_USER + 221
// CDuiEditorApp:
// �йش����ʵ�֣������ DuiEditor.cpp
//

class CDuiEditorApp : public CWinAppEx
{
public:
	CDuiEditorApp();

	HMODULE m_hModuleScript;
// 	CREATE_SCRIPT_ENGINE_INSTANCE m_funCreateScriptEngine;
// 	DELETE_SCRIPT_ENGINE_INSTANCE m_funDeleteScriptEngine;
	CREATE_SCRIPT_HELPER m_funCreateScriptHelper;
	DELETE_SCRIPT_HELPER m_funDeleteScriptHelper;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CDocTemplate *GetUIDocTemplate();
	CDocTemplate *GetScriptDocTemplate();
// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	BOOL  m_bIsDDEOpen;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	void LoadStdProfileSettings(UINT nMaxMRU);
	afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNewXml();
	afx_msg void OnFileNewXmlFromTemplate();
	afx_msg void OnFileNewAsScript();
	afx_msg void OnUpdateFileNewAsScript(CCmdUI *pCmdUI);
};

extern CDuiEditorApp theApp;
