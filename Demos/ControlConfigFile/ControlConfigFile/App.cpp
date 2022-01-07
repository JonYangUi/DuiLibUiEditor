#include "StdAfx.h"
#include "App.h"

CApp uiApp;

CApp::CApp(void)
{
}


CApp::~CApp(void)
{
}


bool CApp::InitInstance(HINSTANCE hInstance)
{
	if(!__super::InitInstance(hInstance))
		return false;

	//���������ļ�����������ļ������ڣ�����ʱ�����µġ�
	//Ĭ���ǳ���Ŀ¼�µ� AppName.xml
	//szPathName��Ϊ�յĻ�����Ҫָ��·�����ļ�
	Config.LoadConfig();

	int x = sizeof(CControlUI);

	//����������
	m_pMainWnd = new CMainFrame();
	if( m_pMainWnd == NULL ) return false;
	m_pMainWnd->Create(NULL, GetAppName(), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	m_pMainWnd->CenterWindow();

	return true;
}

int CApp::ExitInstance()
{
	//���������ļ�
	Config.SaveConfig();
	return __super::ExitInstance();
}