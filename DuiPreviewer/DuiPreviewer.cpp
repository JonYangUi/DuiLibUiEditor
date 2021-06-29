// DuiLibTemplate.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DuiPreviewer.h"

#include "App.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))
	{
		return 1;
	}

	//_CrtSetBreakAlloc();

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
			g_strSkinPath = cmdline.Left(sel + 1);
			g_strSkinFile = cmdline.Right(cmdline.GetLength() - sel - 1);
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

	DuiWriteConsole(_T("loading duilib ......"));

	CPaintManagerUI::UIDESIGNPREVIEW = TRUE;

	//���������ڵ�����
	uiApp.SetAppName(_T("DuiPreviewer"));

	//����xml��Դ���뷽ʽ
	//uiApp.SetResType_File(g_strSkinPath);								//���ļ���������xml�ļ�

	//��ʼ��duilib, ע����������������
	if(!uiApp.InitInstance(hInstance)) 
		return 0;

	//������Ϣѭ��
	uiApp.Run();

	//������Դ��
	uiApp.ExitInstance();
	return 0;
}
