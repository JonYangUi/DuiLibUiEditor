#include "StdAfx.h"
#include "MainFrame.h"

#include "Dialog1.h"
#include "Dialog2.h"
#include "FormAdvanceControl.h"
#include "FormSciEditor.h"

CMainFrame::CMainFrame(void)
{
	AttachVirtualForm(new CFormAdvanceControl);
	AttachVirtualForm(new CFormSciEditor);
	AttachVirtualForm(new CFormTest);
	m_bCheckmenu_cmd_2 = TRUE;
	m_bCheckmenu_cmd_22 = TRUE;
	m_bCheckmenu_cmd_41 = TRUE;
	m_bCheckmenu_cmd_42 = FALSE;
	m_nMenuSelected = 0;

	m_pMenuWnd = NULL;
}


CMainFrame::~CMainFrame(void)
{
	//if(m_pMenuWnd){ delete m_pMenuWnd; m_pMenuWnd = NULL; }
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	CString className = msg.pSender->GetClass();
	CString ControlName = msg.pSender->GetName();
	CString EventName = msg.sType;

// 	if(ControlName != _T("msgwnd_frame"))
// 	{
// 		CString strText;
// 		strText.Format(_T("Notify: class='%s' name='%s' msg='%s'"), className, ControlName, EventName);
// 		InsertMsgUI(strText);
// 	}

	__super::Notify(msg);
}

void CMainFrame::InitWindow()
{
	//�󶨿ؼ�ָ��
	//UI_BINDCONTROL(CImageBoxUI, m_pImageBox, _T("imagebox_1"));
	UI_BINDCONTROL(CProgressUI, m_pProgress, _T("progress_1"));

	//������
	//m_pProgress->SetTimer(1, 200);

	CDuiString strText;
	strText += _T("\r\n��");
	strText += _T("\r\n����");
	strText += _T("\r\n������");
	strText += _T("\r\n������Ļ");
	strText += _T("\r\n������Ļ��");
	strText += _T("\r\n������Ļ����");
	strText += _T("\r\n������Ļ������");
	strText += _T("\r\n������Ļ��������");
	strText += _T("\r\n������Ļ��������Ļ");
	CControlUI *pRoll1 = FindControl(_T("rolltext_1"));
	CControlUI *pRoll2 = FindControl(_T("rolltext_2"));
	CControlUI *pRoll3 = FindControl(_T("rolltext_3"));
	CControlUI *pRoll4 = FindControl(_T("rolltext_4"));
	pRoll1->SetText(pRoll1->GetText() + strText);
	pRoll2->SetText(pRoll2->GetText() + strText);
	pRoll3->SetText(pRoll3->GetText() + strText);
	pRoll4->SetText(pRoll4->GetText() + strText);

	CDuiString str1 = CLangManagerUI::GetString(1);
	CDuiString str2 = CLangManagerUI::GetStringExtra(1);
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	return false;
}

//��Ӧ�˵�����
bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	CString str;
	str.Format(_T("�����˲˵�: name=%s, text=%s"), cmd->szName, cmd->szText);
	InsertMsgUI(str);

	if(IsMenuCommand(cmd, _T("menu_cmd_2")))
	{
		//���±��ز˵�ѡ��״̬
		m_bCheckmenu_cmd_2 = !m_bCheckmenu_cmd_2;
	}
	if(IsMenuCommand(cmd, _T("menu_cmd_22")))
	{
		//���±��ز˵�ѡ��״̬
		m_bCheckmenu_cmd_22 = !m_bCheckmenu_cmd_22;
	}
	if(IsMenuCommand(cmd, _T("menu_cmd_41")))
	{
		//���±��ز˵�ѡ��״̬
		m_bCheckmenu_cmd_41 = !m_bCheckmenu_cmd_41;
	}
	if(IsMenuCommand(cmd, _T("menu_cmd_42")))
	{
		//���±��ز˵�ѡ��״̬
		m_bCheckmenu_cmd_42 = !m_bCheckmenu_cmd_42;
	}
	if(IsMenuCommand(cmd, _T("menu_cmd_5")))
	{
		//���±��ز˵�ѡ��״̬
		m_nMenuSelected = 0;
	}
	if(IsMenuCommand(cmd, _T("menu_cmd_6")))
	{
		//���±��ز˵�ѡ��״̬
		m_nMenuSelected = 1;
	}
	if(IsMenuCommand(cmd, _T("menu_lang_chs")))
	{
		CLangManagerUI::SetLanguage(_T("Lang\\ChineseSimplified"), _T("chs"));
		GetMainWnd()->GetManager()->GetLangManager()->ReloadLanguage();
		return true;
	}
	if(IsMenuCommand(cmd, _T("menu_lang_ChineseTraditional")))
	{
		CLangManagerUI::SetLanguage(_T("Lang\\ChineseTraditional"), _T("chtrad"));
		GetMainWnd()->GetManager()->GetLangManager()->ReloadLanguage();
		return true;
	}
	if(IsMenuCommand(cmd, _T("menu_lang_english")))
	{
		CLangManagerUI::SetLanguage(_T("Lang\\English"), _T("en"));
		GetMainWnd()->GetManager()->GetLangManager()->ReloadLanguage();
		return true;
	}
	return false;
}

/*
	//���²˵�״̬����һ�ַ�ʽ
*/
bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	if(IsMenuCommand(cmdUI, _T("menu_cmd_1")))
	{
		cmdUI->Enable(FALSE);
		cmdUI->SetText(cmdUI->GetText() + _T(" ����"));
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_cmd_2")))
	{
		cmdUI->SetCheck(m_bCheckmenu_cmd_2);
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_cmd_22")))
	{
		cmdUI->SetCheck(m_bCheckmenu_cmd_22);
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_cmd_41")))
	{
		cmdUI->SetCheck(m_bCheckmenu_cmd_41);
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_cmd_42")))
	{
		cmdUI->SetCheck(m_bCheckmenu_cmd_42);
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_cmd_5")))
	{
		cmdUI->SetCheck(m_nMenuSelected == 0);
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_cmd_6")))
	{
		cmdUI->SetCheck(m_nMenuSelected == 1);
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_lang_chs")))
	{
		cmdUI->SetCheck(CLangManagerUI::GetLangName() == _T("chs"));
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_lang_ChineseTraditional")))
	{
		cmdUI->SetCheck(CLangManagerUI::GetLangName() == _T("chtrad"));
		return true;
	}
	if(IsMenuCommand(cmdUI, _T("menu_lang_english")))
	{
		cmdUI->SetCheck(CLangManagerUI::GetLangName() == _T("en"));
		return true;
	}
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{
// 	if(IsControl(msg, _T("btn_showhide_top_pane")))
// 		OnShowHideTopPane();

	//ʹ��UI_COMMAND��Ϣӳ��
//	UI_COMMAND(_T("btn_showhide_bottom_pane"), OnShowHideBottomPane);

// 	if(IsControl(msg, _T("btn_showhide_left_pane")))
// 	{
// 		CAnimationPaneUI *pPane;
// 		UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_3"));
// 		pPane->SetPaneVisible(!pPane->IsPaneVisible());
// 	}
// 	
// 	if(IsControl(msg, _T("btn_showhide_right_pane")))
// 	{
// 		CAnimationPaneUI *pPane;
// 		UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_4"));
// 		pPane->SetPaneVisible(!pPane->IsPaneVisible());
// 	}

	if(IsControl(msg, _T("btn_domodal")))
	{
		CDialog1 dlg;
		dlg.DoModal(this);
		GetManager()->RebuildFont(GetManager()->GetFontInfo(-1));
	}

	if(IsControl(msg, _T("btn_show_dialog")))
	{
		CDialog1 *pDlg = new CDialog1;
		pDlg->ShowDialog(this);
	}

	if(IsControl(msg, _T("windowmenubtn")))
	{
		//���ò˵�ѡ����Ϣ
		//CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

		//����״̬
		//CMenuWnd::SetMenuItemInfo(_T("menu_cmd_2"), m_bCheckmenu_cmd_2 == TRUE);

		//if(m_pMenuWnd){ delete m_pMenuWnd; m_pMenuWnd = NULL; }
		m_pMenuWnd = CreateMenu(_T("Menu.xml"));
	}

	if(IsControl(msg, _T("dpi_96")))
	{
		GetManager()->SetDPI(96);
		GetManager()->ResetDPIAssets();
	}
	if(IsControl(msg, _T("dpi_120")))
	{
		GetManager()->SetDPI(120);
		GetManager()->ResetDPIAssets();
	}
	if(IsControl(msg, _T("dpi_144")))
	{
		GetManager()->SetDPI(144);
		GetManager()->ResetDPIAssets();
	}
	if(IsControl(msg, _T("dpi_168")))
	{
		GetManager()->SetDPI(168);
		GetManager()->ResetDPIAssets();
	}
	if(IsControl(msg, _T("dpi_192")))
	{
		GetManager()->SetDPI(192);
		GetManager()->ResetDPIAssets();
	}
}

void CMainFrame::OnNotifyTimer(TNotifyUI& msg)
{
	if(msg.pSender == m_pProgress)
	{
		int nNewValue = m_pProgress->GetValue() + 1;
		if(nNewValue > m_pProgress->GetMaxValue()) nNewValue = m_pProgress->GetMinValue();
		m_pProgress->SetValue(nNewValue);
	}
}

void CMainFrame::OnShowHideTopPane()
{
// 	CAnimationPaneUI *pPane;
// 	UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_1"));
// 	pPane->SetPaneVisible(!pPane->IsPaneVisible());
}

void CMainFrame::OnShowHideBottomPane()
{
// 	CAnimationPaneUI *pPane;
// 	UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_2"));
// 	pPane->SetPaneVisible(!pPane->IsPaneVisible());
}