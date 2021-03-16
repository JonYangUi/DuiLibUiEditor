#pragma once

//namespace DuiLib{
using namespace DuiLib;


class UILIB_API CUIFrmBase : public CNotifyPump
{

public:
	CUIFrmBase(void);
	virtual ~CUIFrmBase(void);

	BOOL IsControl(TNotifyUI& msg, LPCTSTR name) { return _tcsicmp(msg.pSender->GetName(), name) == 0; }
	BOOL IsClass(TNotifyUI& msg, LPCTSTR classname) { return _tcsicmp(msg.pSender->GetClass(), classname) == 0; }
	BOOL IsInterface(TNotifyUI &msg, LPCTSTR pstrName) { return msg.pSender->GetInterface(pstrName) != NULL; }
	CControlUI *FindControl(LPCTSTR pstrName){ return GetManager()->FindControl(pstrName); }
	BOOL IsMenuCommand(const MenuCmd *cmd, LPCTSTR name) { return _tcsicmp(cmd->szName, name) == 0; }
	BOOL IsMenuCommand(CMenuCmdUI *cmdUI, LPCTSTR name) { return _tcsicmp(cmdUI->GetName(), name) == 0; }

	virtual CPaintManagerUI *GetManager() = 0;

	CMenuWnd *CreateMenu(STRINGorID xml);

	virtual void Notify(TNotifyUI& msg);

	//��Ϣ����
	virtual void OnNotifyClick(TNotifyUI& msg){}
	virtual void OnNotifyRClick(TNotifyUI& msg){}
	virtual void OnNotifyDbClick(TNotifyUI& msg){}
	virtual void OnNotifyMenu(TNotifyUI& msg) {}
	virtual void OnNotifyLink(TNotifyUI& msg) {}
	virtual void OnNotifyTimer(TNotifyUI& msg) {}
	virtual void OnNotifyReturn(TNotifyUI& msg) {}
	virtual void OnNotifyScroll(TNotifyUI& msg) {}
	virtual void OnNotifyPreDropDown(TNotifyUI& msg) {}
	virtual void OnNotifyDropDown(TNotifyUI& msg) {}
	virtual void OnNotifySetFocus(TNotifyUI& msg) {}
	virtual void OnNotifyKillFocus(TNotifyUI& msg) {}
	virtual void OnNotifyItemClick(TNotifyUI& msg) {}
	virtual void OnNotifyItemRClick(TNotifyUI& msg) {}
	virtual void OnNotifyTabSelect(TNotifyUI& msg) {}
	virtual void OnNotifyItemSelect(TNotifyUI& msg) {}
	virtual void OnNotifyItemExpand(TNotifyUI& msg) {}
	virtual void OnNotifyWindowPrepare(TNotifyUI& msg) {}
	virtual void OnNotifyButtonDown(TNotifyUI& msg) {}
	virtual void OnNotifyMouseEnter(TNotifyUI& msg) {}
	virtual void OnNotifyMouseLeave(TNotifyUI& msg) {}
	virtual void OnNotifyTextChanged(TNotifyUI& msg) {}
	virtual void OnNotifyHeaderClick(TNotifyUI& msg) {}
	virtual void OnNotifyItemDBClick(TNotifyUI& msg) {}
	virtual void OnNotifyShowActivex(TNotifyUI& msg) {}
	virtual void OnNotifyItemCollapse(TNotifyUI& msg) {}
	virtual void OnNotifyItemActivate(TNotifyUI& msg) {}
	virtual void OnNotifyValueChanged(TNotifyUI& msg) {}
	virtual void OnNotifySelectChanged(TNotifyUI& msg) {}   

	virtual void OnNotifyDrawItem(TNotifyUI& msg) {}
	virtual void OnNotifySortItem(TNotifyUI& msg) {}
	virtual void OnNotifyStartEdit(TNotifyUI& msg) {}
	virtual void OnNotifyEndEdit(TNotifyUI& msg) {}

	virtual void OnNotityGridStartEdit(TNotifyUI &msg) {}
	virtual void OnNotityGridEndEdit(TNotifyUI &msg) {}
	virtual void OnNotityGridStartSelChange(TNotifyUI &msg) {}
	virtual void OnNotityGridEndSelChange(TNotifyUI &msg) {}
	virtual void OnNotityGridDropDownList(TNotifyUI &msg) {}
	virtual void OnNotityGridGetDispInfo(TNotifyUI &msg) {}

	virtual void OnNotifySciNotify(TNotifyUI &msg) {}

	//���ղ˵���Ϣ������true�����������·�����Ϣ
	virtual bool OnMenuCommand(const MenuCmd *cmd) { return false; }

	virtual bool OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI) { return false; }

	//�Զ�����Ϣ����, ����TRUE, ��ʾ�Ѿ�����, �ײ㲻������������Ϣ, 
	virtual bool OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return FALSE; }

	DUI_DECLARE_MESSAGE_MAP()
};

//}