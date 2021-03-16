#pragma once

#ifndef HEADER_UIDataExchange_HPP
#define HEADER_UIDataExchange_HPP

namespace DuiLib
{

#define UI_BINDCONTROL(classname, pControl, controlname)  { pControl = static_cast<classname *>(FindControl(controlname)); ASSERT(pControl); }
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

	void ddxSetManager(CPaintManagerUI *pManager);
	bool UpdateDataUI(bool bSaveAndValidate); //true: ����>>����; false: ����>>����

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
protected:
	bool ddxText(CControlUI *pControl, PVOID pValue, _ddx_value_type type);
	bool _UpdateText(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateCheckBox(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateCombo(_ddx_data *pData, bool bSaveAndValidate);
private:
	CPaintManagerUI *m_pManager;
	CStdPtrArray m_arrData;

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
