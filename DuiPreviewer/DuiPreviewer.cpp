// DuiLibTemplate.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DuiPreviewer.h"

#include "App.h"

//xml�����ļ����ö���ļ���ʱ����Ҫ�ж�CPaintManagerUI::SetResourcePath()��·��
//��ʱ��ҪѰ��project.dui��������Ϣ
void InitResourcePath()
{
	LSSTRING_CONVERSION;
	

	CString strPath = g_strSkinPath;

	//���ϼ��ļ���Ѱ����Ŀ���ã�������5����ļ���
	int maxLevel = 5;
	int times = 1;
	while(!PathFileExists(strPath + _T("project.dui")) && times < maxLevel)
	{
		strPath += _T("..\\");
		times++;
	}


	if(times < maxLevel) //����ҵ��������ļ�
	{
		xml_document projFile;
		xml_parse_result ret = projFile.load_file(strPath + _T("project.dui"));
		CString projPath = projFile.child(_T("Project")).child(_T("ProjectPath")).text().get();
		if(projPath.IsEmpty())
		{
			CPaintManagerUI::SetResourcePath(g_strSkinPath);
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		}
		else
		{
			TCHAR newPath[MAX_PATH + 1];
			_tcscpy_s(newPath, MAX_PATH, strPath);
			//BOOL bRet = PathResolve(newPath, NULL, PRF_VERIFYEXISTS);
			BOOL bRet = PathCanonicalize(newPath, strPath);
			if(bRet)
			{
				PathAddBackslash(newPath);
				strPath = LSW2T(newPath);
			}

			CString skinFile;
			skinFile = g_strFullPath.Right(g_strFullPath.GetLength() - strPath.GetLength());

			g_strSkinFile = skinFile;
			g_strSkinPath = strPath;
		}
	}

}

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
			g_strFullPath = cmdline;
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

	InitResourcePath();
	//////////////////////////////////////////////////////////////////////////
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
