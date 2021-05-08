#ifndef __UITABCTRL_H__
#define __UITABCTRL_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CTabCtrlUI : public COptionUI
	{
		DECLARE_DUICONTROL(CTabCtrlUI)
	public:
		CTabCtrlUI();
		~CTabCtrlUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool Activate();
		void DoEvent(TEventUI& event);

		virtual bool IsSelected() const;

		virtual void BindTabIndex(int _BindTabIndex);
		virtual void BindTabLayoutName(LPCTSTR _TabLayoutName);
		virtual void BindTriggerTabSel(int _SetSelectIndex = -1);
		virtual void RemoveBindTabIndex();
		virtual int	 GetBindTabLayoutIndex();
		virtual LPCTSTR GetBindTabLayoutName();

		void SetActiveMouseOn(bool bActive);
		bool IsActiveMouseOn();

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		int			m_iBindTabIndex;
		CDuiString	m_sBindTabLayoutName;
		BOOL		m_bActiveMouseOn;
	};

} // namespace DuiLib

#endif // __UITABCTRL_H__