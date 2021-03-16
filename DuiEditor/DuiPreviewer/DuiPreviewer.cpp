// DuiPreviewer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DuiPreviewer.h"
#include "WinMain.h"
#include "DuiPreviewerWnd.h"
#include "locale.h"

#include "../LsStringConverter.h"
LsStringConverter __lsstring__convert__temp__instance__;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//MessageBox(NULL, lpCmdLine, lpCmdLine, MB_OK);

	if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))
	{
		return 1;
	}

	CString strSkinPath;
	CString strSkinFile;
	HANDLE  m_hOutput = NULL;

	int argc = 0;
	LSSTRING_CONVERSION;
	LPWSTR *argv = CommandLineToArgvW(LST2W(lpCmdLine), &argc);

	//���������в���
	for (int i=0; i<argc; i++)
	{
		CString cmdline = LSW2T(argv[i]);
		if(cmdline.Find(_T("-f")) >= 0)
		{
			cmdline.Delete(0,2);
			cmdline.TrimLeft();
			int sel = cmdline.ReverseFind('\\');
			strSkinPath = cmdline.Left(sel + 1);
			strSkinFile = cmdline.Right(cmdline.GetLength() - sel - 1);
		}
		else if(cmdline.Find(_T("-o")) >= 0)
		{
			cmdline.Delete(0,2);
			cmdline.TrimLeft();
			m_hOutput = (HANDLE)_ttoi(cmdline);
		}
	}

	g_hOutput = m_hOutput;

	BOOL bRunConsole = FALSE;
	//���û�����, ����ϵͳConsole
	if(g_hOutput == NULL)
	{

		bRunConsole = TRUE;

		AllocConsole();
		SetConsoleTitle(_T("duilib debug"));
		_tfreopen(_T("CONOUT$"), _T("w+t"),stdout); 
		_tfreopen(_T("CONIN$"), _T("r+t"),stdin); 
		_tsetlocale(0,_T("chs") );
	}


	//���������û�������ļ�, ��������ѡ��.
	if(strSkinPath.IsEmpty() || strSkinFile.IsEmpty())
	{

		CWinMain dlg(hInstance);
		dlg.Run();

		int sel = CWinMain::m_strSelectFile.ReverseFind('\\');
		strSkinPath = CWinMain::m_strSelectFile.Left(sel + 1);
		strSkinFile = CWinMain::m_strSelectFile.Right(CWinMain::m_strSelectFile.GetLength() - sel - 1);
	}

	DuiWriteConsole(_T("loading duilib ......"));

// 	for (;;)
// 	{
// 		int x = 0;
// 	}

	CoInitialize(NULL);
	CPaintManagerUI::SetInstance(hInstance);

#ifdef _DEBUG
	CPaintManagerUI::LoadPlugin( CPaintManagerUI::GetInstancePath() + _T("DuiPlugins_ud.dll"));
#else
	CPaintManagerUI::LoadPlugin(CPaintManagerUI::GetInstancePath() + _T("DuiPlugins_u.dll"));
#endif

#ifndef DUILIB_VERSION_ORIGINAL
#ifdef _DEBUG
	CPaintManagerUI::LoadScriptPlugin(CPaintManagerUI::GetInstancePath() + _T("DuiScript_ud.dll"));
#else
	CPaintManagerUI::LoadScriptPlugin(CPaintManagerUI::GetInstancePath() + _T("DuiScript_u.dll"));
#endif
#endif

	CPaintManagerUI::SetResourcePath(strSkinPath);

	try
	{
		CDuiPreviewerWnd *pFrame = new CDuiPreviewerWnd;
		pFrame->m_strSkinFile = strSkinFile;
		pFrame->Create(NULL, _T("duilib"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		pFrame->CenterWindow();
		pFrame->ShowWindow(true, true);

		CPaintManagerUI::MessageLoop();

		// ������Դ
		CPaintManagerUI::Term();
	}
	catch (...)
	{
		
	}

	CoUninitialize();

	if(bRunConsole)
	{
		FreeConsole();
	}
	return 0;
}