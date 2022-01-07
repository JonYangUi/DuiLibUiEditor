#include "StdAfx.h"
#include <zmouse.h>

namespace DuiLib {

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	static void GetChildWndRect(HWND hWnd, HWND hChildWnd, RECT& rcChildWnd)
	{
		::GetWindowRect(hChildWnd, &rcChildWnd);

		POINT pt;
		pt.x = rcChildWnd.left;
		pt.y = rcChildWnd.top;
		::ScreenToClient(hWnd, &pt);
		rcChildWnd.left = pt.x;
		rcChildWnd.top = pt.y;

		pt.x = rcChildWnd.right;
		pt.y = rcChildWnd.bottom;
		::ScreenToClient(hWnd, &pt);
		rcChildWnd.right = pt.x;
		rcChildWnd.bottom = pt.y;
	}

	static UINT MapKeyState()
	{
		UINT uState = 0;
		if( ::GetKeyState(VK_CONTROL) < 0 ) uState |= MK_CONTROL;
		if( ::GetKeyState(VK_LBUTTON) < 0 ) uState |= MK_LBUTTON;
		if( ::GetKeyState(VK_RBUTTON) < 0 ) uState |= MK_RBUTTON;
		if( ::GetKeyState(VK_SHIFT) < 0 ) uState |= MK_SHIFT;
		if( ::GetKeyState(VK_MENU) < 0 ) uState |= MK_ALT;
		return uState;
	}

	typedef struct tagFINDTABINFO
	{
		CControlUI* pFocus;
		CControlUI* pLast;
		bool bForward;
		bool bNextIsIt;
	} FINDTABINFO;

	typedef struct tagFINDSHORTCUT
	{
		TCHAR ch;
		bool bPickNext;
	} FINDSHORTCUT;

	typedef struct tagTIMERINFO
	{
		CControlUI* pSender;
		UINT nLocalID;
		HWND hWnd;
		UINT uWinTimer;
		bool bKilled;
	} TIMERINFO;

	/////////////////////////////////////////////////////////////////////////////////////
	typedef BOOL (__stdcall *PFUNCUPDATELAYEREDWINDOW)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);
	PFUNCUPDATELAYEREDWINDOW g_fUpdateLayeredWindow = NULL;

	HPEN m_hUpdateRectPen = NULL;

	HINSTANCE CPaintManagerUI::m_hResourceInstance = NULL;
	CDuiString CPaintManagerUI::m_pStrResourcePath;
	CDuiString CPaintManagerUI::m_pStrResourceZip;
	CDuiString CPaintManagerUI::m_pStrResourceZipPwd;  //Garfield 20160325 ������zip������
	HANDLE CPaintManagerUI::m_hResourceZip = NULL;
	bool CPaintManagerUI::m_bCachedResourceZip = true;
	int CPaintManagerUI::m_nResType = UILIB_FILE;
	TResInfo CPaintManagerUI::m_SharedResInfo;
	HINSTANCE CPaintManagerUI::m_hInstance = NULL;
	bool CPaintManagerUI::m_bUseHSL = false;
	bool CPaintManagerUI::m_bForceHSL = false;
	short CPaintManagerUI::m_H = 180;
	short CPaintManagerUI::m_S = 100;
	short CPaintManagerUI::m_L = 100;
	CStdPtrArray CPaintManagerUI::m_aPreMessages;
	CStdPtrArray CPaintManagerUI::m_aPlugins;

	ULONG_PTR CPaintManagerUI::m_gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput *CPaintManagerUI::m_pGdiplusStartupInput = NULL;

	BOOL CPaintManagerUI::UIDESIGNMODE = FALSE;
	BOOL CPaintManagerUI::UIDESIGNPREVIEW = FALSE;

	emRenderEngine CPaintManagerUI::m_emRenderEngine = DuiLib_Render_Default;

	CPaintManagerUI::CPaintManagerUI() :
		m_hWndPaint(NULL),
		m_hDcPaint(NULL),
		m_hwndTooltip(NULL),
		m_uTimerID(0x1000),
		m_pRoot(NULL),
		m_pFocus(NULL), m_pLastFocus(NULL),
		m_pEventHover(NULL),
		m_pEventClick(NULL),
		m_pEventKey(NULL),
		m_bFirstLayout(true),
		m_bFocusNeeded(false),
		m_bUpdateNeeded(false),
		m_bMouseTracking(false),
		m_bMouseCapture(false),
		m_bUsedVirtualWnd(false),
		m_bAsyncNotifyPosted(false), //add by liqs99, ԭ��������Աû�г�ʼ��
		m_bForceUseSharedRes(false),
		m_nOpacity(0xFF),
		m_bLayered(false),
		m_bLayeredChanged(false),
		m_bShowUpdateRect(false),
		m_bUseGdiplusText(false),
		m_trh(0),
		m_bDragMode(false),
		m_pDPI(NULL),
		m_iHoverTime(400UL),
		m_bLockUpdate(false)
	{
		if(m_pGdiplusStartupInput == NULL)
		{
			m_pGdiplusStartupInput = new Gdiplus::GdiplusStartupInput;
			Gdiplus::GdiplusStartup( &m_gdiplusToken, m_pGdiplusStartupInput, NULL); // ����GDI�ӿ�
		}

		if (!m_SharedResInfo.m_DefaultFontInfo)
		{
			m_SharedResInfo.m_dwDefaultDisabledColor = 0xFFA7A6AA;
			m_SharedResInfo.m_dwDefaultFontColor = 0xFF000000;
			m_SharedResInfo.m_dwDefaultLinkFontColor = 0xFF0000FF;
			m_SharedResInfo.m_dwDefaultLinkHoverFontColor = 0xFFD3215F;
			m_SharedResInfo.m_dwDefaultSelectedBkColor = 0xFFBAE4FF;

			m_SharedResInfo.m_DefaultFontInfo = MakeRefPtr<UIFont>(UIGlobal::CreateFont());
			m_SharedResInfo.m_DefaultFontInfo->CreateDefaultFont();		
		}

		m_ResInfo.m_dwDefaultDisabledColor = m_SharedResInfo.m_dwDefaultDisabledColor;
		m_ResInfo.m_dwDefaultFontColor = m_SharedResInfo.m_dwDefaultFontColor;
		m_ResInfo.m_dwDefaultLinkFontColor = m_SharedResInfo.m_dwDefaultLinkFontColor;
		m_ResInfo.m_dwDefaultLinkHoverFontColor = m_SharedResInfo.m_dwDefaultLinkHoverFontColor;
		m_ResInfo.m_dwDefaultSelectedBkColor = m_SharedResInfo.m_dwDefaultSelectedBkColor;
		m_ResInfo.m_DefaultFontInfo = MakeRefPtr<UIFont>(UIGlobal::CreateFont());
		//m_ResInfo.m_DefaultFontInfo->CreateDefaultFont();		


		if( m_hUpdateRectPen == NULL ) {
			m_hUpdateRectPen = ::CreatePen(PS_SOLID, 1, RGB(220, 0, 0));
			// Boot Windows Common Controls (for the ToolTip control)
			::InitCommonControls();
			::LoadLibrary(_T("msimg32.dll"));
		}

		m_szMinWindow.cx = 0;
		m_szMinWindow.cy = 0;
		m_szMaxWindow.cx = 0;
		m_szMaxWindow.cy = 0;
		m_szInitWindowSize.cx = 0;
		m_szInitWindowSize.cy = 0;
		m_szRoundCorner.cx = m_szRoundCorner.cy = 0;
		::ZeroMemory(&m_rcSizeBox, sizeof(m_rcSizeBox));
		::ZeroMemory(&m_rcCaption, sizeof(m_rcCaption));
		::ZeroMemory(&m_rcLayeredInset, sizeof(m_rcLayeredInset));
		::ZeroMemory(&m_rcLayeredUpdate, sizeof(m_rcLayeredUpdate));
		m_ptLastMousePos.x = m_ptLastMousePos.y = -1;


		CShadowUI::Initialize(m_hInstance);

		m_pLangManager = new CLangManagerUI;
		m_bInitWindowParameter = false;
	}

	CPaintManagerUI::~CPaintManagerUI()
	{
		if(m_pLangManager){ delete m_pLangManager; m_pLangManager = NULL; }

		// Delete the control-tree structures
		for( int i = 0; i < m_aDelayedCleanup.GetSize(); i++ ) delete static_cast<CControlUI*>(m_aDelayedCleanup[i]);
		m_aDelayedCleanup.Resize(0);
		for( int i = 0; i < m_aAsyncNotify.GetSize(); i++ ) delete static_cast<TNotifyUI*>(m_aAsyncNotify[i]);
		m_aAsyncNotify.Resize(0);

		m_mNameHash.Resize(0);
		if( m_pRoot != NULL ) delete m_pRoot;

		//delete m_ResInfo.m_DefaultFontInfo;
		RemoveAllFonts();
		RemoveAllImages();
		RemoveAllStyle();
		RemoveAllDefaultAttributeList();
		RemoveAllWindowCustomAttribute();
		RemoveAllOptionGroups();
		RemoveAllTimers();
		RemoveAllDrawInfos();

		if( m_hwndTooltip != NULL ) {
			::DestroyWindow(m_hwndTooltip);
			m_hwndTooltip = NULL;
		}
		if (!m_aFonts.IsEmpty()) {
			for (int i = 0; i < m_aFonts.GetSize();++i)
			{
				HANDLE handle = static_cast<HANDLE>(m_aFonts.GetAt(i));
				::RemoveFontMemResourceEx(handle);
			}
		}
		if( m_hDcPaint != NULL ) ::ReleaseDC(m_hWndPaint, m_hDcPaint);
		m_aPreMessages.Remove(m_aPreMessages.Find(this));


		// DPI�������
		if (m_pDPI != NULL) {
			delete m_pDPI;
			m_pDPI = NULL;
		}
		
		if(m_pRenderEngine)
		{
			//m_pRenderEngine->Release();
			m_pRenderEngine = NULL;
		}
	}

	void CPaintManagerUI::Init(HWND hWnd, LPCTSTR pstrName)
	{
		ASSERT(::IsWindow(hWnd));

		m_mNameHash.Resize();
		RemoveAllFonts();
		RemoveAllImages();
		RemoveAllStyle();
		RemoveAllDefaultAttributeList();
		RemoveAllWindowCustomAttribute();
		RemoveAllOptionGroups();
		RemoveAllTimers();

		m_sName.Empty();
		if( pstrName != NULL ) m_sName = pstrName;

		if( m_hWndPaint != hWnd ) {
			m_hWndPaint = hWnd;
			m_hDcPaint = ::GetDC(hWnd);
			m_aPreMessages.Add(this);

			Render()->Init(this, m_hDcPaint);
		}

		SetTargetWnd(hWnd);
		InitDragDrop();
	}

	void CPaintManagerUI::SetRenderEngineType(emRenderEngine render)
	{
		if(m_emRenderEngine == render)
			return;
		m_emRenderEngine = render;
		UIGlobal::Release();
		for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) 
		{
			CPaintManagerUI* pT = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			//pT->m_pRenderEngine->Release();
			pT->m_pRenderEngine = NULL;
		}
	}

	emRenderEngine CPaintManagerUI::GetRenderEngineType()
	{
		return m_emRenderEngine;
	}

	UIRender *CPaintManagerUI::Render()
	{
		if(!m_pRenderEngine)
		{
			m_pRenderEngine = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
			m_pRenderEngine->Init(this, m_hDcPaint);
		}
		ASSERT(m_pRenderEngine);
		return m_pRenderEngine;
	}

	void CPaintManagerUI::DeletePtr(void* ptr)
	{
		if(ptr) {delete ptr; ptr = NULL;}
	}

	HINSTANCE CPaintManagerUI::GetInstance()
	{
		return m_hInstance;
	}

	CDuiString CPaintManagerUI::GetInstancePath()
	{
		if( m_hInstance == NULL ) return _T('\0');

		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetModuleFileName(m_hInstance, tszModule, MAX_PATH);
		CDuiString sInstancePath = tszModule;
		int pos = sInstancePath.ReverseFind(_T('\\'));
		if( pos >= 0 ) sInstancePath = sInstancePath.Left(pos + 1);
		return sInstancePath;
	}

	CDuiString CPaintManagerUI::GetCurrentPath()
	{
		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetCurrentDirectory(MAX_PATH, tszModule);
		return tszModule;
	}

	HINSTANCE CPaintManagerUI::GetResourceDll()
	{
		if( m_hResourceInstance == NULL ) return m_hInstance;
		return m_hResourceInstance;
	}

	const CDuiString& CPaintManagerUI::GetResourcePath()
	{
		return m_pStrResourcePath;
	}

	const CDuiString& CPaintManagerUI::GetResourceZip()
	{
		return m_pStrResourceZip;
	}

	const CDuiString& CPaintManagerUI::GetResourceZipPwd()
	{
		return m_pStrResourceZipPwd;
	}

	bool CPaintManagerUI::IsCachedResourceZip()
	{
		return m_bCachedResourceZip;
	}

	HANDLE CPaintManagerUI::GetResourceZipHandle()
	{
		return m_hResourceZip;
	}

	void CPaintManagerUI::SetInstance(HINSTANCE hInst)
	{
		m_hInstance = hInst;
	}

	void CPaintManagerUI::SetCurrentPath(LPCTSTR pStrPath)
	{
		::SetCurrentDirectory(pStrPath);
	}

	void CPaintManagerUI::SetResourceDll(HINSTANCE hInst)
	{
		m_hResourceInstance = hInst;
	}

	void CPaintManagerUI::SetResourcePath(LPCTSTR pStrPath)
	{
		m_pStrResourcePath = pStrPath;
		if( m_pStrResourcePath.IsEmpty() ) return;
		TCHAR cEnd = m_pStrResourcePath.GetAt(m_pStrResourcePath.GetLength() - 1);
		if( cEnd != _T('\\') && cEnd != _T('/') ) m_pStrResourcePath += _T('\\');
	}

	void CPaintManagerUI::SetResourceZip(LPVOID pVoid, unsigned int len, LPCTSTR password)
	{
		if( m_pStrResourceZip == _T("membuffer") ) return;
		if( m_bCachedResourceZip && m_hResourceZip != NULL ) {
			CloseZip((HZIP)m_hResourceZip);
			m_hResourceZip = NULL;
		}
		m_pStrResourceZip = _T("membuffer");
		m_bCachedResourceZip = true;
		m_pStrResourceZipPwd = password;  //Garfield 20160325 ������zip������
		if( m_bCachedResourceZip ) 
		{
			UISTRING_CONVERSION;
			m_hResourceZip = (HANDLE)OpenZip(pVoid, len, UIT2A(password));
		}
	}

	void CPaintManagerUI::SetResourceZip(LPCTSTR pStrPath, bool bCachedResourceZip, LPCTSTR password)
	{
		if( m_pStrResourceZip == pStrPath && m_bCachedResourceZip == bCachedResourceZip ) return;
		if( m_bCachedResourceZip && m_hResourceZip != NULL ) {
			CloseZip((HZIP)m_hResourceZip);
			m_hResourceZip = NULL;
		}
		m_pStrResourceZip = pStrPath;
		m_bCachedResourceZip = bCachedResourceZip;
		m_pStrResourceZipPwd = password;
		if( m_bCachedResourceZip ) {
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			sFile += CPaintManagerUI::GetResourceZip();
			UISTRING_CONVERSION;
			m_hResourceZip = (HANDLE)OpenZip(sFile.GetData(), UIT2A(password));
		}
	}

	void CPaintManagerUI::SetResourceType(int nType)
	{
		m_nResType = nType;
	}

	int CPaintManagerUI::GetResourceType()
	{
		return m_nResType;
	}

	bool CPaintManagerUI::GetHSL(short* H, short* S, short* L)
	{
		*H = m_H;
		*S = m_S;
		*L = m_L;
		return m_bUseHSL;
	}

	void CPaintManagerUI::SetHSL(short H, short S, short L)
	{
		bool bUseHSL = H != 180 || S != 100 || L != 100;
		if( m_bUseHSL || m_bUseHSL != bUseHSL ) 
		{
			m_bUseHSL = bUseHSL;
			if( H == m_H && S == m_S && L == m_L ) return;
			m_H = CLAMP(H, 0, 360);
			m_S = CLAMP(S, 0, 200);
			m_L = CLAMP(L, 0, 200);
			AdjustSharedImagesHSL();
			for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) {
				CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
				if( pManager != NULL ) pManager->AdjustImagesHSL();
			}
		}
	}

	bool CPaintManagerUI::IsUseHSL()
	{
		return m_bUseHSL;
	}

	void CPaintManagerUI::SetForceHSL(bool bForceHSL)
	{
		m_bForceHSL = bForceHSL;
	}

	bool CPaintManagerUI::IsForceHSL()
	{
		return m_bForceHSL;
	}

	void CPaintManagerUI::ReloadSkin()
	{
		ReloadSharedImages();
		for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) {
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			pManager->ReloadImages();
		}
	}

	CPaintManagerUI* CPaintManagerUI::GetPaintManager(LPCTSTR pstrName)
	{
		if( pstrName == NULL ) return NULL;
		CDuiString sName = pstrName;
		if( sName.IsEmpty() ) return NULL;
		for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) {
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			if( pManager != NULL && sName == pManager->GetName() ) return pManager;
		}
		return NULL;
	}

	CStdPtrArray* CPaintManagerUI::GetPaintManagers()
	{
		return &m_aPreMessages;
	}

	bool CPaintManagerUI::LoadPlugin(LPCTSTR pstrModuleName)
	{
		ASSERT( !::IsBadStringPtr(pstrModuleName,-1) || pstrModuleName == NULL );
		if( pstrModuleName == NULL ) return false;
		HMODULE hModule = ::LoadLibrary(pstrModuleName);
		if( hModule != NULL ) {
			LPCREATECONTROL lpCreateControl = (LPCREATECONTROL)::GetProcAddress(hModule, "CreateControl");
			if( lpCreateControl != NULL ) {
				if( m_aPlugins.Find(lpCreateControl) >= 0 ) return true;
				m_aPlugins.Add(lpCreateControl);
				return true;
			}
		}
		return false;
	}

	CStdPtrArray* CPaintManagerUI::GetPlugins()
	{
		return &m_aPlugins;
	}

	HWND CPaintManagerUI::GetPaintWindow() const
	{
		return m_hWndPaint;
	}

	HWND CPaintManagerUI::GetTooltipWindow() const
	{
		return m_hwndTooltip;
	}
	int CPaintManagerUI::GetHoverTime() const
	{
		return m_iHoverTime;
	}

	void CPaintManagerUI::SetHoverTime(int iTime)
	{
		m_iHoverTime = iTime;
	}

	LPCTSTR CPaintManagerUI::GetName() const
	{
		return m_sName;
	}

	HDC CPaintManagerUI::GetPaintDC() const
	{
		return m_hDcPaint;
	}

	POINT CPaintManagerUI::GetMousePos() const
	{
		return m_ptLastMousePos;
	}

	SIZE CPaintManagerUI::GetClientSize() const
	{
		RECT rcClient = { 0 };
		::GetClientRect(m_hWndPaint, &rcClient);
		return CDuiSize(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	}

	SIZE CPaintManagerUI::GetInitSize()
	{
		return m_szInitWindowSize;
	}

	void CPaintManagerUI::SetInitSize(int cx, int cy)
	{
		m_szInitWindowSize.cx = cx;
		m_szInitWindowSize.cy = cy;
		if( m_pRoot == NULL && m_hWndPaint != NULL ) {
			::SetWindowPos(m_hWndPaint, NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}

	RECT& CPaintManagerUI::GetSizeBox()
	{
		return m_rcSizeBox;
	}

	void CPaintManagerUI::SetSizeBox(RECT& rcSizeBox)
	{
		m_rcSizeBox = rcSizeBox;
	}

	RECT& CPaintManagerUI::GetCaptionRect()
	{
		return m_rcCaption;
	}

	void CPaintManagerUI::SetCaptionRect(RECT& rcCaption)
	{
		m_rcCaption = rcCaption;
	}

	SIZE CPaintManagerUI::GetRoundCorner() const
	{
		return m_szRoundCorner;
	}

	void CPaintManagerUI::SetRoundCorner(int cx, int cy)
	{
		m_szRoundCorner.cx = cx;
		m_szRoundCorner.cy = cy;
	}

	SIZE CPaintManagerUI::GetMinInfo() const
	{
		return m_szMinWindow;
	}

	void CPaintManagerUI::SetMinInfo(int cx, int cy)
	{
		ASSERT(cx>=0 && cy>=0);
		m_szMinWindow.cx = cx;
		m_szMinWindow.cy = cy;
	}

	SIZE CPaintManagerUI::GetMaxInfo() const
	{
		return m_szMaxWindow;
	}

	void CPaintManagerUI::SetMaxInfo(int cx, int cy)
	{
		ASSERT(cx>=0 && cy>=0);
		m_szMaxWindow.cx = cx;
		m_szMaxWindow.cy = cy;
	}

	bool CPaintManagerUI::IsShowUpdateRect() const
	{
		return m_bShowUpdateRect;
	}

	void CPaintManagerUI::SetShowUpdateRect(bool show)
	{
		m_bShowUpdateRect = show;
	}

	bool CPaintManagerUI::IsNoActivate()
	{
		return m_bNoActivate;
	}

	void CPaintManagerUI::SetNoActivate(bool bNoActivate)
	{
		m_bNoActivate = bNoActivate;
	}

	BYTE CPaintManagerUI::GetOpacity() const
	{
		return m_nOpacity;
	}

	void CPaintManagerUI::SetOpacity(BYTE nOpacity)
	{
		m_nOpacity = nOpacity;
		if( m_hWndPaint != NULL ) {
			typedef BOOL (__stdcall *PFUNCSETLAYEREDWINDOWATTR)(HWND, COLORREF, BYTE, DWORD);
			PFUNCSETLAYEREDWINDOWATTR fSetLayeredWindowAttributes = NULL;

			HMODULE hUser32 = ::GetModuleHandle(_T("User32.dll"));
			if (hUser32)
			{
				fSetLayeredWindowAttributes = 
					(PFUNCSETLAYEREDWINDOWATTR)::GetProcAddress(hUser32, "SetLayeredWindowAttributes");
				if( fSetLayeredWindowAttributes == NULL ) return;
			}

			DWORD dwStyle = ::GetWindowLong(m_hWndPaint, GWL_EXSTYLE);
			DWORD dwNewStyle = dwStyle;
			if( nOpacity >= 0 && nOpacity < 256 ) dwNewStyle |= WS_EX_LAYERED;
			else dwNewStyle &= ~WS_EX_LAYERED;
			if(dwStyle != dwNewStyle) ::SetWindowLong(m_hWndPaint, GWL_EXSTYLE, dwNewStyle);
			fSetLayeredWindowAttributes(m_hWndPaint, 0, nOpacity, LWA_ALPHA);
		}
	}

	bool CPaintManagerUI::IsLayered()
	{
		return m_bLayered;
	}

	void CPaintManagerUI::SetLayered(bool bLayered)
	{
		if( m_hWndPaint != NULL && bLayered != m_bLayered ) {
			UINT uStyle = GetWindowStyle(m_hWndPaint);
			if( (uStyle & WS_CHILD) != 0 ) return;
			if( g_fUpdateLayeredWindow == NULL ) {
				HMODULE hUser32 = ::GetModuleHandle(_T("User32.dll"));
				if (hUser32) {
					g_fUpdateLayeredWindow = 
						(PFUNCUPDATELAYEREDWINDOW)::GetProcAddress(hUser32, "UpdateLayeredWindow");
					if( g_fUpdateLayeredWindow == NULL ) return;
				}
			}
			m_bLayered = bLayered;
			if( m_pRoot != NULL ) m_pRoot->NeedUpdate();
			Invalidate();
		}
	}

	RECT& CPaintManagerUI::GetLayeredInset()
	{
		return m_rcLayeredInset;
	}

	void CPaintManagerUI::SetLayeredInset(RECT& rcLayeredInset)
	{
		m_rcLayeredInset = rcLayeredInset;
		m_bLayeredChanged = true;
		Invalidate();
	}

	BYTE CPaintManagerUI::GetLayeredOpacity()
	{
		return m_nOpacity;
	}

	void CPaintManagerUI::SetLayeredOpacity(BYTE nOpacity)
	{
		m_nOpacity = nOpacity;
		m_bLayeredChanged = true;
		Invalidate();
	}

	LPCTSTR CPaintManagerUI::GetLayeredImage()
	{
		return m_diLayered.sDrawString;
	}

	void CPaintManagerUI::SetLayeredImage(LPCTSTR pstrImage)
	{
		m_diLayered.sDrawString = pstrImage;
		m_diLayered.Parse(pstrImage, NULL, this);
		RECT rcNull = {0};
		Render()->DrawImageInfo(rcNull, rcNull, &m_diLayered);
		m_bLayeredChanged = true;
		Invalidate();
	}

	TDrawInfo *CPaintManagerUI::GetLayeredDrawInfo() { return &m_diLayered; }

	void CPaintManagerUI::SetLayeredChanged(bool bChange) { m_bLayeredChanged = bChange; }

	bool CPaintManagerUI::IsLayeredChanged() const { return m_bLayeredChanged; }

	bool CPaintManagerUI::IsFocusNeeded() const { return m_bFocusNeeded; }

	CShadowUI* CPaintManagerUI::GetShadow()
	{
		return &m_shadow;
	}

	void CPaintManagerUI::SetUseGdiplusText(bool bUse)
	{
		m_bUseGdiplusText = bUse;
	}

	bool CPaintManagerUI::IsUseGdiplusText() const
	{
		return m_bUseGdiplusText;
	}

	void CPaintManagerUI::SetGdiplusTextRenderingHint(int trh)
	{
		m_trh = trh;
	}

	int CPaintManagerUI::GetGdiplusTextRenderingHint() const
	{
		return m_trh;
	}

	bool CPaintManagerUI::PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& /*lRes*/)
	{
		for( int i = 0; i < m_aPreMessageFilters.GetSize(); i++ ) 
		{
			bool bHandled = false;
			LRESULT lResult = static_cast<IMessageFilterUI*>(m_aPreMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
			if( bHandled ) {
				return true;
			}
		}
		switch( uMsg ) {
		case WM_KEYDOWN:
			{
				// Tabbing between controls
				if( wParam == VK_TAB ) {
					if( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("RichEditUI")) != NULL ) {
						if( static_cast<CRichEditUI*>(m_pFocus)->IsWantTab() ) return false;
					}
					if( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("WkeWebkitUI")) != NULL ) {
						return false;
					}
					SetNextTabControl(::GetKeyState(VK_SHIFT) >= 0);
					return true;
				}
			}
			break;
		case WM_SYSCHAR:
			{
				// Handle ALT-shortcut key-combinations
				FINDSHORTCUT fs = { 0 };
				fs.ch = toupper((int)wParam);
				CControlUI* pControl = m_pRoot->FindControl(__FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
				if( pControl != NULL ) {
					pControl->SetFocus();
					pControl->Activate();
					return true;
				}
			}
			break;
		case WM_SYSKEYDOWN:
			{
				if( m_pFocus != NULL ) {
					TEventUI event = { 0 };
					event.Type = UIEVENT_SYSKEY;
					event.chKey = (TCHAR)wParam;
					event.ptMouse = m_ptLastMousePos;
					event.wKeyState = MapKeyState();
					event.dwTimestamp = ::GetTickCount();
					m_pFocus->Event(event);
				}
			}
			break;
		}
		return false;
	}

	bool CPaintManagerUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_hWndPaint == NULL ) return false;
		// Cycle through listeners
		for( int i = 0; i < m_aMessageFilters.GetSize(); i++ ) 
		{
			bool bHandled = false;
			LRESULT lResult = static_cast<IMessageFilterUI*>(m_aMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
			if( bHandled ) {
				lRes = lResult;
				switch( uMsg ) {
				case WM_MOUSEMOVE:
				case WM_LBUTTONDOWN:
				case WM_LBUTTONDBLCLK:
				case WM_LBUTTONUP:
					{
						POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
						m_ptLastMousePos = pt;
					}
					break;
				case WM_CONTEXTMENU:
				case WM_MOUSEWHEEL:
					{
						POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
						::ScreenToClient(m_hWndPaint, &pt);
						m_ptLastMousePos = pt;
					}
					break;
				}
				return true;
			}
		}

		if( m_bLayered ) {
			switch( uMsg ) {
			case WM_NCACTIVATE:
				if( !::IsIconic(m_hWndPaint) ) {
					lRes = (wParam == 0) ? TRUE : FALSE;
					return true;
				}
				break;
			case WM_NCCALCSIZE:
			case WM_NCPAINT:
				lRes = 0;
				return true;
			}
		}
		// Custom handling of events
		switch( uMsg ) {
		case WM_APP + 1:
			{
				for( int i = 0; i < m_aDelayedCleanup.GetSize(); i++ ) 
					delete static_cast<CControlUI*>(m_aDelayedCleanup[i]);
				m_aDelayedCleanup.Empty();

				//CDuiInnerLock lock(&m_lockAsyncNotify); //add by liq99
				m_bAsyncNotifyPosted = false;

				TNotifyUI* pMsg = NULL;
				while( pMsg = static_cast<TNotifyUI*>(m_aAsyncNotify.GetAt(0)) ) {
					m_aAsyncNotify.Remove(0);
					if( pMsg->pSender != NULL ) {
						if( pMsg->pSender->OnNotify ) pMsg->pSender->OnNotify(pMsg);
						if(pMsg->pSender->m_asOnNotify) 
							ExecuteScript(pMsg->pSender->m_asOnNotify, pMsg->pSender, pMsg);
					}
					for( int j = 0; j < m_aNotifiers.GetSize(); j++ ) {
						static_cast<INotifyUI*>(m_aNotifiers[j])->Notify(*pMsg);
					}
					delete pMsg;
				}
			}
			break;
		case WM_CLOSE:
			{
				// Make sure all matching "closing" events are sent
				TEventUI event = { 0 };
				event.ptMouse = m_ptLastMousePos;
				event.wKeyState = MapKeyState();
				event.dwTimestamp = ::GetTickCount();
				if( m_pEventHover != NULL ) {
					event.Type = UIEVENT_MOUSELEAVE;
					event.pSender = m_pEventHover;
					m_pEventHover->Event(event);
				}
				if( m_pEventClick != NULL ) {
					event.Type = UIEVENT_BUTTONUP;
					event.pSender = m_pEventClick;
					m_pEventClick->Event(event);
				}

				SetFocus(NULL);

				if( ::GetActiveWindow() == m_hWndPaint ) {
					HWND hwndParent = GetWindowOwner(m_hWndPaint);
					if( hwndParent != NULL ) ::SetFocus(hwndParent);
				}

				if (m_hwndTooltip != NULL) {
					::DestroyWindow(m_hwndTooltip);
					m_hwndTooltip = NULL;
				}
			}
			break;
		case WM_ERASEBKGND:
			{
				// We'll do the painting here...
				lRes = 1;
			}
			return true;
		case WM_PAINT:
			{
				if( m_pRoot == NULL ) {
					PAINTSTRUCT ps = { 0 };
					::BeginPaint(m_hWndPaint, &ps);
					//Render()->DrawColor( ps.rcPaint, 0xFF000000);
					::EndPaint(m_hWndPaint, &ps);
					return true;
				}

				RECT rcClient = { 0 };
				::GetClientRect(m_hWndPaint, &rcClient);

				RECT rcPaint = { 0 };
				if( !::GetUpdateRect(m_hWndPaint, &rcPaint, FALSE) ) 
					return true;

				// Set focus to first control?
				if( m_bFocusNeeded ) {
					SetNextTabControl();
				}

				bool bNeedSizeMsg = false;
				DWORD dwWidth = rcClient.right - rcClient.left;
				DWORD dwHeight = rcClient.bottom - rcClient.top;

				SetPainting(true);
				if( m_bUpdateNeeded && !m_bLockUpdate) 
				{
					m_bUpdateNeeded = false;
					if( !::IsRectEmpty(&rcClient) && !::IsIconic(m_hWndPaint) ) 
					{
						if( m_pRoot->IsUpdateNeeded() ) 
						{
							RECT rcRoot = rcClient;
							if( m_bLayered ) 
							{
								rcRoot.left += m_rcLayeredInset.left;
								rcRoot.top += m_rcLayeredInset.top;
								rcRoot.right -= m_rcLayeredInset.right;
								rcRoot.bottom -= m_rcLayeredInset.bottom;
							}
							m_pRoot->SetPos(rcRoot, true);
							bNeedSizeMsg = true;
						}
						else 
						{
							//����NeedUpdateĳ���ؼ�ʱ
							CControlUI* pControl = NULL;
							m_aFoundControls.Empty();
							m_pRoot->FindControl(__FindControlsFromUpdate, NULL, UIFIND_VISIBLE | UIFIND_ME_FIRST | UIFIND_UPDATETEST);
							for( int it = 0; it < m_aFoundControls.GetSize(); it++ ) {
								pControl = static_cast<CControlUI*>(m_aFoundControls[it]);
								//float�ؼ�����Ҫ���¼���RelativePos����Ϊ��һ��SetPosʱ���Ѿ���ƫ�Ƽ�����ˣ����ұ��浽m_rcItem��
								//if( !pControl->IsFloat() ) pControl->SetPos(pControl->GetPos(), true);
								//else pControl->SetPos(pControl->GetRelativePos(), true);
								pControl->SetPos(pControl->GetPos(), true);
							}
							bNeedSizeMsg = true;
						}

						//��һ�λ��ƴ���֮ǰ������DUI_MSGTYPE_WINDOWINIT��Ϣ
						if( m_bFirstLayout ) 
						{
							m_bFirstLayout = false;
							SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWINIT,  0, 0, false);
							if( m_bLayered && m_bLayeredChanged ) 
							{
								Invalidate();
								SetPainting(false);
								return true;
							}
							// ������Ӱ������ʾ
							m_shadow.Update(m_hWndPaint);
						}
					}
				}
				else if( m_bLayered && m_bLayeredChanged ) {
					RECT rcRoot = rcClient;
					rcRoot.left += m_rcLayeredInset.left;
					rcRoot.top += m_rcLayeredInset.top;
					rcRoot.right -= m_rcLayeredInset.right;
					rcRoot.bottom -= m_rcLayeredInset.bottom;
					m_pRoot->SetPos(rcRoot, true);
				}

				if( m_bLayered ) 
				{
					DWORD dwExStyle = ::GetWindowLong(m_hWndPaint, GWL_EXSTYLE);
					DWORD dwNewExStyle = dwExStyle | WS_EX_LAYERED;
					if(dwExStyle != dwNewExStyle) ::SetWindowLong(m_hWndPaint, GWL_EXSTYLE, dwNewExStyle);

					UnionRect(&rcPaint, &rcPaint, &m_rcLayeredUpdate);
					if( rcPaint.right > rcClient.right ) rcPaint.right = rcClient.right;
					if( rcPaint.bottom > rcClient.bottom ) rcPaint.bottom = rcClient.bottom;
					::ZeroMemory(&m_rcLayeredUpdate, sizeof(m_rcLayeredUpdate));
				}

				PAINTSTRUCT ps = { 0 };
				::BeginPaint(m_hWndPaint, &ps);

				Render()->Resize(dwWidth, dwHeight);

				Render()->GetBitmap()->ClearAlpha(rcPaint);

				Render()->SaveDC();

				if( m_bLayered ) 
				{
					if(!m_diLayered.sDrawString.IsEmpty() && IsLayeredChanged()) 
					{
						DWORD dwWidth = rcClient.right - rcClient.left;
						DWORD dwHeight = rcClient.bottom - rcClient.top;
						RECT rcLayeredClient = rcClient;
						rcLayeredClient.left += m_rcLayeredInset.left;
						rcLayeredClient.top += m_rcLayeredInset.top;
						rcLayeredClient.right -= m_rcLayeredInset.right;
						rcLayeredClient.bottom -= m_rcLayeredInset.bottom;

						UIClip clip;
						clip.GenerateClip(Render(), rcLayeredClient);
						Render()->DrawImageInfo(rcLayeredClient, rcLayeredClient, &m_diLayered);
					}
				}

				GetRoot()->Paint(Render(), rcPaint, NULL);

				for( int i = 0; i < m_aPostPaintControls.GetSize(); i++ ) {
					CControlUI* pPostPaintControl = static_cast<CControlUI*>(m_aPostPaintControls[i]);
					pPostPaintControl->DoPostPaint(Render(), rcPaint);
				}

				//������Ҫˢ�µ����򣬿�������ĳЩ���Գ�����
				if( IsShowUpdateRect() && !IsLayered() ) 
				{
					Render()->DrawRect(rcPaint, 1, UIRGB(255,0,0));
				}

				//Render()->GetBitmap()->SaveFile(_T("c:\\uiframe.bmp"));
				Render()->RestoreDC();

				if( IsLayered() ) 
				{
					RECT rcWnd = { 0 };
					::GetWindowRect(m_hWndPaint, &rcWnd);
					BLENDFUNCTION bf = { AC_SRC_OVER, 0, GetOpacity(), AC_SRC_ALPHA };
					POINT ptPos   = { rcWnd.left, rcWnd.top };
					SIZE sizeWnd  = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
					POINT ptSrc   = { 0, 0 };
					::UpdateLayeredWindow(m_hWndPaint, NULL, &ptPos, &sizeWnd, Render()->GetDC(), &ptSrc, 0, &bf, ULW_ALPHA);
				}
				else 
				{
					::BitBlt(m_hDcPaint, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, Render()->GetDC(), rcPaint.left, rcPaint.top, SRCCOPY);
				}

				Render()->RestoreObject();

				::EndPaint(m_hWndPaint, &ps);

				// ���ƽ���
				SetPainting(false);
				m_bLayeredChanged = false;
				if( m_bUpdateNeeded ) Invalidate();

				// ���ʹ��ڴ�С�ı���Ϣ
				if(bNeedSizeMsg) {
					this->SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWSIZE, 0, 0, true);
				}
				return true;
			}
		case WM_PRINTCLIENT:
			{
// 				if( m_pRoot == NULL ) break;
// 				RECT rcClient;
// 				::GetClientRect(m_hWndPaint, &rcClient);
// 				HDC hDC = (HDC) wParam;
// 				int save = ::SaveDC(hDC);
// 				m_pRoot->Paint(hDC, rcClient, NULL);
// 				if( (lParam & PRF_CHILDREN) != 0 ) {
// 					HWND hWndChild = ::GetWindow(m_hWndPaint, GW_CHILD);
// 					while( hWndChild != NULL ) {
// 						RECT rcPos = { 0 };
// 						::GetWindowRect(hWndChild, &rcPos);
// 						::MapWindowPoints(HWND_DESKTOP, m_hWndPaint, reinterpret_cast<LPPOINT>(&rcPos), 2);
// 						::SetWindowOrgEx(hDC, -rcPos.left, -rcPos.top, NULL);
// 						::SendMessage(hWndChild, WM_PRINT, wParam, lParam | PRF_NONCLIENT);
// 						hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
// 					}
// 				}
// 				::RestoreDC(hDC, save);
			}
			break;
		case WM_GETMINMAXINFO:
			{
				MONITORINFO Monitor = {};
				Monitor.cbSize = sizeof(Monitor);
				::GetMonitorInfo(::MonitorFromWindow(m_hWndPaint, MONITOR_DEFAULTTOPRIMARY), &Monitor);
				RECT rcWork = Monitor.rcWork;
				if( Monitor.dwFlags != MONITORINFOF_PRIMARY ) {
					::OffsetRect(&rcWork, -rcWork.left, -rcWork.top);
				}

				LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
				if( m_szMinWindow.cx > 0 ) lpMMI->ptMinTrackSize.x = m_szMinWindow.cx;
				if( m_szMinWindow.cy > 0 ) lpMMI->ptMinTrackSize.y = m_szMinWindow.cy;
				if( m_szMaxWindow.cx > 0 ) lpMMI->ptMaxTrackSize.x = m_szMaxWindow.cx;
				if( m_szMaxWindow.cy > 0 ) lpMMI->ptMaxTrackSize.y = m_szMaxWindow.cy;
				if( m_szMaxWindow.cx > 0 ) lpMMI->ptMaxSize.x = m_szMaxWindow.cx;
				if( m_szMaxWindow.cy > 0 ) lpMMI->ptMaxSize.y = m_szMaxWindow.cy;
			}
			break;
		case WM_SIZE:
			{
				if( m_pFocus != NULL ) {
					TEventUI event = { 0 };
					event.Type = UIEVENT_WINDOWSIZE;
					event.pSender = m_pFocus;
					event.dwTimestamp = ::GetTickCount();
					m_pFocus->Event(event);
				}
				if( m_pRoot != NULL ) m_pRoot->NeedUpdate();
			}
			return true;
		case WM_TIMER:
			{
				for( int i = 0; i < m_aTimers.GetSize(); i++ ) {
					const TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
					if(pTimer->hWnd == m_hWndPaint && 
						pTimer->uWinTimer == LOWORD(wParam) && 
						pTimer->bKilled == false)
					{
						TEventUI event = { 0 };
						event.Type = UIEVENT_TIMER;
						event.pSender = pTimer->pSender;
						event.dwTimestamp = ::GetTickCount();
						event.ptMouse = m_ptLastMousePos;
						event.wKeyState = MapKeyState();
						event.wParam = pTimer->nLocalID;
						event.lParam = lParam;
						pTimer->pSender->Event(event);
						break;
					}
				}
			}
			break;
		case WM_MOUSEHOVER:
			{
				m_bMouseTracking = false;
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				CControlUI* pHover = FindControl(pt);
				if( pHover == NULL ) break;
				// Generate mouse hover event
				if( m_pEventHover != NULL ) {
					TEventUI event = { 0 };
					event.Type = UIEVENT_MOUSEHOVER;
					event.pSender = m_pEventHover;
					event.wParam = wParam;
					event.lParam = lParam;
					event.dwTimestamp = ::GetTickCount();
					event.ptMouse = pt;
					event.wKeyState = MapKeyState();
					m_pEventHover->Event(event);
				}
				// Create tooltip information
				CDuiString sToolTip = pHover->GetToolTip();
				if( sToolTip.IsEmpty() ) return true;
				::ZeroMemory(&m_ToolTip, sizeof(TOOLINFO));
				m_ToolTip.cbSize = sizeof(TOOLINFO);
				m_ToolTip.uFlags = TTF_IDISHWND;
				m_ToolTip.hwnd = m_hWndPaint;
				m_ToolTip.uId = (UINT_PTR) m_hWndPaint;
				m_ToolTip.hinst = m_hInstance;
				m_ToolTip.lpszText = const_cast<LPTSTR>( (LPCTSTR) sToolTip );
				m_ToolTip.rect = pHover->GetPos();
				if( m_hwndTooltip == NULL ) {
					m_hwndTooltip = ::CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hWndPaint, NULL, m_hInstance, NULL);
					::SendMessage(m_hwndTooltip, TTM_ADDTOOL, 0, (LPARAM) &m_ToolTip);
					::SendMessage(m_hwndTooltip,TTM_SETMAXTIPWIDTH,0, pHover->GetToolTipWidth());
				}
				if(!::IsWindowVisible(m_hwndTooltip))
				{
					::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolTip);
					::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ToolTip);
				}
			}
			return true;
		case WM_MOUSELEAVE:
			{
				if( m_hwndTooltip != NULL ) ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM) &m_ToolTip);
				if( m_bMouseTracking ) {
					POINT pt = { 0 };
					RECT rcWnd = { 0 };
					::GetCursorPos(&pt);
					::GetWindowRect(m_hWndPaint, &rcWnd);
					if( !::IsIconic(m_hWndPaint) && ::GetActiveWindow() == m_hWndPaint && ::PtInRect(&rcWnd, pt) ) {
						if( ::SendMessage(m_hWndPaint, WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y)) == HTCLIENT ) {
							::ScreenToClient(m_hWndPaint, &pt);
							::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
						}
						else 
							::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM)-1);
					}
					else 
						::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM)-1);
				}
				m_bMouseTracking = false;
			}
			break;
		case WM_MOUSEMOVE:
			{
				// Start tracking this entire window again...
				if( !m_bMouseTracking ) {
					TRACKMOUSEEVENT tme = { 0 };
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_HOVER | TME_LEAVE;
					tme.hwndTrack = m_hWndPaint;
					tme.dwHoverTime = m_hwndTooltip == NULL ? m_iHoverTime : (DWORD) ::SendMessage(m_hwndTooltip, TTM_GETDELAYTIME, TTDT_INITIAL, 0L);
					_TrackMouseEvent(&tme);
					m_bMouseTracking = true;
				}

				// Generate the appropriate mouse messages
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				// �Ƿ��ƶ�
				bool bNeedDrag = true;
				if(m_ptLastMousePos.x == pt.x && m_ptLastMousePos.y == pt.y) {
					bNeedDrag = false;
				}
				// ��¼���λ��
				m_ptLastMousePos = pt;
				CControlUI* pNewHover = FindControl(pt);
				if( pNewHover != NULL && pNewHover->GetManager() != this ) break;

				// ��ק�¼�
				if(bNeedDrag && m_bDragMode && wParam == MK_LBUTTON)
				{
					::ReleaseCapture();
					CIDropSource* pdsrc = new CIDropSource;
					if(pdsrc == NULL) return 0;
					pdsrc->AddRef();

					CIDataObject* pdobj = new CIDataObject(pdsrc);
					if(pdobj == NULL) return 0;
					pdobj->AddRef();

					FORMATETC fmtetc = {0};
					STGMEDIUM medium = {0};
					fmtetc.dwAspect = DVASPECT_CONTENT;
					fmtetc.lindex = -1;
					//////////////////////////////////////
					fmtetc.cfFormat = CF_BITMAP;
					fmtetc.tymed = TYMED_GDI;			
					medium.tymed = TYMED_GDI;
					//HBITMAP hBitmap = (HBITMAP)OleDuplicateData(m_hDragBitmap, fmtetc.cfFormat, NULL);
					HBITMAP hBitmap = (HBITMAP)OleDuplicateData(m_dragBitmap->GetBitmap(), fmtetc.cfFormat, NULL);
					medium.hBitmap = hBitmap;
					pdobj->SetData(&fmtetc,&medium,FALSE);
					//////////////////////////////////////
					BITMAP bmap;
					GetObject(hBitmap, sizeof(BITMAP), &bmap);
					RECT rc={0, 0, bmap.bmWidth, bmap.bmHeight};
					fmtetc.cfFormat = CF_ENHMETAFILE;
					fmtetc.tymed = TYMED_ENHMF;
					HDC hMetaDC = CreateEnhMetaFile(m_hDcPaint, NULL, NULL, NULL);
					HDC hdcMem = CreateCompatibleDC(m_hDcPaint);
					HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBitmap);
					::BitBlt(hMetaDC, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
					::SelectObject(hdcMem, hOldBmp);
					medium.hEnhMetaFile = CloseEnhMetaFile(hMetaDC);
					DeleteDC(hdcMem);
					medium.tymed = TYMED_ENHMF;
					pdobj->SetData(&fmtetc, &medium, TRUE);
					//////////////////////////////////////
					CDragSourceHelper dragSrcHelper;
					POINT ptDrag = {0};
					ptDrag.x = bmap.bmWidth / 2;
					ptDrag.y = bmap.bmHeight / 2;
					dragSrcHelper.InitializeFromBitmap(hBitmap, ptDrag, rc, pdobj); //will own the bmp
					DWORD dwEffect;
					HRESULT hr = ::DoDragDrop(pdobj, pdsrc, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);
					if(dwEffect )
						pdsrc->Release();
					delete pdsrc;
					pdobj->Release();
					m_bDragMode = false;
					break;
				}
				TEventUI event = { 0 };
				event.ptMouse = pt;
				event.wParam = wParam;
				event.lParam = lParam;
				event.dwTimestamp = ::GetTickCount();
				event.wKeyState = MapKeyState();
				if( !IsCaptured() ) {
					pNewHover = FindControl(pt);
					if( pNewHover != NULL && pNewHover->GetManager() != this ) break;
					if( pNewHover != m_pEventHover && m_pEventHover != NULL ) {
						event.Type = UIEVENT_MOUSELEAVE;
						event.pSender = m_pEventHover;

						CStdPtrArray aNeedMouseLeaveNeeded(m_aNeedMouseLeaveNeeded.GetSize());
						aNeedMouseLeaveNeeded.Resize(m_aNeedMouseLeaveNeeded.GetSize());
						::CopyMemory(aNeedMouseLeaveNeeded.GetData(), m_aNeedMouseLeaveNeeded.GetData(), m_aNeedMouseLeaveNeeded.GetSize() * sizeof(LPVOID));
						for( int i = 0; i < aNeedMouseLeaveNeeded.GetSize(); i++ ) {
							static_cast<CControlUI*>(aNeedMouseLeaveNeeded[i])->Event(event);
						}

						m_pEventHover->Event(event);
						m_pEventHover = NULL;
						if( m_hwndTooltip != NULL ) ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM) &m_ToolTip);
					}
					if( pNewHover != m_pEventHover && pNewHover != NULL ) {
						event.Type = UIEVENT_MOUSEENTER;
						event.pSender = pNewHover;
						pNewHover->Event(event);
						m_pEventHover = pNewHover;
					}
				}
				if( m_pEventClick != NULL ) {
					event.Type = UIEVENT_MOUSEMOVE;
					event.pSender = m_pEventClick;
					m_pEventClick->Event(event);
				}
				else if( pNewHover != NULL ) {
					event.Type = UIEVENT_MOUSEMOVE;
					event.pSender = pNewHover;
					pNewHover->Event(event);
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				// We alway set focus back to our app (this helps
				// when Win32 child windows are placed on the dialog
				// and we need to remove them on focus change).
				if (!m_bNoActivate) ::SetFocus(m_hWndPaint);
				if( m_pRoot == NULL ) break;
				// ���ҿؼ�
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				m_ptLastMousePos = pt;
				CControlUI* pControl = FindControl(pt);
				if( pControl == NULL ) break;
				if( pControl->GetManager() != this ) break;

				// ׼����ק
				if(pControl->IsDragEnabled()) {
					m_bDragMode = true;
					m_dragBitmap = MakeRefPtr<UIBitmap>(UIGlobal::CreateControlBitmap(pControl));
				}

				// ��������
				SetCapture();
				// �¼�����
				m_pEventClick = pControl;
				pControl->SetFocus();

				TEventUI event = { 0 };
				event.Type = UIEVENT_BUTTONDOWN;
				event.pSender = pControl;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = (WORD)wParam;
				event.dwTimestamp = ::GetTickCount();
				pControl->Event(event);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				if (!m_bNoActivate) ::SetFocus(m_hWndPaint);

				//����duilib�ѵ�����˫������һ��
				//��˫���¼�֮�󵯳����ڣ�����ڴ����иı��˸����ڿؼ������ܵ���m_pEvenClickʧЧ
				//˫��ʱ�¼��Ĵ���˳���� 1������� -- 2�������  -- 3˫�� 
				//�������ʱ��д��m_pEventClick = NULL; ˫��֮��û�������m_pEventClick = NULL��

				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				m_ptLastMousePos = pt;
				CControlUI* pControl = FindControl(pt);
				if( pControl == NULL ) break;
				if( pControl->GetManager() != this ) break;
				SetCapture();
				TEventUI event = { 0 };
				event.Type = UIEVENT_DBLCLICK;
				event.pSender = pControl;
				event.ptMouse = pt;
				event.wParam = wParam;
				event.lParam = lParam;
				event.wKeyState = (WORD)wParam;
				event.dwTimestamp = ::GetTickCount();
				pControl->Event(event);
				m_pEventClick = pControl;
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				m_ptLastMousePos = pt;
				if( m_pEventClick == NULL ) break;
				ReleaseCapture();
				TEventUI event = { 0 };
				event.Type = UIEVENT_BUTTONUP;
				event.pSender = m_pEventClick;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = (WORD)wParam;
				event.dwTimestamp = ::GetTickCount();

				CControlUI* pClick = m_pEventClick;
				m_pEventClick = NULL;
				pClick->Event(event);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				if (!m_bNoActivate) ::SetFocus(m_hWndPaint);
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				m_ptLastMousePos = pt;
				CControlUI* pControl = FindControl(pt);
				if( pControl == NULL ) break;
				if( pControl->GetManager() != this ) break;
				pControl->SetFocus();
				SetCapture();
				TEventUI event = { 0 };
				event.Type = UIEVENT_RBUTTONDOWN;
				event.pSender = pControl;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = (WORD)wParam;
				event.dwTimestamp = ::GetTickCount();
				pControl->Event(event);
				m_pEventClick = pControl;
			}
			break;
		case WM_RBUTTONUP:
			{
				if(m_bMouseCapture) ReleaseCapture();
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				m_ptLastMousePos = pt;
				m_pEventClick = FindControl(pt);
				if(m_pEventClick == NULL) break;

				TEventUI event = { 0 };
				event.Type = UIEVENT_RBUTTONUP;
				event.pSender = m_pEventClick;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = (WORD)wParam;
				event.dwTimestamp = ::GetTickCount();
				m_pEventClick->Event(event);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				if (!m_bNoActivate) ::SetFocus(m_hWndPaint);
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				m_ptLastMousePos = pt;
				CControlUI* pControl = FindControl(pt);
				if( pControl == NULL ) break;
				if( pControl->GetManager() != this ) break;
				pControl->SetFocus();
				SetCapture();
				TEventUI event = { 0 };
				event.Type = UIEVENT_MBUTTONDOWN;
				event.pSender = pControl;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = (WORD)wParam;
				event.dwTimestamp = ::GetTickCount();
				pControl->Event(event);
				m_pEventClick = pControl;
			}
			break;
		case WM_MBUTTONUP:
			{
				if(m_bMouseCapture) ReleaseCapture();
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				m_ptLastMousePos = pt;
				m_pEventClick = FindControl(pt);
				if(m_pEventClick == NULL) break;

				TEventUI event = { 0 };
				event.Type = UIEVENT_MBUTTONUP;
				event.pSender = m_pEventClick;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = (WORD)wParam;
				event.dwTimestamp = ::GetTickCount();
				m_pEventClick->Event(event);
			}
			break;
		case WM_CONTEXTMENU:
			{
				if( m_pRoot == NULL ) break;
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				::ScreenToClient(m_hWndPaint, &pt);
				m_ptLastMousePos = pt;
				if( m_pEventClick == NULL ) break;
				ReleaseCapture();
				TEventUI event = { 0 };
				event.Type = UIEVENT_CONTEXTMENU;
				event.pSender = m_pEventClick;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = (WORD)wParam;
				event.lParam = (LPARAM)m_pEventClick;
				event.dwTimestamp = ::GetTickCount();
				m_pEventClick->Event(event);
				m_pEventClick = NULL;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				if( m_pRoot == NULL ) break;
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				::ScreenToClient(m_hWndPaint, &pt);
				m_ptLastMousePos = pt;
				CControlUI* pControl = FindControl(pt);
				if( pControl == NULL ) break;
				if( pControl->GetManager() != this ) break;
				int zDelta = (int) (short) HIWORD(wParam);
				TEventUI event = { 0 };
				event.Type = UIEVENT_SCROLLWHEEL;
				event.pSender = pControl;
				event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, zDelta);
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = MapKeyState();
				event.dwTimestamp = ::GetTickCount();
				pControl->Event(event);

				// Let's make sure that the scroll item below the cursor is the same as before...
				::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM) MAKELPARAM(m_ptLastMousePos.x, m_ptLastMousePos.y));
			}
			break;
		case WM_CHAR:
			{
				if( m_pRoot == NULL ) break;
				if( m_pFocus == NULL ) break;
				TEventUI event = { 0 };
				event.Type = UIEVENT_CHAR;
				event.pSender = m_pFocus;
				event.wParam = wParam;
				event.lParam = lParam;
				event.chKey = (TCHAR)wParam;
				event.ptMouse = m_ptLastMousePos;
				event.wKeyState = MapKeyState();
				event.dwTimestamp = ::GetTickCount();
				m_pFocus->Event(event);
			}
			break;
		case WM_KEYDOWN:
			{
				if( m_pRoot == NULL ) break;
				if( m_pFocus == NULL ) break;
				TEventUI event = { 0 };
				event.Type = UIEVENT_KEYDOWN;
				event.pSender = m_pFocus;
				event.wParam = wParam;
				event.lParam = lParam;
				event.chKey = (TCHAR)wParam;
				event.ptMouse = m_ptLastMousePos;
				event.wKeyState = MapKeyState();
				event.dwTimestamp = ::GetTickCount();
				m_pFocus->Event(event);
				m_pEventKey = m_pFocus;
			}
			break;
		case WM_KEYUP:
			{
				if( m_pRoot == NULL ) break;
				if( m_pEventKey == NULL ) break;
				TEventUI event = { 0 };
				event.Type = UIEVENT_KEYUP;
				event.pSender = m_pEventKey;
				event.wParam = wParam;
				event.lParam = lParam;
				event.chKey = (TCHAR)wParam;
				event.ptMouse = m_ptLastMousePos;
				event.wKeyState = MapKeyState();
				event.dwTimestamp = ::GetTickCount();
				m_pEventKey->Event(event);
				m_pEventKey = NULL;
			}
			break;
		case WM_SETCURSOR:
			{
				if( m_pRoot == NULL ) break;
				if( LOWORD(lParam) != HTCLIENT ) break;
				if( m_bMouseCapture ) return true;

				POINT pt = { 0 };
				::GetCursorPos(&pt);
				::ScreenToClient(m_hWndPaint, &pt);
				CControlUI* pControl = FindControl(pt);
				if( pControl == NULL ) break;
				if( (pControl->GetControlFlags() & UIFLAG_SETCURSOR) == 0 ) break;
				TEventUI event = { 0 };
				event.Type = UIEVENT_SETCURSOR;
				event.pSender = pControl;
				event.wParam = wParam;
				event.lParam = lParam;
				event.ptMouse = pt;
				event.wKeyState = MapKeyState();
				event.dwTimestamp = ::GetTickCount();
				pControl->Event(event);
			}
			return true;
		case WM_SETFOCUS:
			{
				if( m_pFocus != NULL ) {
					TEventUI event = { 0 };
					event.Type = UIEVENT_SETFOCUS;
					event.wParam = wParam;
					event.lParam = lParam;
					event.pSender = m_pFocus;
					event.dwTimestamp = ::GetTickCount();
					m_pFocus->Event(event);
				}
				break;
			}
		case WM_KILLFOCUS:
			{
				if(IsCaptured()) ReleaseCapture();
				break;
			}
		case WM_NOTIFY:
			{
				if( lParam == 0 ) break;
				LPNMHDR lpNMHDR = (LPNMHDR) lParam;
				if( lpNMHDR != NULL ) lRes = ::SendMessage(lpNMHDR->hwndFrom, OCM__BASE + uMsg, wParam, lParam);
				return true;
			}
			break;
		case WM_COMMAND:
			{
				if( lParam == 0 ) break;
				HWND hWndChild = (HWND) lParam;
				lRes = ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
				if(lRes != 0) return true;
			}
			break;
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSTATIC:
			{
				// Refer To: http://msdn.microsoft.com/en-us/library/bb761691(v=vs.85).aspx
				// Read-only or disabled edit controls do not send the WM_CTLCOLOREDIT message; instead, they send the WM_CTLCOLORSTATIC message.
				if( lParam == 0 ) break;
				HWND hWndChild = (HWND) lParam;
				lRes = ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
				if(lRes != 0) return true;
			}
			break;
		default:
			break;
		}
		return false;
	}

	bool CPaintManagerUI::IsUpdateNeeded() const
	{
		return m_bUpdateNeeded;
	}

	void CPaintManagerUI::NeedUpdate()
	{
		m_bUpdateNeeded = true;
	}

	void CPaintManagerUI::LockUpdate(bool bLock)
	{
		m_bLockUpdate = bLock;
	}

	bool CPaintManagerUI::IsLockUpdate()
	{
		return m_bLockUpdate;
	}
	void CPaintManagerUI::Invalidate()
	{
		RECT rcClient = { 0 };
		::GetClientRect(m_hWndPaint, &rcClient);
		::UnionRect(&m_rcLayeredUpdate, &m_rcLayeredUpdate, &rcClient);
		::InvalidateRect(m_hWndPaint, NULL, FALSE);
	}

	void CPaintManagerUI::Invalidate(RECT& rcItem)
	{
		if( rcItem.left < 0 ) rcItem.left = 0;
		if( rcItem .top < 0 ) rcItem.top = 0;
		if( rcItem.right < rcItem.left ) rcItem.right = rcItem.left;
		if( rcItem.bottom < rcItem.top ) rcItem.bottom = rcItem.top;
		::UnionRect(&m_rcLayeredUpdate, &m_rcLayeredUpdate, &rcItem);
		::InvalidateRect(m_hWndPaint, &rcItem, FALSE);
	}

	bool CPaintManagerUI::AttachDialog(CControlUI* pControl)
	{
		ASSERT(::IsWindow(m_hWndPaint));
		// ������Ӱ����
		m_shadow.Create(this);

		// Reset any previous attachment
		SetFocus(NULL);
		m_pEventKey = NULL;
		m_pEventHover = NULL;
		m_pEventClick = NULL;
		// Remove the existing control-tree. We might have gotten inside this function as
		// a result of an event fired or similar, so we cannot just delete the objects and
		// pull the internal memory of the calling code. We'll delay the cleanup.
		if( m_pRoot != NULL ) {
			m_aPostPaintControls.Empty();
			AddDelayedCleanup(m_pRoot);
		}
		// Set the dialog root element
		m_pRoot = pControl;
		// Go ahead...
		m_bUpdateNeeded = true;
		m_bFirstLayout = true;
		m_bFocusNeeded = true;
		// Initiate all control
		return InitControls(pControl);
	}

	bool CPaintManagerUI::InitControls(CControlUI* pControl, CControlUI* pParent /*= NULL*/)
	{
		ASSERT(pControl);
		if( pControl == NULL ) return false;
		pControl->SetManager(this, pParent != NULL ? pParent : pControl->GetParent(), true);
		pControl->FindControl(__FindControlFromNameHash, this, UIFIND_ALL);
		return true;
	}

	void CPaintManagerUI::ReapObjects(CControlUI* pControl)
	{
		if( pControl == m_pEventKey ) m_pEventKey = NULL;
		if( pControl == m_pEventHover ) m_pEventHover = NULL;
		if( pControl == m_pEventClick ) m_pEventClick = NULL;
		if( pControl == m_pFocus ) m_pFocus = NULL;
		KillTimer(pControl);
		const CDuiString& sName = pControl->GetName();
		if( !sName.IsEmpty() ) {
			if( pControl == FindControl(sName) ) m_mNameHash.Remove(sName);
		}
		for( int i = 0; i < m_aAsyncNotify.GetSize(); i++ ) {
			TNotifyUI* pMsg = static_cast<TNotifyUI*>(m_aAsyncNotify[i]);
			if( pMsg->pSender == pControl ) pMsg->pSender = NULL;
		}    
	}

	bool CPaintManagerUI::AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl)
	{
		LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
		if( lp ) {
			CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
			for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
				if( static_cast<CControlUI*>(aOptionGroup->GetAt(i)) == pControl ) {
					return false;
				}
			}
			aOptionGroup->Add(pControl);
		}
		else {
			CStdPtrArray* aOptionGroup = new CStdPtrArray(6);
			aOptionGroup->Add(pControl);
			m_mOptionGroup.Insert(pStrGroupName, aOptionGroup);
		}
		return true;
	}

	CStdPtrArray* CPaintManagerUI::GetOptionGroup(LPCTSTR pStrGroupName)
	{
		LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
		if( lp ) return static_cast<CStdPtrArray*>(lp);
		return NULL;
	}

	void CPaintManagerUI::RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl)
	{
		LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
		if( lp ) {
			CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
			if( aOptionGroup == NULL ) return;
			for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
				if( static_cast<CControlUI*>(aOptionGroup->GetAt(i)) == pControl ) {
					aOptionGroup->Remove(i);
					break;
				}
			}
			if( aOptionGroup->IsEmpty() ) {
				delete aOptionGroup;
				m_mOptionGroup.Remove(pStrGroupName);
			}
		}
	}

	void CPaintManagerUI::RemoveAllOptionGroups()
	{
		CStdPtrArray* aOptionGroup;
		for( int i = 0; i< m_mOptionGroup.GetSize(); i++ ) {
			if(LPCTSTR key = m_mOptionGroup.GetAt(i)) {
				aOptionGroup = static_cast<CStdPtrArray*>(m_mOptionGroup.Find(key));
				delete aOptionGroup;
			}
		}
		m_mOptionGroup.RemoveAll();
	}

	void CPaintManagerUI::MessageLoop()
	{
		MSG msg = { 0 };
		while( ::GetMessage(&msg, NULL, 0, 0) ) {
			if( !CPaintManagerUI::TranslateMessage(&msg) ) {
				::TranslateMessage(&msg);
				try{
					::DispatchMessage(&msg);
				} catch(...) {
					DUITRACE(_T("EXCEPTION: %s(%d)\n"), __FILET__, __LINE__);
#ifdef _DEBUG
					throw "CPaintManagerUI::MessageLoop";
#endif
				}
			}
		}
	}

	void CPaintManagerUI::Term()
	{
		//ж�ؽű�����
		if(m_funDeleteScriptEngine){
			(*m_funDeleteScriptEngine)(m_pSharedScriptEngine);
		}

		// ������Դ������
		CResourceManager::GetInstance()->Release();
		CControlFactory::GetInstance()->Release();
		CMenuWnd::DestroyMenu();
		CLangManagerUI::ReleaseStringTable();

		// ��������Դ
		// ͼƬ
		UIImage* data;
		for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i)) {
				data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(key, false));
				if (data) {
					data->Release();
				}
			}
		}
		m_SharedResInfo.m_ImageHash.RemoveAll();
		// ����
		UIFont* pFontInfo;
		for( int i = 0; i< m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) {
				pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key, false));
				if (pFontInfo) {
					pFontInfo->Release();
				}
			}
		}
		m_SharedResInfo.m_CustomFonts.RemoveAll();
		// Ĭ������
 		if(m_SharedResInfo.m_DefaultFontInfo) {
 			m_SharedResInfo.m_DefaultFontInfo = NULL;
 		}
		// ��ʽ
		CDuiString* pStyle;
		for( int i = 0; i< m_SharedResInfo.m_StyleHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_StyleHash.GetAt(i)) {
				pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(key, false));
				if (pStyle) {
					delete pStyle;
					pStyle = NULL;
				}
			}
		}
		m_SharedResInfo.m_StyleHash.RemoveAll();

		// ��ʽ
		CDuiString* pAttr;
		for( int i = 0; i< m_SharedResInfo.m_AttrHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_AttrHash.GetAt(i)) {
				pAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(key, false));
				if (pAttr) {
					delete pAttr;
					pAttr = NULL;
				}
			}
		}
		m_SharedResInfo.m_AttrHash.RemoveAll();

		// �ر�ZIP
		if( m_bCachedResourceZip && m_hResourceZip != NULL ) {
			CloseZip((HZIP)m_hResourceZip);
			m_hResourceZip = NULL;
		}

		//ж��GDIPlus
		if(m_pGdiplusStartupInput != NULL)
		{
			Gdiplus::GdiplusShutdown(m_gdiplusToken);
			delete m_pGdiplusStartupInput;
		}
	}

	CDPI * DuiLib::CPaintManagerUI::GetDPIObj()
	{
		if (m_pDPI == NULL) {
			m_pDPI = new CDPI;
		}
		return m_pDPI;
	}

	void DuiLib::CPaintManagerUI::SetDPI(int iDPI)
	{
		int scale1 = GetDPIObj()->GetScale();
		GetDPIObj()->SetScale(iDPI);
		int scale2 = GetDPIObj()->GetScale();
		ResetDPIAssets();
		RECT rcWnd = {0};
		::GetWindowRect(GetPaintWindow(), &rcWnd);
		RECT*  prcNewWindow = &rcWnd;
		if (!::IsZoomed(GetPaintWindow())) {
			RECT rc = rcWnd;
			rc.right = rcWnd.left + (rcWnd.right - rcWnd.left) * scale2 / scale1;
			rc.bottom = rcWnd.top + (rcWnd.bottom - rcWnd.top) * scale2 / scale1;
			prcNewWindow = &rc;
		}
		SetWindowPos(GetPaintWindow(), NULL, prcNewWindow->left, prcNewWindow->top, prcNewWindow->right - prcNewWindow->left, prcNewWindow->bottom - prcNewWindow->top, SWP_NOZORDER | SWP_NOACTIVATE);
		if (GetRoot() != NULL) GetRoot()->NeedUpdate();
		::PostMessage(GetPaintWindow(), UIMSG_SET_DPI, 0, 0);
	}

	void DuiLib::CPaintManagerUI::SetAllDPI(int iDPI)
	{
		for (int i = 0; i < m_aPreMessages.GetSize(); i++) {
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			pManager->SetDPI(iDPI);
		}
	}

	void DuiLib::CPaintManagerUI::ResetDPIAssets()
	{
		RemoveAllDrawInfos();
		RemoveAllImages();;

		for (int it = 0; it < m_ResInfo.m_CustomFonts.GetSize(); it++) {
			UIFont* pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(m_ResInfo.m_CustomFonts[it]));
			pFontInfo->RebuildFont(this);
		}
		m_ResInfo.m_DefaultFontInfo->RebuildFont(this);

		for (int it = 0; it < m_SharedResInfo.m_CustomFonts.GetSize(); it++) {
			UIFont* pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(m_SharedResInfo.m_CustomFonts[it]));
			pFontInfo->RebuildFont(this);
		}
		m_SharedResInfo.m_DefaultFontInfo->RebuildFont(this);

		CStdPtrArray *richEditList = FindSubControlsByClass(GetRoot(), _T("RichEditUI"));
		for (int i = 0; i < richEditList->GetSize(); i++)
		{
			CRichEditUI* pT = static_cast<CRichEditUI*>((*richEditList)[i]);
			pT->SetFont(pT->GetFont());

		}
	}

	CControlUI* CPaintManagerUI::GetLastFocus() const //add by liqs99
	{
		if(m_pLastFocus == NULL)	return NULL;
		if(!m_pLastFocus->IsEnabled() || !m_pLastFocus->IsVisible())	return NULL;
		return m_pLastFocus;
	}

	CControlUI* CPaintManagerUI::GetFocus() const
	{
		return m_pFocus;
	}

	void CPaintManagerUI::SetFocus(CControlUI* pControl)
	{
		//�����ģʽ�����ÿؼ��Ľ��㣬��Ҫ���ô��ڽ���
		if(!UIDESIGNMODE)
		{
			// Paint manager window has focus?
			HWND hFocusWnd = ::GetFocus();
			if( hFocusWnd != m_hWndPaint && pControl != m_pFocus ) 
				::SetFocus(m_hWndPaint);
		}

		// Already has focus?
		if( pControl == m_pFocus ) return;
		// Remove focus from old control
		if( m_pFocus != NULL ) 
		{
			TEventUI event = { 0 };
			event.Type = UIEVENT_KILLFOCUS;
			event.pSender = pControl;
			event.dwTimestamp = ::GetTickCount();
			m_pFocus->Event(event);
			SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
			m_pLastFocus = m_pFocus;
			m_pFocus = NULL;
		}
		if( pControl == NULL ) return;
		// Set focus to new control
		if( pControl != NULL 
			&& pControl->GetManager() == this 
			&& pControl->IsVisible() 
			&& pControl->IsEnabled() ) 
		{
			m_pFocus = pControl;
			TEventUI event = { 0 };
			event.Type = UIEVENT_SETFOCUS;
			event.pSender = pControl;
			event.dwTimestamp = ::GetTickCount();
			m_pFocus->Event(event);
			SendNotify(m_pFocus, DUI_MSGTYPE_SETFOCUS);
		}
	}

	void CPaintManagerUI::SetFocusNeeded(CControlUI* pControl)
	{
		::SetFocus(m_hWndPaint);
		if( pControl == NULL ) return;
		if( m_pFocus != NULL ) {
			TEventUI event = { 0 };
			event.Type = UIEVENT_KILLFOCUS;
			event.pSender = pControl;
			event.dwTimestamp = ::GetTickCount();
			m_pFocus->Event(event);
			SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
			m_pFocus = NULL;
		}
		FINDTABINFO info = { 0 };
		info.pFocus = pControl;
		info.bForward = false;
		m_pFocus = m_pRoot->FindControl(__FindControlFromTab, &info, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
		m_bFocusNeeded = true;
		if( m_pRoot != NULL ) m_pRoot->NeedUpdate();
	}

	bool CPaintManagerUI::SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse)
	{
		ASSERT(pControl!=NULL);
		ASSERT(uElapse>0);
		for( int i = 0; i< m_aTimers.GetSize(); i++ ) {
			TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
			if( pTimer->pSender == pControl
				&& pTimer->hWnd == m_hWndPaint
				&& pTimer->nLocalID == nTimerID ) {
					if( pTimer->bKilled == true ) {
						if( ::SetTimer(m_hWndPaint, pTimer->uWinTimer, uElapse, NULL) ) {
							pTimer->bKilled = false;
							return true;
						}
						return false;
					}
					return false;
			}
		}

		m_uTimerID = (++m_uTimerID) % 0xF0; //0xf1-0xfe������;
		if( !::SetTimer(m_hWndPaint, m_uTimerID, uElapse, NULL) ) return FALSE;
		TIMERINFO* pTimer = new TIMERINFO;
		if( pTimer == NULL ) return FALSE;
		pTimer->hWnd = m_hWndPaint;
		pTimer->pSender = pControl;
		pTimer->nLocalID = nTimerID;
		pTimer->uWinTimer = m_uTimerID;
		pTimer->bKilled = false;
		return m_aTimers.Add(pTimer);
	}

	bool CPaintManagerUI::KillTimer(CControlUI* pControl, UINT nTimerID)
	{
		ASSERT(pControl!=NULL);
		for( int i = 0; i< m_aTimers.GetSize(); i++ ) {
			TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
			if( pTimer->pSender == pControl
				&& pTimer->hWnd == m_hWndPaint
				&& pTimer->nLocalID == nTimerID )
			{
				if( pTimer->bKilled == false ) {
					if( ::IsWindow(m_hWndPaint) ) ::KillTimer(pTimer->hWnd, pTimer->uWinTimer);
					pTimer->bKilled = true;
					return true;
				}
			}
		}
		return false;
	}

	void CPaintManagerUI::KillTimer(CControlUI* pControl)
	{
		ASSERT(pControl!=NULL);
		int count = m_aTimers.GetSize();
		for( int i = 0, j = 0; i < count; i++ ) {
			TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i - j]);
			if( pTimer->pSender == pControl && pTimer->hWnd == m_hWndPaint ) {
				if( pTimer->bKilled == false ) ::KillTimer(pTimer->hWnd, pTimer->uWinTimer);
				delete pTimer;
				m_aTimers.Remove(i - j);
				j++;
			}
		}
	}

	void CPaintManagerUI::RemoveAllTimers()
	{
		for( int i = 0; i < m_aTimers.GetSize(); i++ ) {
			TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
			if( pTimer->hWnd == m_hWndPaint ) {
				if( pTimer->bKilled == false ) {
					if( ::IsWindow(m_hWndPaint) ) ::KillTimer(m_hWndPaint, pTimer->uWinTimer);
				}
				delete pTimer;
			}
		}

		m_aTimers.Empty();
	}

	void CPaintManagerUI::SetCapture()
	{
		::SetCapture(m_hWndPaint);
		m_bMouseCapture = true;
	}

	void CPaintManagerUI::ReleaseCapture()
	{
		::ReleaseCapture();
		m_bMouseCapture = false;
		m_bDragMode = false;
	}

	bool CPaintManagerUI::IsCaptured()
	{
		return m_bMouseCapture;
	}

	bool CPaintManagerUI::IsPainting()
	{
		return m_bIsPainting;
	}

	void CPaintManagerUI::SetPainting(bool bIsPainting)
	{
		m_bIsPainting = bIsPainting;
	}

	bool CPaintManagerUI::SetNextTabControl(bool bForward)
	{
		// If we're in the process of restructuring the layout we can delay the
		// focus calulation until the next repaint.
		if( m_bUpdateNeeded && bForward ) {
			m_bFocusNeeded = true;
			::InvalidateRect(m_hWndPaint, NULL, FALSE);
			return true;
		}
		// Find next/previous tabbable control
		FINDTABINFO info1 = { 0 };
		info1.pFocus = m_pFocus;
		info1.bForward = bForward;
		CControlUI* pControl = m_pRoot->FindControl(__FindControlFromTab, &info1, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
		if( pControl == NULL ) {  
			if( bForward ) {
				// Wrap around
				FINDTABINFO info2 = { 0 };
				info2.pFocus = bForward ? NULL : info1.pLast;
				info2.bForward = bForward;
				pControl = m_pRoot->FindControl(__FindControlFromTab, &info2, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
			}
			else {
				pControl = info1.pLast;
			}
		}
		if( pControl != NULL ) SetFocus(pControl);
		m_bFocusNeeded = false;
		return true;
	}

	bool CPaintManagerUI::AddNotifier(INotifyUI* pNotifier)
	{
		ASSERT(m_aNotifiers.Find(pNotifier)<0);
		return m_aNotifiers.Add(pNotifier);
	}

	bool CPaintManagerUI::RemoveNotifier(INotifyUI* pNotifier)
	{
		for( int i = 0; i < m_aNotifiers.GetSize(); i++ ) {
			if( static_cast<INotifyUI*>(m_aNotifiers[i]) == pNotifier ) {
				return m_aNotifiers.Remove(i);
			}
		}
		return false;
	}

	bool CPaintManagerUI::AddPreMessageFilter(IMessageFilterUI* pFilter)
	{
		ASSERT(m_aPreMessageFilters.Find(pFilter)<0);
		return m_aPreMessageFilters.Add(pFilter);
	}

	bool CPaintManagerUI::RemovePreMessageFilter(IMessageFilterUI* pFilter)
	{
		for( int i = 0; i < m_aPreMessageFilters.GetSize(); i++ ) {
			if( static_cast<IMessageFilterUI*>(m_aPreMessageFilters[i]) == pFilter ) {
				return m_aPreMessageFilters.Remove(i);
			}
		}
		return false;
	}

	bool CPaintManagerUI::AddMessageFilter(IMessageFilterUI* pFilter)
	{
		ASSERT(m_aMessageFilters.Find(pFilter)<0);
		return m_aMessageFilters.Add(pFilter);
	}

	bool CPaintManagerUI::RemoveMessageFilter(IMessageFilterUI* pFilter)
	{
		for( int i = 0; i < m_aMessageFilters.GetSize(); i++ ) {
			if( static_cast<IMessageFilterUI*>(m_aMessageFilters[i]) == pFilter ) {
				return m_aMessageFilters.Remove(i);
			}
		}
		return false;
	}

	int CPaintManagerUI::GetPostPaintCount() const
	{
		return m_aPostPaintControls.GetSize();
	}

	bool CPaintManagerUI::IsPostPaint(CControlUI* pControl)
	{
		return m_aPostPaintControls.Find(pControl) >= 0;
	}

	bool CPaintManagerUI::AddPostPaint(CControlUI* pControl)
	{
		ASSERT(m_aPostPaintControls.Find(pControl) < 0);
		return m_aPostPaintControls.Add(pControl);
	}

	bool CPaintManagerUI::RemovePostPaint(CControlUI* pControl)
	{
		for( int i = 0; i < m_aPostPaintControls.GetSize(); i++ ) {
			if( static_cast<CControlUI*>(m_aPostPaintControls[i]) == pControl ) {
				return m_aPostPaintControls.Remove(i);
			}
		}
		return false;
	}

	bool CPaintManagerUI::SetPostPaintIndex(CControlUI* pControl, int iIndex)
	{
		RemovePostPaint(pControl);
		return m_aPostPaintControls.InsertAt(iIndex, pControl);
	}

	int CPaintManagerUI::GetNativeWindowCount() const
	{
		return m_aNativeWindow.GetSize();
	}

	bool CPaintManagerUI::AddNativeWindow(CControlUI* pControl, HWND hChildWnd)
	{
		if (pControl == NULL || hChildWnd == NULL) return false;

		RECT rcChildWnd = GetNativeWindowRect(hChildWnd);
		Invalidate(rcChildWnd);

		if (m_aNativeWindow.Find(hChildWnd) >= 0) return false;
		if (m_aNativeWindow.Add(hChildWnd)) {
			m_aNativeWindowControl.Add(pControl);
			return true;
		}
		return false;
	}

	bool CPaintManagerUI::RemoveNativeWindow(HWND hChildWnd)
	{
		for( int i = 0; i < m_aNativeWindow.GetSize(); i++ ) {
			if( static_cast<HWND>(m_aNativeWindow[i]) == hChildWnd ) {
				if( m_aNativeWindow.Remove(i) ) {
					m_aNativeWindowControl.Remove(i);
					return true;
				}
				return false;
			}
		}
		return false;
	}

	RECT CPaintManagerUI::GetNativeWindowRect(HWND hChildWnd)
	{
		RECT rcChildWnd;
		::GetWindowRect(hChildWnd, &rcChildWnd);
		::ScreenToClient(m_hWndPaint, (LPPOINT)(&rcChildWnd));
		::ScreenToClient(m_hWndPaint, (LPPOINT)(&rcChildWnd)+1);
		return rcChildWnd;
	}

	void CPaintManagerUI::AddDelayedCleanup(CControlUI* pControl)
	{
		if (pControl == NULL) return;
		pControl->SetManager(this, NULL, false);
		m_aDelayedCleanup.Add(pControl);
		PostAsyncNotify();
	}

	void CPaintManagerUI::AddMouseLeaveNeeded(CControlUI* pControl)
	{
		if (pControl == NULL) return;
		for( int i = 0; i < m_aNeedMouseLeaveNeeded.GetSize(); i++ ) {
			if( static_cast<CControlUI*>(m_aNeedMouseLeaveNeeded[i]) == pControl ) {
				return;
			}
		}
		m_aNeedMouseLeaveNeeded.Add(pControl);
	}

	bool CPaintManagerUI::RemoveMouseLeaveNeeded(CControlUI* pControl)
	{
		if (pControl == NULL) return false;
		for( int i = 0; i < m_aNeedMouseLeaveNeeded.GetSize(); i++ ) {
			if( static_cast<CControlUI*>(m_aNeedMouseLeaveNeeded[i]) == pControl ) {
				return m_aNeedMouseLeaveNeeded.Remove(i);
			}
		}
		return false;
	}

	void CPaintManagerUI::SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/, bool bAsync /*= false*/)
	{
		TNotifyUI Msg;
		Msg.pSender = pControl;
		Msg.sType = pstrMessage;
		Msg.wParam = wParam;
		Msg.lParam = lParam;
		SendNotify(Msg, bAsync);
	}

	void CPaintManagerUI::SendNotify(TNotifyUI& Msg, bool bAsync /*= false*/)
	{
		Msg.ptMouse = m_ptLastMousePos;
		Msg.dwTimestamp = ::GetTickCount();
		if( m_bUsedVirtualWnd )
		{
			Msg.sVirtualWnd = Msg.pSender->GetVirtualWnd();
		}

		if( !bAsync ) {
			// Send to all listeners
			if( Msg.pSender != NULL ) {
				if( Msg.pSender->OnNotify ) Msg.pSender->OnNotify(&Msg);
				if(Msg.pSender->m_asOnNotify) 
					ExecuteScript(Msg.pSender->m_asOnNotify, Msg.pSender, &Msg);
			}
			for( int i = 0; i < m_aNotifiers.GetSize(); i++ ) {
				static_cast<INotifyUI*>(m_aNotifiers[i])->Notify(Msg);
			}
		}
		else 
		{
			//CDuiInnerLock lock(&m_lockAsyncNotify);//add by liq99

			TNotifyUI *pMsg = new TNotifyUI;
			pMsg->pSender = Msg.pSender;
			pMsg->sType = Msg.sType;
			pMsg->wParam = Msg.wParam;
			pMsg->lParam = Msg.lParam;
			pMsg->ptMouse = Msg.ptMouse;
			pMsg->dwTimestamp = Msg.dwTimestamp;
			m_aAsyncNotify.Add(pMsg);

			PostAsyncNotify();
		}
	}

	bool CPaintManagerUI::IsForceUseSharedRes() const
	{
		return m_bForceUseSharedRes;
	}

	void CPaintManagerUI::SetForceUseSharedRes(bool bForce)
	{
		m_bForceUseSharedRes = bForce;
	}

	DWORD CPaintManagerUI::GetDefaultDisabledColor() const
	{
		return m_ResInfo.m_dwDefaultDisabledColor;
	}

	void CPaintManagerUI::SetDefaultDisabledColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultDisabledColor == m_SharedResInfo.m_dwDefaultDisabledColor)
				m_ResInfo.m_dwDefaultDisabledColor = dwColor;
			m_SharedResInfo.m_dwDefaultDisabledColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultDisabledColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultFontColor() const
	{
		return m_ResInfo.m_dwDefaultFontColor;
	}

	void CPaintManagerUI::SetDefaultFontColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultFontColor == m_SharedResInfo.m_dwDefaultFontColor)
				m_ResInfo.m_dwDefaultFontColor = dwColor;
			m_SharedResInfo.m_dwDefaultFontColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultFontColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultLinkFontColor() const
	{
		return m_ResInfo.m_dwDefaultLinkFontColor;
	}

	void CPaintManagerUI::SetDefaultLinkFontColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultLinkFontColor == m_SharedResInfo.m_dwDefaultLinkFontColor)
				m_ResInfo.m_dwDefaultLinkFontColor = dwColor;
			m_SharedResInfo.m_dwDefaultLinkFontColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultLinkFontColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultLinkHoverFontColor() const
	{
		return m_ResInfo.m_dwDefaultLinkHoverFontColor;
	}

	void CPaintManagerUI::SetDefaultLinkHoverFontColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultLinkHoverFontColor == m_SharedResInfo.m_dwDefaultLinkHoverFontColor)
				m_ResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
			m_SharedResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultSelectedBkColor() const
	{
		return m_ResInfo.m_dwDefaultSelectedBkColor;
	}

	void CPaintManagerUI::SetDefaultSelectedBkColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultSelectedBkColor == m_SharedResInfo.m_dwDefaultSelectedBkColor)
				m_ResInfo.m_dwDefaultSelectedBkColor = dwColor;
			m_SharedResInfo.m_dwDefaultSelectedBkColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultSelectedBkColor = dwColor;
		}
	}

	UIFont* CPaintManagerUI::GetDefaultFontInfo()
	{
		if (m_ResInfo.m_DefaultFontInfo->GetFontName().IsEmpty())
		{
			return m_SharedResInfo.m_DefaultFontInfo;
		}
		else
		{
			return m_ResInfo.m_DefaultFontInfo;
		}
	}

	void CPaintManagerUI::SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared)
	{
		if (bShared)
		{
			m_SharedResInfo.m_DefaultFontInfo->CreateFont(this, -1, pStrFontName, nSize, bBold, bUnderline, bItalic, true, bShared);
		}
		else
		{
			m_ResInfo.m_DefaultFontInfo->CreateFont(this, -1, pStrFontName, nSize, bBold, bUnderline, bItalic, true, bShared);
		}
	}

	DWORD CPaintManagerUI::GetCustomFontCount(bool bShared) const
	{
		if (bShared)
			return m_SharedResInfo.m_CustomFonts.GetSize();
		else
			return m_ResInfo.m_CustomFonts.GetSize();
	}

	UIFont* CPaintManagerUI::AddFont(int id, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared)
	{
		UIFont *pFontInfo = UIGlobal::CreateFont();
		pFontInfo->CreateFont(this, id, pStrFontName, nSize, bBold, bUnderline, bItalic, true, bShared);

		TCHAR idBuffer[16];
		::ZeroMemory(idBuffer, sizeof(idBuffer));
		_itot(id, idBuffer, 10);
		if (bShared || m_bForceUseSharedRes)
		{
			UIFont* pOldFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
			if (pOldFontInfo)
			{
				pOldFontInfo->Release();
				m_SharedResInfo.m_CustomFonts.Remove(idBuffer);
			}

			if( !m_SharedResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo) ) 
			{
				pFontInfo->Release();
				return NULL;
			}
		}
		else
		{
			UIFont* pOldFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
			if (pOldFontInfo)
			{
				pOldFontInfo->Release();
				m_ResInfo.m_CustomFonts.Remove(idBuffer);
			}

			if( !m_ResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo) ) 
			{
				pFontInfo->Release();
				return NULL;
			}
		}

		return pFontInfo;
	}

	void CPaintManagerUI::AddFontArray(LPCTSTR pstrPath) 
	{
		CUIFile f;
		if(!f.LoadFile(pstrPath)) return;

		DWORD nFonts;
		HANDLE hFont = ::AddFontMemResourceEx((LPVOID)f.GetData(), f.GetSize(), NULL, &nFonts);
		m_aFonts.Add(hFont);
	}

	UIFont* CPaintManagerUI::GetFont(int id)
	{
		if (id < 0) return GetDefaultFontInfo();

		TCHAR idBuffer[16];
		::ZeroMemory(idBuffer, sizeof(idBuffer));
		_itot(id, idBuffer, 10);
		UIFont* pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
		if( !pFontInfo ) pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
		if (!pFontInfo) return GetDefaultFontInfo();
		return pFontInfo;
	}

	UIFont* CPaintManagerUI::GetFont(int nIndex, bool bShared)
	{
		UIFont *pFontInfo = NULL;
		if (bShared)
		{
			LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(nIndex);
			pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key));
		}
		else
		{
			LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(nIndex);
			pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key));
		}
		return pFontInfo;
	}

	UIFont* CPaintManagerUI::GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
	{
		UIFont* pFontInfo = NULL;
		for( int i = 0; i< m_ResInfo.m_CustomFonts.GetSize(); i++ ) {
			if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i)) {
				pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key));
				if (pFontInfo && pFontInfo->GetFontName() == pStrFontName && pFontInfo->GetSize() == nSize && 
					pFontInfo->IsBold() == bBold && pFontInfo->IsUnderline() == bUnderline && pFontInfo->IsItalic() == bItalic) 
					return pFontInfo;
			}
		}
		for( int i = 0; i< m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) {
				pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key));
				if (pFontInfo && pFontInfo->GetFontName() == pStrFontName && pFontInfo->GetSize() == nSize && 
					pFontInfo->IsBold() == bBold && pFontInfo->IsUnderline() == bUnderline && pFontInfo->IsItalic() == bItalic) 
					return pFontInfo;
			}
		}

		return NULL;
	}

	int CPaintManagerUI::GetFontHeight(int id)
	{
		return GetFont(id)->GetHeight(this);
	}

	void CPaintManagerUI::RemoveFont(UIFont *uiFont, bool bShared)
	{
		UIFont* pFontInfo = NULL;
		if (bShared)
		{
			for( int i = 0; i < m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) 
			{
				if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) 
				{
					pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key));
					if (pFontInfo && pFontInfo == uiFont) 
					{
						pFontInfo->Release();
						m_SharedResInfo.m_CustomFonts.Remove(key);
						return;
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++ ) 
			{
				if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i)) 
				{
					pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key));
					if (pFontInfo && pFontInfo == uiFont) 
					{
						pFontInfo->Release();
						m_ResInfo.m_CustomFonts.Remove(key);
						return;
					}
				}
			}
		}
	}

	void CPaintManagerUI::RemoveFont(int id, bool bShared)
	{
		TCHAR idBuffer[16];
		::ZeroMemory(idBuffer, sizeof(idBuffer));
		_itot(id, idBuffer, 10);

		UIFont* pFontInfo = NULL;
		if (bShared)
		{
			pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
			if (pFontInfo)
			{
				pFontInfo->Release();
				m_SharedResInfo.m_CustomFonts.Remove(idBuffer);
			}
		}
		else
		{
			pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
			if (pFontInfo)
			{
				pFontInfo->Release();
				m_ResInfo.m_CustomFonts.Remove(idBuffer);
			}
		}
	}

	void CPaintManagerUI::RemoveAllFonts(bool bShared)
	{
		UIFont* pFontInfo;
		if (bShared)
		{
			for( int i = 0; i< m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) {
					pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key, false));
					if (pFontInfo) {
						pFontInfo->Release();
					}
				}
			}
			m_SharedResInfo.m_CustomFonts.RemoveAll();
		}
		else
		{
			for( int i = 0; i< m_ResInfo.m_CustomFonts.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i)) {
					pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key, false));
					if (pFontInfo) {
						pFontInfo->Release();
					}
				}
			}
			m_ResInfo.m_CustomFonts.RemoveAll();
		}
	}

	UIFont* CPaintManagerUI::CloneFont(int id)
	{
		return GetFont(id)->Clone(this);
	}

	const UIImage* CPaintManagerUI::GetImage(LPCTSTR bitmap)
	{
		UIImage* data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(bitmap));
		if( !data ) data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
		return data;
	}

	const UIImage* CPaintManagerUI::GetImageEx(LPCTSTR bitmap, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, bool bUseHSL, HINSTANCE instance)
	{
		CDuiString sImageName = bitmap;
		if(width !=0 || height != 0 || fillcolor != 0)
		{
			//����ж���svg�Ŀ�ȡ��߶ȡ����ɫ��������һ��Ž�����Ϊ�ؼ��֡�
			sImageName.Format(_T("%s-%d-%d-%08X"), bitmap, width, height, fillcolor);
		}

		const UIImage* data = GetImage(sImageName);
		if( !data ) {
			if( AddImage(bitmap, type, mask, width, height, fillcolor, bUseHSL, false, instance) ) {
				if (m_bForceUseSharedRes) data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				else data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName)); 
			}
		}

		return data;
	}

	const UIImage* CPaintManagerUI::GetImageExX(const TDrawInfo *pDrawInfo, HINSTANCE instance)
	{
		CDuiString sImageName = pDrawInfo->sImageName;
		if(pDrawInfo->width !=0 || pDrawInfo->height != 0 || pDrawInfo->fillcolor != 0)
		{
			//����ж���svg�Ŀ�ȡ��߶ȡ����ɫ��������һ��Ž�����Ϊ�ؼ��֡�
			sImageName.Format(_T("%s-%d-%d-%08X"), pDrawInfo->sImageName.GetData(), pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor);		
		}

		const UIImage* data = GetImage(sImageName);
		if( !data ) {
			if( AddImageX(pDrawInfo, false, instance) ) {
				if (m_bForceUseSharedRes) data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				else data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName)); 
			}
		}

		return data;
	}

	const UIImage* CPaintManagerUI::AddImageX(const TDrawInfo *pDrawInfo, bool bShared, HINSTANCE instance)
	{
		if(pDrawInfo->sImageName.IsEmpty()) return NULL;

		CDuiString sImageName = pDrawInfo->sImageName;
		if(pDrawInfo->width !=0 || pDrawInfo->height != 0 || pDrawInfo->fillcolor != 0)
		{
			//����ж���svg�Ŀ�ȡ��߶ȡ����ɫ��������һ��Ž�����Ϊ�ؼ��֡�
			sImageName.Format(_T("%s-%d-%d-%08X"), pDrawInfo->sImageName.GetData(), pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor);
		}

		UIImage* data = UIGlobal::CreateImage();
		if(!data->LoadImage(pDrawInfo, this, instance))
		{
			data->Release();
			return NULL;
		}	

		data->bUseHSL = pDrawInfo->bHSL;
		data->sResType = pDrawInfo->sResType;
		data->dwMask = pDrawInfo->dwMask;

		if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
		if (data)
		{
			if (bShared || m_bForceUseSharedRes)
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_SharedResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_SharedResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release(); 
				}
			}
			else
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_ResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_ResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release(); 
				}
			}
		}

		return data;
	}

	const UIImage* CPaintManagerUI::AddImage(LPCTSTR bitmap, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, bool bUseHSL, bool bShared, HINSTANCE instance)
	{
		if( bitmap == NULL || bitmap[0] == _T('\0') ) return NULL;

		CDuiString sImageName = bitmap;
		if(width !=0 || height != 0 || fillcolor != 0)
		{
			//����ж���svg�Ŀ�ȡ��߶ȡ����ɫ��������һ��Ž�����Ϊ�ؼ��֡�
			sImageName.Format(_T("%s-%d-%d-%08X"), bitmap, width, height, fillcolor);
		}

		UIImage* data = UIGlobal::CreateImage();
		if( type != NULL && lstrlen(type) > 0 && isdigit(*bitmap)) 
		{
			LPTSTR pstr = NULL;
			int iIndex = _tcstol(bitmap, &pstr, 10);
			if(!data->LoadImage(iIndex, type, mask, width, height, fillcolor, this, instance))
			{
				data->Release();
				return NULL;
			}	
		}
		else 
		{
			if(!data->LoadImage(bitmap, NULL, mask, width, height, fillcolor, this, instance))
			{
				data->Release();
				return NULL;
			}
		}

		data->bUseHSL = bUseHSL;
		if( type != NULL ) data->sResType = type;
		data->dwMask = mask;
		if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
		if (data)
		{
			if (bShared || m_bForceUseSharedRes)
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_SharedResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_SharedResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release();
				}
			}
			else
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_ResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_ResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release(); 
				}
			}
		}

		return data;
	}

	const UIImage* CPaintManagerUI::AddImage(LPCTSTR bitmap, HBITMAP hBitmap, bool bAlpha, bool bShared)
	{
		// ���޷�ȷ���ⲿHBITMAP��ʽ������ʹ��hsl����
		if( bitmap == NULL || bitmap[0] == _T('\0') ) return NULL;
		if( hBitmap == NULL ) return NULL;

		UIImage* data = UIGlobal::CreateImage();
		data->CreateImage(hBitmap, bAlpha);

		if (bShared || m_bForceUseSharedRes)
		{
			if( !m_SharedResInfo.m_ImageHash.Insert(bitmap, data) ) {
				data->Release(); 
			}
		}
		else
		{
			if( !m_ResInfo.m_ImageHash.Insert(bitmap, data) ) {
				data->Release(); 
			}
		}

		return data;
	}

	void CPaintManagerUI::RemoveImage(LPCTSTR bitmap, bool bShared)
	{
		UIImage* data = NULL;
		if (bShared) 
		{
			data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
			if (data)
			{
				data->Release(); 
				m_SharedResInfo.m_ImageHash.Remove(bitmap);
			}
		}
		else
		{
			data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(bitmap));
			if (data)
			{
				data->Release(); 
				m_ResInfo.m_ImageHash.Remove(bitmap);
			}
		}
	}

	void CPaintManagerUI::RemoveAllImages(bool bShared)
	{
		if (bShared)
		{
			UIImage* data;
			for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i)) {
					data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(key, false));
					if (data) {
						data->Release(); 
					}
				}
			}
			m_SharedResInfo.m_ImageHash.RemoveAll();
		}
		else
		{
			UIImage* data;
			for( int i = 0; i< m_ResInfo.m_ImageHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_ImageHash.GetAt(i)) {
					data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(key, false));
					if (data) 
					{
						data->Release(); 
					}
				}
			}
			m_ResInfo.m_ImageHash.RemoveAll();
		}
	}

	void CPaintManagerUI::AdjustSharedImagesHSL()
	{
		UIImage* data;
		for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) 
		{
			if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i)) 
			{
				data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(key));
				if( data && (data->bUseHSL || IsForceHSL()) )
				{
					data->AdjustHslImage(m_bUseHSL, m_H, m_S, m_L);
				}
			}
		}
	}

	void CPaintManagerUI::AdjustImagesHSL()
	{
		UIImage* data;
		for( int i = 0; i< m_ResInfo.m_ImageHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_ResInfo.m_ImageHash.GetAt(i)) {
				data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(key));
				if( data && (data->bUseHSL || IsForceHSL()) )
				{
					data->AdjustHslImage(m_bUseHSL, m_H, m_S, m_L);
				}
			}
		}
		Invalidate();
	}

	void CPaintManagerUI::PostAsyncNotify()
	{
		if (!m_bAsyncNotifyPosted) {
			::PostMessage(m_hWndPaint, WM_APP + 1, 0, 0L);
			m_bAsyncNotifyPosted = true;
		}
	}
	void CPaintManagerUI::ReloadSharedImages()
	{
		UIImage* data = NULL;
		for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) 
		{
			if(LPCTSTR bitmap = m_SharedResInfo.m_ImageHash.GetAt(i)) 
			{
				data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
				if( data != NULL ) 
				{
					if( !data->sResType.IsEmpty() ) 
					{
						if( isdigit(*bitmap) ) 
						{
							LPTSTR pstr = NULL;
							int iIndex = _tcstol(bitmap, &pstr, 10);
							data->LoadImage(iIndex, data->sResType.GetData(), data->dwMask);
						}
					}
					else 
					{
						data->LoadImage(bitmap, NULL, data->dwMask);
					}

					if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
				}
			}
		}
	}

	void CPaintManagerUI::ReloadImages()
	{
		RemoveAllDrawInfos();

		UIImage* data = NULL;
		for( int i = 0; i< m_ResInfo.m_ImageHash.GetSize(); i++ ) 
		{
			if(LPCTSTR bitmap = m_ResInfo.m_ImageHash.GetAt(i)) 
			{
				data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(bitmap));
				if( data != NULL ) {
					if( !data->sResType.IsEmpty() ) 
					{
						if( isdigit(*bitmap) ) 
						{
							LPTSTR pstr = NULL;
							int iIndex = _tcstol(bitmap, &pstr, 10);
							data->LoadImage(iIndex, data->sResType.GetData(), data->dwMask);
						}
					}
					else 
					{
						data->LoadImage(bitmap, NULL, data->dwMask);
					}

					if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
				}
			}
		}

		if( m_pRoot ) m_pRoot->Invalidate();
	}

	const TDrawInfo* CPaintManagerUI::GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		CDuiString sStrImage = pStrImage;
		CDuiString sStrModify = pStrModify;
		CDuiString sKey = sStrImage + sStrModify;
		TDrawInfo* pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(sKey));
		if(pDrawInfo == NULL && !sKey.IsEmpty()) {
			pDrawInfo = new TDrawInfo();
			pDrawInfo->Parse(pStrImage, pStrModify,this);
			m_ResInfo.m_DrawInfoHash.Insert(sKey, pDrawInfo);
		}
		return pDrawInfo;
	}

	void CPaintManagerUI::RemoveDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		CDuiString sStrImage = pStrImage;
		CDuiString sStrModify = pStrModify;
		CDuiString sKey = sStrImage + sStrModify;
		TDrawInfo* pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(sKey));
		if(pDrawInfo != NULL) {
			m_ResInfo.m_DrawInfoHash.Remove(sKey);
			delete pDrawInfo;
			pDrawInfo = NULL;
		}
	}

	void CPaintManagerUI::RemoveAllDrawInfos()
	{
		TDrawInfo* pDrawInfo = NULL;
		for( int i = 0; i< m_ResInfo.m_DrawInfoHash.GetSize(); i++ ) {
			LPCTSTR key = m_ResInfo.m_DrawInfoHash.GetAt(i);
			if(key != NULL) {
				pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(key, false));
				if (pDrawInfo) {
					delete pDrawInfo;
					pDrawInfo = NULL;
				}
			}
		}
		m_ResInfo.m_DrawInfoHash.RemoveAll();
	}

	void CPaintManagerUI::AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, bool bShared)
	{
		if (bShared || m_bForceUseSharedRes)
		{
			CDuiString* pDefaultAttr = new CDuiString(pStrControlAttrList);
			if (pDefaultAttr != NULL)
			{
				CDuiString* pOldDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Set(pStrControlName, (LPVOID)pDefaultAttr));
				if (pOldDefaultAttr) delete pOldDefaultAttr;
			}
		}
		else
		{
			CDuiString* pDefaultAttr = new CDuiString(pStrControlAttrList);
			if (pDefaultAttr != NULL)
			{
				CDuiString* pOldDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Set(pStrControlName, (LPVOID)pDefaultAttr));
				if (pOldDefaultAttr) delete pOldDefaultAttr;
			}
		}
	}

	LPCTSTR CPaintManagerUI::GetDefaultAttributeList(LPCTSTR pStrControlName) const
	{
		CDuiString* pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(pStrControlName));
		if( !pDefaultAttr ) pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(pStrControlName));
		if (pDefaultAttr) return pDefaultAttr->GetData();
		return NULL;
	}

	bool CPaintManagerUI::RemoveDefaultAttributeList(LPCTSTR pStrControlName, bool bShared)
	{
		if (bShared)
		{
			CDuiString* pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(pStrControlName));
			if( !pDefaultAttr ) return false;

			delete pDefaultAttr;
			return m_SharedResInfo.m_AttrHash.Remove(pStrControlName);
		}
		else
		{
			CDuiString* pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(pStrControlName));
			if( !pDefaultAttr ) return false;

			delete pDefaultAttr;
			return m_ResInfo.m_AttrHash.Remove(pStrControlName);
		}
	}

	void CPaintManagerUI::RemoveAllDefaultAttributeList(bool bShared)
	{
		if (bShared)
		{
			CDuiString* pDefaultAttr;
			for( int i = 0; i< m_SharedResInfo.m_AttrHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_AttrHash.GetAt(i)) {
					pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(key));
					if (pDefaultAttr) delete pDefaultAttr;
				}
			}
			m_SharedResInfo.m_AttrHash.RemoveAll();
		}
		else
		{
			CDuiString* pDefaultAttr;
			for( int i = 0; i< m_ResInfo.m_AttrHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_AttrHash.GetAt(i)) {
					pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(key));
					if (pDefaultAttr) delete pDefaultAttr;
				}
			}
			m_ResInfo.m_AttrHash.RemoveAll();
		}
	}

	void CPaintManagerUI::AddWindowCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr)
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') || pstrAttr == NULL || pstrAttr[0] == _T('\0') ) return;
		CDuiString* pCostomAttr = new CDuiString(pstrAttr);
		if (pCostomAttr != NULL) {
			if (m_mWindowCustomAttrHash.Find(pstrName) == NULL)
				m_mWindowCustomAttrHash.Set(pstrName, (LPVOID)pCostomAttr);
			else
				delete pCostomAttr;
		}
	}

	LPCTSTR CPaintManagerUI::GetWindowCustomAttribute(LPCTSTR pstrName) const
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') ) return NULL;
		CDuiString* pCostomAttr = static_cast<CDuiString*>(m_mWindowCustomAttrHash.Find(pstrName));
		if( pCostomAttr ) return pCostomAttr->GetData();
		return NULL;
	}

	bool CPaintManagerUI::RemoveWindowCustomAttribute(LPCTSTR pstrName)
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') ) return NULL;
		CDuiString* pCostomAttr = static_cast<CDuiString*>(m_mWindowCustomAttrHash.Find(pstrName));
		if( !pCostomAttr ) return false;

		delete pCostomAttr;
		return m_mWindowCustomAttrHash.Remove(pstrName);
	}

	void CPaintManagerUI::RemoveAllWindowCustomAttribute()
	{
		CDuiString* pCostomAttr;
		for( int i = 0; i< m_mWindowCustomAttrHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_mWindowCustomAttrHash.GetAt(i)) {
				pCostomAttr = static_cast<CDuiString*>(m_mWindowCustomAttrHash.Find(key));
				delete pCostomAttr;
			}
		}
		m_mWindowCustomAttrHash.Resize();
	}

	CControlUI* CPaintManagerUI::GetRoot() const
	{
		//ASSERT(m_pRoot);
		return m_pRoot;
	}

	CControlUI* CPaintManagerUI::FindControl(POINT pt) const
	{
		//ASSERT(m_pRoot);
		if(!m_pRoot) return NULL;
		return m_pRoot->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
	}

	CControlUI* CPaintManagerUI::FindControl(LPCTSTR pstrName) const
	{
		//ASSERT(m_pRoot);
		if(!m_pRoot) return NULL;
		return static_cast<CControlUI*>(m_mNameHash.Find(pstrName));
	}

	CControlUI* CPaintManagerUI::FindSubControlByPoint(CControlUI* pParent, POINT pt) const
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		return pParent->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
	}

	CControlUI* CPaintManagerUI::FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		return pParent->FindControl(__FindControlFromName, (LPVOID)pstrName, UIFIND_ALL);
	}

	CControlUI* CPaintManagerUI::FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex)
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		m_aFoundControls.Resize(iIndex + 1);
		return pParent->FindControl(__FindControlFromClass, (LPVOID)pstrClass, UIFIND_ALL);
	}

	CStdPtrArray* CPaintManagerUI::FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass)
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		m_aFoundControls.Empty();
		pParent->FindControl(__FindControlsFromClass, (LPVOID)pstrClass, UIFIND_ALL);
		return &m_aFoundControls;
	}

	CStdPtrArray* CPaintManagerUI::GetFoundControls()
	{
		return &m_aFoundControls;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromNameHash(CControlUI* pThis, LPVOID pData)
	{
		CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(pData);
		const CDuiString& sName = pThis->GetName();
		if( sName.IsEmpty() ) return NULL;
		// Add this control to the hash list
		pManager->m_mNameHash.Set(sName, pThis);
		return NULL; // Attempt to add all controls
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromCount(CControlUI* /*pThis*/, LPVOID pData)
	{
		int* pnCount = static_cast<int*>(pData);
		(*pnCount)++;
		return NULL;  // Count all controls
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromPoint(CControlUI* pThis, LPVOID pData)
	{
		LPPOINT pPoint = static_cast<LPPOINT>(pData);
		return ::PtInRect(&pThis->GetPos(), *pPoint) ? pThis : NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromTab(CControlUI* pThis, LPVOID pData)
	{
		FINDTABINFO* pInfo = static_cast<FINDTABINFO*>(pData);
		if( pInfo->pFocus == pThis ) {
			if( pInfo->bForward ) pInfo->bNextIsIt = true;
			return pInfo->bForward ? NULL : pInfo->pLast;
		}
		if( (pThis->GetControlFlags() & UIFLAG_TABSTOP) == 0 ) return NULL;
		pInfo->pLast = pThis;
		if( pInfo->bNextIsIt ) return pThis;
		if( pInfo->pFocus == NULL ) return pThis;
		return NULL;  // Examine all controls
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromShortcut(CControlUI* pThis, LPVOID pData)
	{
		if( !pThis->IsVisible() ) return NULL; 
		FINDSHORTCUT* pFS = static_cast<FINDSHORTCUT*>(pData);
		if( pFS->ch == toupper(pThis->GetShortcut()) ) pFS->bPickNext = true;
		if( _tcsstr(pThis->GetClass(), _T("LabelUI")) != NULL ) return NULL;   // Labels never get focus!
		return pFS->bPickNext ? pThis : NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromName(CControlUI* pThis, LPVOID pData)
	{
		LPCTSTR pstrName = static_cast<LPCTSTR>(pData);
		const CDuiString& sName = pThis->GetName();
		if( sName.IsEmpty() ) return NULL;
		return (_tcsicmp(sName, pstrName) == 0) ? pThis : NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromClass(CControlUI* pThis, LPVOID pData)
	{
		LPCTSTR pstrType = static_cast<LPCTSTR>(pData);
		LPCTSTR pType = pThis->GetClass();
		CStdPtrArray* pFoundControls = pThis->GetManager()->GetFoundControls();
		if( _tcscmp(pstrType, _T("*")) == 0 || _tcscmp(pstrType, pType) == 0 ) {
			int iIndex = -1;
			while( pFoundControls->GetAt(++iIndex) != NULL ) ;
			if( iIndex < pFoundControls->GetSize() ) pFoundControls->SetAt(iIndex, pThis);
		}
		if( pFoundControls->GetAt(pFoundControls->GetSize() - 1) != NULL ) return pThis; 
		return NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlsFromClass(CControlUI* pThis, LPVOID pData)
	{
		LPCTSTR pstrType = static_cast<LPCTSTR>(pData);
		LPCTSTR pType = pThis->GetClass();
		if( _tcscmp(pstrType, _T("*")) == 0 || _tcscmp(pstrType, pType) == 0 ) 
			pThis->GetManager()->GetFoundControls()->Add((LPVOID)pThis);
		return NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlsFromUpdate(CControlUI* pThis, LPVOID pData)
	{
		if( pThis->IsUpdateNeeded() ) {
			pThis->GetManager()->GetFoundControls()->Add((LPVOID)pThis);
			return pThis;
		}
		return NULL;
	}

	bool CPaintManagerUI::TranslateAccelerator(LPMSG pMsg)
	{
		for (int i = 0; i < m_aTranslateAccelerator.GetSize(); i++)
		{
			LRESULT lResult = static_cast<ITranslateAccelerator *>(m_aTranslateAccelerator[i])->TranslateAccelerator(pMsg);
			if( lResult == S_OK ) return true;
		}
		return false;
	}

	bool CPaintManagerUI::TranslateMessage(const LPMSG pMsg)
	{
		// Pretranslate Message takes care of system-wide messages, such as
		// tabbing and shortcut key-combos. We'll look for all messages for
		// each window and any child control attached.
		UINT uStyle = GetWindowStyle(pMsg->hwnd);
		UINT uChildRes = uStyle & WS_CHILD;	
		LRESULT lRes = 0;
		if (uChildRes != 0)
		{
			HWND hWndParent = ::GetParent(pMsg->hwnd);

			for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) 
			{
				CPaintManagerUI* pT = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);        
				HWND hTempParent = hWndParent;
				while(hTempParent)
				{
					if(pMsg->hwnd == pT->GetPaintWindow() || hTempParent == pT->GetPaintWindow())
					{
						if (pT->TranslateAccelerator(pMsg))
							return true;

						pT->PreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes);
					}
					hTempParent = GetParent(hTempParent);
				}
			}
		}
		else
		{
			for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) 
			{
				CPaintManagerUI* pT = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
				if(pMsg->hwnd == pT->GetPaintWindow())
				{
					if (pT->TranslateAccelerator(pMsg))
						return true;

					if( pT->PreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes) ) 
						return true;

					return false;
				}
			}
		}
		return false;
	}

	bool CPaintManagerUI::AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
	{
		ASSERT(m_aTranslateAccelerator.Find(pTranslateAccelerator) < 0);
		return m_aTranslateAccelerator.Add(pTranslateAccelerator);
	}

	bool CPaintManagerUI::RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
	{
		for (int i = 0; i < m_aTranslateAccelerator.GetSize(); i++)
		{
			if (static_cast<ITranslateAccelerator *>(m_aTranslateAccelerator[i]) == pTranslateAccelerator)
			{
				return m_aTranslateAccelerator.Remove(i);
			}
		}
		return false;
	}

	void CPaintManagerUI::UsedVirtualWnd(bool bUsed)
	{
		m_bUsedVirtualWnd = bUsed;
	}

	// ��ʽ����
	void CPaintManagerUI::AddStyle(LPCTSTR pName, LPCTSTR pDeclarationList, bool bShared)
	{
		CDuiString* pStyle = new CDuiString(pDeclarationList);
		if(!pStyle) return;

		if(bShared || m_bForceUseSharedRes)
		{
			//������������´��ļ� �� ��ʱ����Style���塣 modify by liqs99
			CDuiString *pOldStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Set(pName, pStyle));
			if(pOldStyle) delete pOldStyle;
		}
		else
		{
			//������������´��ļ� �� ��ʱ����Style���塣 modify by liqs99
			CDuiString *pOldStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Set(pName, pStyle));
			if(pOldStyle) delete pOldStyle;
		}
	}

	LPCTSTR CPaintManagerUI::GetStyle(LPCTSTR pName) const
	{
		CDuiString* pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(pName));
		if( !pStyle ) pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(pName));
		if( pStyle ) return pStyle->GetData();
		else return NULL;
	}

	BOOL CPaintManagerUI::RemoveStyle(LPCTSTR pName, bool bShared)
	{
		CDuiString* pStyle = NULL;
		if (bShared) 
		{
			pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(pName));
			if (pStyle)
			{
				delete pStyle;
				m_SharedResInfo.m_StyleHash.Remove(pName);
			}
		}
		else
		{
			pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(pName));
			if (pStyle)
			{
				delete pStyle;
				m_ResInfo.m_StyleHash.Remove(pName);
			}
		}
		return true;
	}

	const CStdStringPtrMap& CPaintManagerUI::GetStyles(bool bShared) const
	{
		if(bShared) return m_SharedResInfo.m_StyleHash;
		else return m_ResInfo.m_StyleHash;
	}

	void CPaintManagerUI::RemoveAllStyle(bool bShared)
	{
		if (bShared)
		{
			CDuiString* pStyle;
			for( int i = 0; i< m_SharedResInfo.m_StyleHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_StyleHash.GetAt(i)) {
					pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(key));
					delete pStyle;
				}
			}
			m_SharedResInfo.m_StyleHash.RemoveAll();
		}
		else
		{
			CDuiString* pStyle;
			for( int i = 0; i< m_ResInfo.m_StyleHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_StyleHash.GetAt(i)) {
					pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(key));
					delete pStyle;
				}
			}
			m_ResInfo.m_StyleHash.RemoveAll();
		}
	}

	const UIImage* CPaintManagerUI::GetImageString(LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		CDuiString sImageName = pStrImage;
		CDuiString sImageResType = _T("");
		DWORD dwMask = 0;
		CDuiString sItem;
		CDuiString sValue;
		LPTSTR pstr = NULL;

		for( int i = 0; i < 2; ++i) {
			if( i == 1)
				pStrImage = pStrModify;

			if( !pStrImage ) continue;

			while( *pStrImage != _T('\0') ) {
				sItem.Empty();
				sValue.Empty();
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				while( *pStrImage != _T('\0') && *pStrImage != _T('=') && *pStrImage > _T(' ') ) {
					LPTSTR pstrTemp = ::CharNext(pStrImage);
					while( pStrImage < pstrTemp) {
						sItem += *pStrImage++;
					}
				}
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				if( *pStrImage++ != _T('=') ) break;
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				if( *pStrImage++ != _T('\'') ) break;
				while( *pStrImage != _T('\0') && *pStrImage != _T('\'') ) {
					LPTSTR pstrTemp = ::CharNext(pStrImage);
					while( pStrImage < pstrTemp) {
						sValue += *pStrImage++;
					}
				}
				if( *pStrImage++ != _T('\'') ) break;
				if( !sValue.IsEmpty() ) {
					if( sItem == _T("file") || sItem == _T("res") ) {
						sImageName = sValue;
					}
					else if( sItem == _T("restype") ) {
						sImageResType = sValue;
					}
					else if( sItem == _T("mask") ) 
					{
						if( sValue[0] == _T('#')) dwMask = _tcstoul(sValue.GetData() + 1, &pstr, 16);
						else dwMask = _tcstoul(sValue.GetData(), &pstr, 16);
					}

				}
				if( *pStrImage++ != _T(' ') ) break;
			}
		}
		return GetImageEx(sImageName, sImageResType, dwMask);
	}

	bool CPaintManagerUI::InitDragDrop()
	{
		AddRef();

		if(FAILED(RegisterDragDrop(m_hWndPaint, this))) //calls addref
		{
			DWORD dwError = GetLastError();
			return false;
		}
		else Release(); //i decided to AddRef explicitly after new

		FORMATETC ftetc={0};
		ftetc.cfFormat = CF_BITMAP;
		ftetc.dwAspect = DVASPECT_CONTENT;
		ftetc.lindex = -1;
		ftetc.tymed = TYMED_GDI;
		AddSuportedFormat(ftetc);
		ftetc.cfFormat = CF_DIB;
		ftetc.tymed = TYMED_HGLOBAL;
		AddSuportedFormat(ftetc);
		ftetc.cfFormat = CF_HDROP;
		ftetc.tymed = TYMED_HGLOBAL;
		AddSuportedFormat(ftetc);
		ftetc.cfFormat = CF_ENHMETAFILE;
		ftetc.tymed = TYMED_ENHMF;
		AddSuportedFormat(ftetc);
		return true;
	}
	static WORD DIBNumColors(void* pv) 
	{     
		int bits;     
		LPBITMAPINFOHEADER  lpbi;     
		LPBITMAPCOREHEADER  lpbc;      
		lpbi = ((LPBITMAPINFOHEADER)pv);     
		lpbc = ((LPBITMAPCOREHEADER)pv);      
		/*  With the BITMAPINFO format headers, the size of the palette 
		*  is in biClrUsed, whereas in the BITMAPCORE - style headers, it      
		*  is dependent on the bits per pixel ( = 2 raised to the power of      
		*  bits/pixel).
		*/     
		if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
		{         
			if (lpbi->biClrUsed != 0)
				return (WORD)lpbi->biClrUsed;         
			bits = lpbi->biBitCount;     
		}     
		else         
			bits = lpbc->bcBitCount;
		switch (bits)
		{         
		case 1:                 
			return 2;         
		case 4:                 
			return 16;         
		case 8:       
			return 256;
		default:
			/* A 24 bitcount DIB has no color table */                 
			return 0;
		} 
	} 
	//code taken from SEEDIB MSDN sample
	static WORD ColorTableSize(LPVOID lpv)
	{
		LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)lpv;

		if (lpbih->biSize != sizeof(BITMAPCOREHEADER))
		{
			if (((LPBITMAPINFOHEADER)(lpbih))->biCompression == BI_BITFIELDS)
				/* Remember that 16/32bpp dibs can still have a color table */
				return (sizeof(DWORD) * 3) + (DIBNumColors (lpbih) * sizeof (RGBQUAD));
			else
				return (WORD)(DIBNumColors (lpbih) * sizeof (RGBQUAD));
		}
		else
			return (WORD)(DIBNumColors (lpbih) * sizeof (RGBTRIPLE));
	}

	bool CPaintManagerUI::OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium,DWORD *pdwEffect)
	{
		POINT ptMouse = {0};
		GetCursorPos(&ptMouse);
		::SendMessage(m_hTargetWnd, WM_LBUTTONUP, NULL, MAKELPARAM(ptMouse.x, ptMouse.y));

		if(pFmtEtc->cfFormat == CF_DIB && medium.tymed == TYMED_HGLOBAL)
		{
			if(medium.hGlobal != NULL)
			{
				LPBITMAPINFOHEADER  lpbi = (BITMAPINFOHEADER*)GlobalLock(medium.hGlobal);
				if(lpbi != NULL)
				{
					HBITMAP hbm;
					HDC hdc = GetDC(NULL);
					if(hdc != NULL)
					{
						int i = ((BITMAPFILEHEADER *)lpbi)->bfOffBits;
						hbm = CreateDIBitmap(hdc,(LPBITMAPINFOHEADER)lpbi,
							(LONG)CBM_INIT,
							(LPSTR)lpbi + lpbi->biSize + ColorTableSize(lpbi),
							(LPBITMAPINFO)lpbi,DIB_RGB_COLORS);

						::ReleaseDC(NULL,hdc);
					}
					GlobalUnlock(medium.hGlobal);
					if(hbm != NULL)
						hbm = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm);
					if(hbm != NULL)
						DeleteObject(hbm);
					return true; //release the medium
				}
			}
		}
		if(pFmtEtc->cfFormat == CF_BITMAP && medium.tymed == TYMED_GDI)
		{
			if(medium.hBitmap != NULL)
			{
				HBITMAP hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)medium.hBitmap);
				if(hBmp != NULL)
					DeleteObject(hBmp);
				return false; //don't free the bitmap
			}
		}
		if(pFmtEtc->cfFormat == CF_ENHMETAFILE && medium.tymed == TYMED_ENHMF)
		{
			ENHMETAHEADER emh;
			GetEnhMetaFileHeader(medium.hEnhMetaFile, sizeof(ENHMETAHEADER),&emh);
			RECT rc;//={0,0,EnhMetaHdr.rclBounds.right-EnhMetaHdr.rclBounds.left, EnhMetaHdr.rclBounds.bottom-EnhMetaHdr.rclBounds.top};
			HDC hDC= GetDC(m_hTargetWnd);
			//start code: taken from ENHMETA.EXE MSDN Sample
			//*ALSO NEED to GET the pallete (select and RealizePalette it, but i was too lazy*
			// Get the characteristics of the output device
			float PixelsX = (float)GetDeviceCaps( hDC, HORZRES );
			float PixelsY = (float)GetDeviceCaps( hDC, VERTRES );
			float MMX = (float)GetDeviceCaps( hDC, HORZSIZE );
			float MMY = (float)GetDeviceCaps( hDC, VERTSIZE );
			// Calculate the rect in which to draw the metafile based on the
			// intended size and the current output device resolution
			// Remember that the intended size is given in 0.01mm units, so
			// convert those to device units on the target device
			rc.top = (int)((float)(emh.rclFrame.top) * PixelsY / (MMY*100.0f));
			rc.left = (int)((float)(emh.rclFrame.left) * PixelsX / (MMX*100.0f));
			rc.right = (int)((float)(emh.rclFrame.right) * PixelsX / (MMX*100.0f));
			rc.bottom = (int)((float)(emh.rclFrame.bottom) * PixelsY / (MMY*100.0f));
			//end code: taken from ENHMETA.EXE MSDN Sample

			HDC hdcMem = CreateCompatibleDC(hDC);
			HGDIOBJ hBmpMem = CreateCompatibleBitmap(hDC, emh.rclBounds.right, emh.rclBounds.bottom);
			HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBmpMem);
			PlayEnhMetaFile(hdcMem,medium.hEnhMetaFile,&rc);
			HBITMAP hBmp = (HBITMAP)::SelectObject(hdcMem, hOldBmp);
			DeleteDC(hdcMem);
			ReleaseDC(m_hTargetWnd,hDC);
			hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp);
			if(hBmp != NULL)
				DeleteObject(hBmp);
			return true;
		}
		if(pFmtEtc->cfFormat == CF_HDROP && medium.tymed == TYMED_HGLOBAL)
		{
			HDROP hDrop = (HDROP)GlobalLock(medium.hGlobal);
			if(hDrop != NULL)
			{
				TCHAR szFileName[MAX_PATH];
				UINT cFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); 
				if(cFiles > 0)
				{
					DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName)); 
					HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, szFileName,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
					if(hBitmap)
					{
						HBITMAP hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
						if(hBmp != NULL)
							DeleteObject(hBmp);
					}
				}
				//DragFinish(hDrop); // base class calls ReleaseStgMedium
			}
			GlobalUnlock(medium.hGlobal);
		}
		return true; //let base free the medium
	}

	//////////////////////////////////////////////////////////////////////////
	// �ű�
	//////////////////////////////////////////////////////////////////////////
	CREATE_SCRIPT_ENGINE_INSTANCE CPaintManagerUI::m_funCreateScriptEngine = NULL;	//add by liqs99
	DELETE_SCRIPT_ENGINE_INSTANCE CPaintManagerUI::m_funDeleteScriptEngine = NULL;	//add by liqs99
	IScriptManager* CPaintManagerUI::m_pSharedScriptEngine = NULL;					//add by liqs99

	// 	bool CPaintManagerUI::LoadScriptPlugin(LPCTSTR pstrModuleName)
	// 	{
	// 		ASSERT( !::IsBadStringPtr(pstrModuleName,-1) || pstrModuleName == NULL );
	// 		if( pstrModuleName == NULL ) return false;
	// 		HMODULE hModule = ::LoadLibrary(pstrModuleName);
	// 		if( hModule != NULL ) 
	// 		{
	// 			m_funCreateScriptEngine = (CREATE_SCRIPT_ENGINE_INSTANCE)::GetProcAddress(hModule, "CreateScriptEngine");
	// 			m_funDeleteScriptEngine = (DELETE_SCRIPT_ENGINE_INSTANCE)::GetProcAddress(hModule, "DeleteScriptEngine");
	// 			if(m_funCreateScriptEngine != NULL || m_funDeleteScriptEngine != NULL)
	// 			{
	// 				return true;
	// 			}
	// 		}
	// 		return false;
	// 	}
	bool CPaintManagerUI::LoadScriptPlugin(CREATE_SCRIPT_ENGINE_INSTANCE pFunCreate, DELETE_SCRIPT_ENGINE_INSTANCE pFunDelete)
	{
		m_funCreateScriptEngine = (CREATE_SCRIPT_ENGINE_INSTANCE)pFunCreate;
		m_funDeleteScriptEngine = (DELETE_SCRIPT_ENGINE_INSTANCE)pFunDelete;
		return true;
	}

	IScriptManager *CPaintManagerUI::GetScriptEngine(bool bCreateScriptEngine)
	{
		if(m_funCreateScriptEngine == NULL)	
			return NULL;

		if(m_pSharedScriptEngine == NULL && bCreateScriptEngine)
			m_pSharedScriptEngine = (*m_funCreateScriptEngine)();
		return m_pSharedScriptEngine;
	}

	void CPaintManagerUI::AddScriptFile(LPCTSTR pstrFileName, LPCTSTR pLanguageType)
	{
		IScriptManager *pScriptEngine = GetScriptEngine(true);
		if(pScriptEngine == NULL) return;
		pScriptEngine->AddScriptFile(pstrFileName);
	}

	bool CPaintManagerUI::CompileScript()
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine == NULL) return false;
		return pScriptEngine->CompileScript();
	}

	void *CPaintManagerUI::GetScriptFunAddress(LPCTSTR lpszFunName)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine == NULL) return NULL;
		return pScriptEngine->GetFunAddress(lpszFunName);
	}

	bool CPaintManagerUI::ExecuteScript(void *pFun, CControlUI *pControl)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine)
		{
			return pScriptEngine->ExecuteScript(pFun, pControl);
		}
		return false;
	}

	bool CPaintManagerUI::ExecuteScript(void *pFun, CControlUI *pControl, TEventUI *ev)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine)
		{
			return pScriptEngine->ExecuteScript(pFun, pControl, ev);
		}
		return false;
	}

	bool CPaintManagerUI::ExecuteScript(void *pFun, CControlUI *pControl, TNotifyUI *pMsg)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine)
		{
			return pScriptEngine->ExecuteScript(pFun, pControl, pMsg);
		}

		return false;
	}

	bool CPaintManagerUI::ExecuteScript(void *pFun, CControlUI *pControl, UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine)
		{
			return pScriptEngine->ExecuteScript(pFun, pControl, pRender, rcPaint, pStopControl);
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	CLockWindowUpdateUI::CLockWindowUpdateUI(CPaintManagerUI *pManager) : m_pManager(pManager)
	{
		if(::IsWindow(m_pManager->GetPaintWindow()))
		{
			//::LockWindowUpdate(m_pManager->GetPaintWindow());
			SetWindowRedraw(pManager->GetPaintWindow(), FALSE);
		}
	}
	CLockWindowUpdateUI::~CLockWindowUpdateUI()
	{
		if(::IsWindow(m_pManager->GetPaintWindow()))
		{
			//::LockWindowUpdate(NULL);
			SetWindowRedraw(m_pManager->GetPaintWindow(), TRUE);
			m_pManager->Invalidate();
		}
	}

} // namespace DuiLib
