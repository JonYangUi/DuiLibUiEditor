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
	//���Դ��������������δ��롣
	UI_BINDCONTROL(CEditUI, m_edit_text, _T("edit_text"));
	UI_BINDCONTROL(CRichEditUI, m_redit_text, _T("redit_text"));
	UI_BINDCONTROL(COptionUI, m_opt_check, _T("opt_check"));
	UI_BINDCONTROL(CComboExUI, m_cb_recv_type, _T("cb_recv_type"));

	//ָ�������ļ��У���Ӧ�ĸ��ڵ㡣
	CXmlNodeUI root = uiApp.Config.GetRoot().child_auto(_T("MainFrame"));

	//����ؼ����ݡ� ���Դ��������������δ��롣
	UIGetApp()->Config.LoadControl(root, m_edit_text, _T("default_text_1"));
	UIGetApp()->Config.LoadControl(root, m_redit_text, _T("default_text_2"));
	UIGetApp()->Config.LoadControl(root, m_opt_check, TRUE);
	UIGetApp()->Config.LoadControl(root, m_cb_recv_type, _T("AAA"), TRUE);

	//û�����ݵĻ���д�뼸��������
	if(m_cb_recv_type->GetCount() <= 0)
	{
		m_cb_recv_type->AddString(_T("AAA"));
		m_cb_recv_type->AddString(_T("BBB"));
		m_cb_recv_type->AddString(_T("CCC"));
		m_cb_recv_type->AddString(_T("DDD"));
	}
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CLOSE)
	{
		//ָ�������ļ��У���Ӧ�ĸ��ڵ㡣
		CXmlNodeUI root = uiApp.Config.GetRoot().child_auto(_T("MainFrame"));

		//����ؼ����ݡ� ���Դ��������������δ��롣
		UIGetApp()->Config.SaveControl(root, m_edit_text);
		UIGetApp()->Config.SaveControl(root, m_redit_text);
		UIGetApp()->Config.SaveControl(root, m_opt_check);
		UIGetApp()->Config.SaveControl(root, m_cb_recv_type, TRUE);
	}
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