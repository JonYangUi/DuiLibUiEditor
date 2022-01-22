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
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{

}
