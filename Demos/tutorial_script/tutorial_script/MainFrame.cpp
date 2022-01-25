#include "StdAfx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(void)
{
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::InitWindow()
{
	IScriptManager *pEngine = GetManager()->GetScriptEngine();
	if(pEngine)
	{
		//���ýű�����
		//
		IScriptFunction fun;
		fun.SetReturnType(UIArg_void);					//����ֵ
		fun.SetFuctionName(_T("MainFrame_InitWindow")); //������
		fun.PushArgAddress(GetManager());				//������ջ
		pEngine->ExecuteScript(&fun);
	}
	return;
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	IScriptManager *pEngine = GetManager()->GetScriptEngine();
	if(pEngine)
	{
		//���ýű�����
		//
		IScriptFunction fun;
		fun.SetReturnType(UIArg_bool);						//����ֵ
		fun.SetFuctionName(_T("MainFrame_OnMenuCommand"));	//������
		fun.PushArgAddress(GetManager());					//������ջ
		fun.PushArgAddress((void *)cmd);
		pEngine->ExecuteScript(&fun);
		if(fun.GetReturnBool())
			return true;
	}
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	IScriptManager *pEngine = GetManager()->GetScriptEngine();
	if(pEngine)
	{
		IScriptFunction fun;
		fun.SetReturnType(UIArg_void);
		fun.SetFuctionName(_T("MainFrame_OnMenuUpdateCommandUI"));
		fun.PushArgAddress(GetManager());
		fun.PushArgAddress((void *)cmdUI);
		pEngine->ExecuteScript(&fun);
		if(fun.GetReturnBool())
			return true;
	}
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{
	IScriptManager *pEngine = GetManager()->GetScriptEngine();
	if(pEngine)
	{
		IScriptFunction fun;
		fun.SetReturnType(UIArg_void);
		fun.SetFuctionName(_T("MainFrame_OnNotifyClick"));
		fun.PushArgAddress(GetManager());
		fun.PushArgAddress(&msg);
		pEngine->ExecuteScript(&fun);
	}
}

void CMainFrame::OnNotifyTimer(TNotifyUI& msg)
{
	IScriptManager *pEngine = GetManager()->GetScriptEngine();
	if(pEngine)
	{
		IScriptFunction fun;
		fun.SetReturnType(UIArg_void);
		fun.SetFuctionName(_T("MainFrame_OnNotifyTimer"));
		fun.PushArgAddress(GetManager());
		fun.PushArgAddress(&msg);
		pEngine->ExecuteScript(&fun);
	}
}