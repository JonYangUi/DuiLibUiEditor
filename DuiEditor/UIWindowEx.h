#pragma once
#include "UIWindow.h"
#include "UITrackerMuliti.h"
#include <list>
// CUIWindowEx

class CUIWindowEx : public CUIWindow, public WindowImplBase
{
public:
	CUIWindowEx();
	virtual ~CUIWindowEx();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return _T(""); }
	virtual LPCTSTR GetWindowClassName(void) const { return _T("UIWindowEx"); }

#ifdef DUILIB_VERSION_ORIGINAL
	virtual CPaintManagerUI *GetManager() { return &m_PaintManager; }
#else
	virtual CPaintManagerUI *GetManager() { return &m_pm; }
#endif
	virtual HWND GetSafeHwnd() { return GetHWND(); }
	virtual HWND CreateUiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle, DWORD dwExStyle) 
	{ return CreateDuiWindow(hwndParent, pstrWindowName, dwStyle, dwExStyle); }
public:
	CUITrackerMuliti m_tracker;
	bool m_bOpenConfigFile;

public:
	void AddNewControlFromToolBox(xml_node nodeToolBox, CPoint pt);
	std::list<CControlUI *>	m_listTrackerSeleted;
	void OnSelectingControl(CControlUI *pControl, const CRect &rcTracker);
	void ResizeWindow();
public:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


