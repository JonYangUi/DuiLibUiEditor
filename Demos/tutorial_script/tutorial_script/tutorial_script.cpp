// DuiLibTemplate.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "tutorial_script.h"

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

	//���������ڵ�����
	uiApp.SetAppName(_T("tutorial_script"));

	//���ó���Ϊ��һִ��ʵ���������ʵ���Ļ����Ͳ���Ҫ�����ˡ�
	uiApp.SetSingleApplication(true, _T("02499635-8835-487C-8EFD-4B31C491A6C5"));

	//����xml��Դ���뷽ʽ��ѡ��һ�־���	
#ifdef _DEBUG
	uiApp.SetResType_File(_T("skin"));								//���ļ���������xml�ļ�
#else
	uiApp.SetResType_ZipResource(_T("skin"), _T(""), IDR_ZIPRES1, _T("ZIPRES"));	//�ӳ�����Դ�ļ�������xml�ļ�
#endif

	//��ʼ��duilib, ע����������������
	if(!uiApp.InitInstance(hInstance)) 
		return 0;

	//������Ϣѭ��
	uiApp.Run();

	//������Դ��
	uiApp.ExitInstance();
	return 0;
}
