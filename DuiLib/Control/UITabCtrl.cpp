#include "stdafx.h"
#include "UITabCtrl.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CTabCtrlUI)

	CTabCtrlUI::CTabCtrlUI()
		: m_iBindTabIndex(-1)
	{
	}

	CTabCtrlUI::~CTabCtrlUI()
	{
		
	}

	LPCTSTR CTabCtrlUI::GetClass() const
	{
		return DUI_CTR_TABCTRL;
	}

	LPVOID CTabCtrlUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_TABCTRL) == 0 ) 
			return static_cast<COptionUI*>(this);
		return COptionUI::GetInterface(pstrName);
	}

	bool CTabCtrlUI::Activate()
	{
		if( !COptionUI::Activate() ) return false;
		
		BindTriggerTabSel();
		return true;
	}

	bool CTabCtrlUI::IsSelected() const
	{
		LPCTSTR pstrName = m_sBindTabLayoutName;
		if(pstrName == NULL || m_iBindTabIndex < 0) 
		{
			return __super::IsSelected();
		}

		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
		if(!pTabLayout) return __super::IsSelected();

		if(pTabLayout->GetCurSel() == m_iBindTabIndex)
			return true;

		return false;
	}

	void CTabCtrlUI::BindTabIndex(int _BindTabIndex )
	{
		if( _BindTabIndex >= 0)
			m_iBindTabIndex	= _BindTabIndex;
	}

	void CTabCtrlUI::BindTabLayoutName( LPCTSTR _TabLayoutName )
	{
		if(_TabLayoutName)
			m_sBindTabLayoutName = _TabLayoutName;
	}

	void CTabCtrlUI::BindTriggerTabSel( int _SetSelectIndex /*= -1*/ )
	{
		LPCTSTR pstrName = GetBindTabLayoutName();
		if(pstrName == NULL || (GetBindTabLayoutIndex() < 0 && _SetSelectIndex < 0))
			return;

		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
		if(!pTabLayout) return;

		int sel = _SetSelectIndex >=0?_SetSelectIndex : GetBindTabLayoutIndex();
		if(pTabLayout->GetCurSel() == sel)
			return;

		pTabLayout->SelectItem(sel);
		Selected(true);
	}

	void CTabCtrlUI::RemoveBindTabIndex()
	{
		m_iBindTabIndex	= -1;
		m_sBindTabLayoutName.Empty();
	}

	int CTabCtrlUI::GetBindTabLayoutIndex()
	{
		return m_iBindTabIndex;
	}

	LPCTSTR CTabCtrlUI::GetBindTabLayoutName()
	{
		return m_sBindTabLayoutName;
	}

	void CTabCtrlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("bindtabindex")) == 0 ) 
			BindTabIndex(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("bindtablayoutname")) == 0 ) 
			BindTabLayoutName(pstrValue);
		else 
			__super::SetAttribute(pstrName, pstrValue);
	}
}