#pragma once

#ifndef HEADER_UIDataExchange_HPP
#define HEADER_UIDataExchange_HPP

namespace DuiLib
{

#define UI_BINDCONTROL(classname, pControl, controlname)  { pControl = static_cast<classname *>(FindControl(controlname)); ASSERT(pControl); }
#define UI_BINDSUBCONTROL(parent, classname, pControl, controlname)  {											\
	CContainerUI *pContainerParent = static_cast<CContainerUI *>(parent->GetInterface(DUI_CTR_CONTAINER));		\
	ASSERT(pContainerParent);																					\
	pControl = static_cast<classname *>(pContainerParent->FindSubControl(controlname));							\
	ASSERT(pControl); }
#define UI_COMMAND(controlname, fun) if(IsControl(msg, controlname)) { fun(); return; }

class CUIDataExchange
{
public:
	//�󶨵���������
	enum _ddx_value_type
	{
		_value_null = 0,
		_value_bool,
		_value_BOOL,
		_value_int,
		_value_duistring,
#ifdef _AFX
		_value_cstring,
		_value_coledatetime,
		_value_colecurrency,
#endif
	};

	//�󶨵Ŀؼ�����
	enum _ddx_control_type
	{
		//�������ʶ�ģ�ӵ���������SetText()��GetText()�Ŀؼ�
		_control_text = 0,  

		//Option��ؼ�
		_control_checkbox, 

		//combo�ؼ�
		_control_combo,	
		_control_combo_ex,	

		//tablayout, cursel
		_control_tablayout,
	};

	struct _ddx_data 
	{
		CControlUI *pControl;
		_ddx_control_type controlType;
		_ddx_value_type valueType;
		PVOID pValue;
	};
public:
	CUIDataExchange(void);
	virtual ~CUIDataExchange(void);

	//�������pContainer��������ʱ�����Ȳ���pContainer�����еĿؼ��� ����Խ��ĳЩ�ӿؼ����ò����������⡣
	void ddxSetManager(CPaintManagerUI *pManager, CControlUI *pContainer=NULL);

	//true: ����>>����; false: ����>>����
	bool UpdateDataUI(bool bSaveAndValidate); 

	//
	bool ddxText(CControlUI *pControl, CDuiString &va);
	bool ddxText(LPCTSTR pControlName, CDuiString &va);
	bool ddxText(CControlUI *pControl, int &va);
	bool ddxText(LPCTSTR pControlName, int &va);

#ifdef _AFX
	bool ddxText(CControlUI *pControl, CString &va);
	bool ddxText(LPCTSTR pControlName, CString &va);
	bool ddxText(CControlUI *pControl, COleDateTime &va);
	bool ddxText(LPCTSTR pControlName, COleDateTime &va);
	bool ddxText(CControlUI *pControl, COleCurrency &va);
	bool ddxText(LPCTSTR pControlName, COleCurrency &va);
#endif

	bool ddxCheckBox(CControlUI *pControl, bool &va);
	bool ddxCheckBox(LPCTSTR pControlName, bool &va);
	bool ddxCheckBox(CControlUI *pControl, BOOL &va);
	bool ddxCheckBox(LPCTSTR pControlName, BOOL &va);

	bool ddxCombo(CControlUI *pControl, int &va); //cursel
	bool ddxCombo(LPCTSTR pControlName, int &va); //cursel
	bool ddxComboItemData(CControlUI *pControl, int &va); //��ComboItemData��ʹ�ó���: combo�����id��
	bool ddxComboItemData(LPCTSTR pControlName, int &va); //��ComboItemData��ʹ�ó���: combo�����id��

	bool ddxTabLayout(CControlUI *pControl, int &va); //cursel
	bool ddxTabLayout(LPCTSTR pControlName, int &va); //cursel
protected:
	bool ddxText(CControlUI *pControl, PVOID pValue, _ddx_value_type type);
	bool _UpdateText(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateCheckBox(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateCombo(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateComboEx(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateTabLayout(_ddx_data *pData, bool bSaveAndValidate);
private:
	CPaintManagerUI *m_pManager;
	CContainerUI *m_pRoot;
	CStdPtrArray _ddxDataArray;

//////////////////////////////////////////////////////////////////////////
//һЩ���õ�ת�����ṩ��̬����
public:
#ifdef _AFX
	static COleDateTime MergeCOleDateTime(const SYSTEMTIME &date, const SYSTEMTIME &time);
	static COleDateTime MergeCOleDateTime(const COleDateTime &date, const COleDateTime &time);
#endif
};

} //namespace DuiLib

#endif //#define HEADER_UIDataExchange_HPP



#ifndef UIDataExchange_SOURCE
#	define UIDataExchange_SOURCE "UIDataExchange.cpp"
#	include UIDataExchange_SOURCE
#endif
