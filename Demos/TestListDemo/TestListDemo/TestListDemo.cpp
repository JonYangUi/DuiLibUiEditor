// DuiLibTemplate.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestListDemo.h"

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

	//设置主窗口的名字
	uiApp.SetAppName(_T("TestListDemo"));

	//设置程序为单一执行实例，允许多实例的话，就不需要这行了。
	uiApp.SetSingleApplication(true, _T("6A9ECCA2-12C6-40FD-882C-34DD4AA06B86"));

	//设置xml资源载入方式，选择一种就行	
#ifdef _DEBUG
	uiApp.SetResType_File(_T("skin"));								//从文件夹中载入xml文件
#else
	uiApp.SetResType_ZipResource(_T("skin"), _T(""), IDR_ZIPRES1, _T("ZIPRES"));	//从程序资源文件中载入xml文件
#endif

	//初始化duilib, 注册插件，创建主窗口
	if(!uiApp.InitInstance(hInstance)) 
		return 0;

	//进入消息循环
	uiApp.Run();

	//清理资源。
	uiApp.ExitInstance();
	return 0;
}
