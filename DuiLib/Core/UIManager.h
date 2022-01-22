#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CControlUI;
	//class CIDropTarget;
	/////////////////////////////////////////////////////////////////////////////////////
	//
	enum UILIB_RESTYPE
	{
		UILIB_FILE=1,		// ���Դ����ļ�
		UILIB_ZIP,			// ���Դ���zipѹ����
		UILIB_RESOURCE,		// ������Դ
		UILIB_ZIPRESOURCE,	// ������Դ��zipѹ����
	};
	/////////////////////////////////////////////////////////////////////////////////////
	//

	enum EVENTTYPE_UI
	{
		UIEVENT__FIRST = 1,
		UIEVENT__KEYBEGIN,
		UIEVENT_KEYDOWN,
		UIEVENT_KEYUP,
		UIEVENT_CHAR,
		UIEVENT_SYSKEY,
		UIEVENT__KEYEND,
		UIEVENT__MOUSEBEGIN,
		UIEVENT_MOUSEMOVE,
		UIEVENT_MOUSELEAVE,
		UIEVENT_MOUSEENTER,
		UIEVENT_MOUSEHOVER,
		UIEVENT_BUTTONDOWN,
		UIEVENT_BUTTONUP,
		UIEVENT_RBUTTONDOWN,
		UIEVENT_RBUTTONUP,
		UIEVENT_MBUTTONDOWN,
		UIEVENT_MBUTTONUP,
		UIEVENT_DBLCLICK,
		UIEVENT_CONTEXTMENU,
		UIEVENT_SCROLLWHEEL,
		UIEVENT__MOUSEEND,
		UIEVENT_KILLFOCUS,
		UIEVENT_SETFOCUS,
		UIEVENT_WINDOWSIZE,
		UIEVENT_SETCURSOR,
		UIEVENT_TIMER,
		UIEVENT__LAST,
	};

	enum MSGTYPE_UI
	{
		// �ڲ�������Ϣ
		UIMSG_TRAYICON = WM_USER + 1,
		UIMSG_INSERT_MSG,					//CMsgWnd�����ı�
		UIMSG_SET_DPI,
		UIMSG_MENUCLICK,					//�������հ�ť��������Ϣ
		UIMSG_MENU_UPDATE_COMMAND_UI,		//���²˵���Ϣ

		// �����Զ�����Ϣ
		UIMSG_USER = WM_USER + 100,
	};
	/////////////////////////////////////////////////////////////////////////////////////
	//

	// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

	// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_UPDATETEST    0x00000008
#define UIFIND_TOP_FIRST     0x00000010
#define UIFIND_ME_FIRST      0x80000000

// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040

	typedef struct UILIB_API tagButtonState 
	{
		tagButtonState() { m_uState = 0; }

		bool IsEnabled() const					{ return (m_uState & UISTATE_DISABLED) != UISTATE_DISABLED;					}
		void SetEnabled(bool bEnable = true)	{ bEnable ? m_uState &= ~UISTATE_DISABLED : m_uState |= UISTATE_DISABLED;	}

		bool IsFocused() const					{ return (m_uState & UISTATE_FOCUSED) == UISTATE_FOCUSED;					}
		void SetFocus(bool bFocus)				{ bFocus ? m_uState |= UISTATE_FOCUSED : m_uState &= ~UISTATE_FOCUSED;		}

		bool IsHot() const						{ return (m_uState & UISTATE_HOT) == UISTATE_HOT;							}
		void SetHot(bool bHot = true)			{ bHot ? m_uState |= UISTATE_HOT : m_uState &= ~UISTATE_HOT;				}

		bool IsCapture() const					{ return (m_uState & UISTATE_CAPTURED) == UISTATE_CAPTURED;					}
		void SetCapture(bool bCaptured)			{ bCaptured ? m_uState |= UISTATE_CAPTURED : m_uState &= ~UISTATE_CAPTURED;	}

		bool IsPushed() const					{ return (m_uState & UISTATE_PUSHED) == UISTATE_PUSHED;						}
		void SetPushed(bool bPushed)			{ bPushed ? m_uState |= UISTATE_PUSHED : m_uState &= ~UISTATE_PUSHED;		}

		bool IsSelected() const					{ return (m_uState & UISTATE_SELECTED) == UISTATE_SELECTED;					}
		void SetSelected(bool bSelected)		{ bSelected ? m_uState |= UISTATE_SELECTED : m_uState &= ~UISTATE_SELECTED;	}

		bool IsReadOnly() const					{ return (m_uState & UISTATE_READONLY) == UISTATE_READONLY;					}
		void SetReadonly(bool bReadOnly)		{ bReadOnly ? m_uState |= UISTATE_READONLY : m_uState &= ~UISTATE_READONLY;	}

		UINT m_uState;
	}TButtonState;

	typedef struct UILIB_API tagTPercentInfo
	{
		double left;
		double top;
		double right;
		double bottom;
	} TPercentInfo;

	typedef struct UILIB_API tagTResInfo
	{
		DWORD m_dwDefaultDisabledColor;
		DWORD m_dwDefaultFontColor;
		DWORD m_dwDefaultLinkFontColor;
		DWORD m_dwDefaultLinkHoverFontColor;
		DWORD m_dwDefaultSelectedBkColor;
		CStdRefPtr<UIFont> m_DefaultFontInfo;
		CStdStringPtrMap m_CustomFonts;		//����
		CStdStringPtrMap m_ImageHash;		//ͼ��
		CStdStringPtrMap m_AttrHash;		
		CStdStringPtrMap m_StyleHash;		//��ʽ��
		CStdStringPtrMap m_DrawInfoHash;	//����
	} TResInfo;

	// Structure for notifications from the system
	// to the control implementation.
	typedef struct UILIB_API tagTEventUI
	{
		int Type;
		CControlUI* pSender;
		DWORD dwTimestamp;
		POINT ptMouse;
		TCHAR chKey;
		WORD wKeyState;
		WPARAM wParam;
		LPARAM lParam;
	} TEventUI;

	// Drag&Drop control
	const TCHAR* const CF_MOVECONTROL = _T("CF_MOVECONTROL");

	typedef struct UILIB_API tagTCFMoveUI
	{
		CControlUI* pControl;
	} TCFMoveUI;

	// Listener interface
	class INotifyUI
	{
	public:
		virtual void Notify(TNotifyUI& msg) = 0;
	};

	// MessageFilter interface
	class IMessageFilterUI
	{
	public:
		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
	};

	class ITranslateAccelerator
	{
	public:
		virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
	};


	//duilib script interface  add by liqs99
	class UILIB_API IScriptManager
	{
	public:
		//����ű��ļ�
		virtual bool AddScriptFile(LPCTSTR pstrFileName) = 0;

		//����ű�����
		virtual bool AddScriptCode(LPCTSTR pstrCode) = 0;

		//����ű��� һ���Լ���ű��ļ���ϣ�Ȼ��ֻ�ܱ���һ�Ρ�
		virtual bool CompileScript() = 0;

		//ִ�нű����������������(CControlUI *)
		virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl) = 0;

		//ִ�нű����������������(CControlUI *, TEventUI *); duilib�ؼ��¼���Ӧ������
		virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TEventUI *ev) = 0;

		//ִ�нű����������������(CControlUI *, TNotifyUI *pMsg); duilib��Ϣ��Ӧ������
		virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TNotifyUI *pMsg) = 0;

		//ִ�нű����������������(CControlUI *, UIRender *, const RECT&, CControlUI*); duilib��ͼ�ຯ����
		virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) = 0;
		
		//ִ�нű�������ͨ�ú�������
		virtual bool ExecuteScript(IScriptFunction *pFun) = 0;
	};
	typedef IScriptManager* (__stdcall *CREATE_SCRIPT_ENGINE_INSTANCE)();
	typedef void (__stdcall *DELETE_SCRIPT_ENGINE_INSTANCE)(IScriptManager *pEngine);

	/////////////////////////////////////////////////////////////////////////////////////
	//
	typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);

	class UILIB_API CPaintManagerUI : public CIDropTarget
	{
	public:
		CPaintManagerUI();
		~CPaintManagerUI();

	public:
		void Init(HWND hWnd, LPCTSTR pstrName = NULL);
		UIRender *Render();
		bool IsUpdateNeeded() const;
		void NeedUpdate();
		void LockUpdate(bool bLock);
		bool IsLockUpdate();
		void Invalidate();
		void Invalidate(RECT& rcItem);

		LPCTSTR GetName() const;
		HDC GetPaintDC() const;
		HWND GetPaintWindow() const;
		HWND GetTooltipWindow() const;
		int GetHoverTime() const;
		void SetHoverTime(int iTime);

		POINT GetMousePos() const;
		SIZE GetClientSize() const;
		SIZE GetInitSize();
		void SetInitSize(int cx, int cy);
		RECT& GetSizeBox();
		void SetSizeBox(RECT& rcSizeBox);
		RECT& GetCaptionRect();
		void SetCaptionRect(RECT& rcCaption);
		SIZE GetRoundCorner() const;
		void SetRoundCorner(int cx, int cy);
		SIZE GetMinInfo() const;
		void SetMinInfo(int cx, int cy);
		SIZE GetMaxInfo() const;
		void SetMaxInfo(int cx, int cy);
		bool IsShowUpdateRect() const;
		void SetShowUpdateRect(bool show);
		bool IsNoActivate();
		void SetNoActivate(bool bNoActivate);

		BYTE GetOpacity() const;
		void SetOpacity(BYTE nOpacity);

		bool IsLayered();
		void SetLayered(bool bLayered);
		RECT& GetLayeredInset();
		void SetLayeredInset(RECT& rcLayeredInset);
		BYTE GetLayeredOpacity();
		void SetLayeredOpacity(BYTE nOpacity);
		LPCTSTR GetLayeredImage();
		void SetLayeredImage(LPCTSTR pstrImage);
		TDrawInfo *GetLayeredDrawInfo();
		void SetLayeredChanged(bool bChange);
		bool IsLayeredChanged() const;
		bool IsFocusNeeded() const;

		CShadowUI* GetShadow();

		void SetUseGdiplusText(bool bUse);
		bool IsUseGdiplusText() const;
		void SetGdiplusTextRenderingHint(int trh);
		int GetGdiplusTextRenderingHint() const;

		static void SetRenderEngineType(emRenderEngine render);
		static emRenderEngine GetRenderEngineType();
		static HINSTANCE GetInstance();
		static CDuiString GetInstancePath();
		static CDuiString GetCurrentPath();
		static HINSTANCE GetResourceDll();
		static const CDuiString& GetResourcePath();
		static const CDuiString& GetResourceZip();
		static const CDuiString& GetResourceZipPwd();
		static bool IsCachedResourceZip();
		static HANDLE GetResourceZipHandle();
		static void SetInstance(HINSTANCE hInst);
		static void SetCurrentPath(LPCTSTR pStrPath);
		static void SetResourceDll(HINSTANCE hInst);
		static void SetResourcePath(LPCTSTR pStrPath);
		static void SetResourceZip(LPVOID pVoid, unsigned int len, LPCTSTR password = NULL);
		static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false, LPCTSTR password = NULL);
		static void SetResourceType(int nType);
		static int GetResourceType();

		static bool GetHSL(short* H, short* S, short* L);
		static void SetHSL(short H=180, short S=100, short L=100); // H:0~360, S:0~200, L:0~200 

		static bool IsUseHSL();

		static void SetForceHSL(bool bForceHSL);
		static bool IsForceHSL();

		static void ReloadSkin();
		static CPaintManagerUI* GetPaintManager(LPCTSTR pstrName);
		static CStdPtrArray* GetPaintManagers();
		static bool LoadPlugin(LPCTSTR pstrModuleName);
		static CStdPtrArray* GetPlugins();

		bool IsForceUseSharedRes() const;
		void SetForceUseSharedRes(bool bForce);

		void DeletePtr(void* ptr);

		DWORD GetDefaultDisabledColor() const;
		void SetDefaultDisabledColor(DWORD dwColor, bool bShared = false);
		DWORD GetDefaultFontColor() const;
		void SetDefaultFontColor(DWORD dwColor, bool bShared = false);
		DWORD GetDefaultLinkFontColor() const;
		void SetDefaultLinkFontColor(DWORD dwColor, bool bShared = false);
		DWORD GetDefaultLinkHoverFontColor() const;
		void SetDefaultLinkHoverFontColor(DWORD dwColor, bool bShared = false);
		DWORD GetDefaultSelectedBkColor() const;
		void SetDefaultSelectedBkColor(DWORD dwColor, bool bShared = false);

		UIFont* GetDefaultFontInfo();
		void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		DWORD GetCustomFontCount(bool bShared = false) const;
		
		//���ļ��а�װ����
		void AddFontArray(LPCTSTR pstrPath);

		//��������
		UIFont* AddFont(int id, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		
		//��������id��ȡ����
		UIFont* GetFont(int id);

		//��������������Ż�ȡ����
		UIFont* GetFont(int nIndex, bool bShared);

		//�������������ȡ����
		UIFont* GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		
		//��ȡ����߶�
		int GetFontHeight(int id);
		
		//ɾ��ָ������
		void RemoveFont(UIFont *uiFont, bool bShared = false);

		//ɾ��ָ��id����
		void RemoveFont(int id, bool bShared = false);

		//ɾ����������
		void RemoveAllFonts(bool bShared = false);

		//������id��¡һ������
		UIFont* CloneFont(int id);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//��ȡָ�����Ƶ�bitmap��ͼƬ��
		const UIImage* GetImage(LPCTSTR bitmap);

		//��ȡͼƬ, bitmapΪ�ļ�������Դid
		const UIImage* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, bool bUseHSL = false, HINSTANCE instance = NULL);
		
		//��ȡͼƬ, bitmapΪ�ļ�������Դid,��һ�����Ĳ����򻯰�.
		const UIImage* GetImageExX(const TDrawInfo *pDrawInfo, HINSTANCE instance = NULL);
		
		//����ͼƬ, bitmapΪ�ļ�������Դid 
		const UIImage* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, bool bUseHSL = false, bool bShared = false, HINSTANCE instance = NULL);
		
		//����ͼƬ, bitmapΪ�ļ�������Դid, ��һ�����Ĳ����򻯰�.
		const UIImage* AddImageX(const TDrawInfo *pDrawInfo, bool bShared = false, HINSTANCE instance = NULL);

		//�����Զ���ͼƬ, ��bitmap�ظ�������ʧ�ܡ�
		const UIImage* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, bool bAlpha, bool bShared = false);

		//ɾ��ָ�����Ƶ�bitmap��ͼƬ��
		void RemoveImage(LPCTSTR bitmap, bool bShared = false);

		//ɾ������ͼƬ
		void RemoveAllImages(bool bShared = false);

		//�������빲��ͼƬ
		static void ReloadSharedImages();

		//��������ǹ���ͼƬ
		void ReloadImages();

		//
		const TDrawInfo* GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
		void RemoveDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
		void RemoveAllDrawInfos();

		void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, bool bShared = false);
		LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
		bool RemoveDefaultAttributeList(LPCTSTR pStrControlName, bool bShared = false);
		void RemoveAllDefaultAttributeList(bool bShared = false);

		void AddWindowCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
		LPCTSTR GetWindowCustomAttribute(LPCTSTR pstrName) const;
		bool RemoveWindowCustomAttribute(LPCTSTR pstrName);
		void RemoveAllWindowCustomAttribute();

		// ��ʽ����
		void AddStyle(LPCTSTR pName, LPCTSTR pStyle, bool bShared = false);
		LPCTSTR GetStyle(LPCTSTR pName) const;
		BOOL RemoveStyle(LPCTSTR pName, bool bShared = false);
		const CStdStringPtrMap& GetStyles(bool bShared = false) const;
		void RemoveAllStyle(bool bShared = false);

		const UIImage* GetImageString(LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

		// ��ʼ����ק
		bool InitDragDrop();
		virtual bool OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium,DWORD *pdwEffect);

		bool AttachDialog(CControlUI* pControl);
		bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
		void ReapObjects(CControlUI* pControl);

		bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
		CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
		void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
		void RemoveAllOptionGroups();

		CControlUI* GetLastFocus() const; //add by liqs99
		CControlUI* GetFocus() const;
		void SetFocus(CControlUI* pControl);
		void SetFocusNeeded(CControlUI* pControl);

		bool SetNextTabControl(bool bForward = true);

		bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
		bool KillTimer(CControlUI* pControl, UINT nTimerID);
		void KillTimer(CControlUI* pControl);
		void RemoveAllTimers();

		void SetCapture();
		void ReleaseCapture();
		bool IsCaptured();

		bool IsPainting();
		void SetPainting(bool bIsPainting);

		bool AddNotifier(INotifyUI* pControl);
		bool RemoveNotifier(INotifyUI* pControl);   
		void SendNotify(TNotifyUI& Msg, bool bAsync = false);
		void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

		bool AddPreMessageFilter(IMessageFilterUI* pFilter);
		bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

		bool AddMessageFilter(IMessageFilterUI* pFilter);
		bool RemoveMessageFilter(IMessageFilterUI* pFilter);

		int GetPostPaintCount() const;
		bool IsPostPaint(CControlUI* pControl);
		bool AddPostPaint(CControlUI* pControl);
		bool RemovePostPaint(CControlUI* pControl);
		bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

		int GetNativeWindowCount() const;
		RECT GetNativeWindowRect(HWND hChildWnd);
		bool AddNativeWindow(CControlUI* pControl, HWND hChildWnd);
		bool RemoveNativeWindow(HWND hChildWnd);

		void AddDelayedCleanup(CControlUI* pControl);
		void AddMouseLeaveNeeded(CControlUI* pControl);
		bool RemoveMouseLeaveNeeded(CControlUI* pControl);

		bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
		bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
		bool TranslateAccelerator(LPMSG pMsg);

		CControlUI* GetRoot() const;
		CControlUI* FindControl(POINT pt) const;
		CControlUI* FindControl(LPCTSTR pstrName) const;
		CControlUI* FindControl(const CDuiString &strName) const;
		CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
		CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
		CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
		CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);

		static void MessageLoop();
		static bool TranslateMessage(const LPMSG pMsg);
		static void Term();

		CDPI* GetDPIObj();
		void ResetDPIAssets();
		void SetDPI(int iDPI);
		static void SetAllDPI(int iDPI);

		bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
		bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
		void UsedVirtualWnd(bool bUsed);

	private:
		CStdPtrArray* GetFoundControls();
		static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlsFromUpdate(CControlUI* pThis, LPVOID pData);

		static void AdjustSharedImagesHSL();
		void AdjustImagesHSL();
		void PostAsyncNotify();

	private:
		static emRenderEngine m_emRenderEngine;
		CStdRefPtr<UIRender> m_pRenderEngine;
		CDuiString m_sName;
		HWND m_hWndPaint;	//�����ӵĴ���ľ��
		HDC m_hDcPaint;

		// ��ʾ��Ϣ
		HWND m_hwndTooltip;
		TOOLINFO m_ToolTip;
		int m_iHoverTime;
		bool m_bNoActivate;
		bool m_bShowUpdateRect;

		//
		CControlUI* m_pRoot;
		CControlUI* m_pFocus;
		CControlUI* m_pLastFocus; //add by liqs99
		CControlUI* m_pEventHover;
		CControlUI* m_pEventClick;
		CControlUI* m_pEventKey;
		CControlUI* m_pLastToolTip;
		//
		POINT m_ptLastMousePos;
		SIZE m_szMinWindow;
		SIZE m_szMaxWindow;
		SIZE m_szInitWindowSize;
		RECT m_rcSizeBox;
		SIZE m_szRoundCorner;
		RECT m_rcCaption;
		UINT m_uTimerID;
		bool m_bFirstLayout;
		bool m_bUpdateNeeded;
		bool m_bLockUpdate;
		bool m_bFocusNeeded;
		bool m_bOffscreenPaint;
		
		BYTE m_nOpacity;
		bool m_bLayered;
		RECT m_rcLayeredInset;
		bool m_bLayeredChanged;
		RECT m_rcLayeredUpdate;
		TDrawInfo m_diLayered;

		bool m_bMouseTracking;
		bool m_bMouseCapture;
		bool m_bIsPainting;
		bool m_bUsedVirtualWnd;
		bool m_bAsyncNotifyPosted;

		//
		CStdPtrArray m_aNotifiers;
		CStdPtrArray m_aTimers;
		CStdPtrArray m_aTranslateAccelerator;
		CStdPtrArray m_aPreMessageFilters;
		CStdPtrArray m_aMessageFilters;
		CStdPtrArray m_aPostPaintControls;
		CStdPtrArray m_aNativeWindow;
		CStdPtrArray m_aNativeWindowControl;
		CStdPtrArray m_aDelayedCleanup;
		CStdPtrArray m_aAsyncNotify; //CDuiLock m_lockAsyncNotify; //add by liq99
		CStdPtrArray m_aFoundControls;
		CStdPtrArray m_aFonts;
		CStdPtrArray m_aNeedMouseLeaveNeeded;
		CStdStringPtrMap m_mNameHash;
		CStdStringPtrMap m_mWindowCustomAttrHash;
		CStdStringPtrMap m_mOptionGroup;
		
		bool m_bForceUseSharedRes;
		TResInfo m_ResInfo;
		
		// ������Ӱ
		CShadowUI m_shadow;
		
		// DPI������
		CDPI* m_pDPI;
		// �Ƿ���Gdiplus
		bool m_bUseGdiplusText;
		int m_trh;
		static ULONG_PTR m_gdiplusToken;
		static Gdiplus::GdiplusStartupInput *m_pGdiplusStartupInput;

		// ��ק
		bool m_bDragMode;
		HBITMAP m_hDragBitmap;
		CStdRefPtr<UIBitmap> m_dragBitmap;

		
		//
		static HINSTANCE m_hInstance;
		static HINSTANCE m_hResourceInstance;
		static CDuiString m_pStrResourcePath;
		static CDuiString m_pStrResourceZip;
		static CDuiString m_pStrResourceZipPwd;
		static HANDLE m_hResourceZip;
		static bool m_bCachedResourceZip;
		static int m_nResType;
		static TResInfo m_SharedResInfo;
		static bool m_bUseHSL;
		static bool m_bForceHSL;
		static short m_H;
		static short m_S;
		static short m_L;
		static CStdPtrArray m_aPreMessages;
		static CStdPtrArray m_aPlugins;

	public: 
		CLangManagerUI *GetLangManager() { return m_pLangManager; }
		bool IsInitWindowParameter() { return m_bInitWindowParameter; }
		void SetInitWindowParameter(bool bInit) { m_bInitWindowParameter = bInit; }
	private:
		bool m_bInitWindowParameter; //�Ƿ��Ѿ���ʼ����Window����, window����ֻ�ܳ�ʼ��һ��,��������Include�ļ��е�window���� Modify by liqs99
		CLangManagerUI *m_pLangManager;		//�����Թ�����, add by liqs99

	//////////////////////////////////////////////////////////////////////////
	// �ű�
	//////////////////////////////////////////////////////////////////////////
	public:
		//����ű�������
		static bool LoadScriptPlugin(CREATE_SCRIPT_ENGINE_INSTANCE pFunCreate, DELETE_SCRIPT_ENGINE_INSTANCE pFunDelete);
		static IScriptManager *GetScriptEngine();

		static void AddScriptFile(LPCTSTR pstrFileName, LPCTSTR pLanguageType=NULL);
		static bool CompileScript();
		static bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl);
		static bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TEventUI *ev);
		static bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TNotifyUI *pMsg);
		static bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl);
	private:
		static IScriptManager *m_pSharedScriptEngine;
		static CREATE_SCRIPT_ENGINE_INSTANCE m_funCreateScriptEngine;
		static DELETE_SCRIPT_ENGINE_INSTANCE m_funDeleteScriptEngine;

	//////////////////////////////////////////////////////////////////////////
	//�����ģʽ
	public:
		static BOOL UIDESIGNMODE;
		static BOOL UIDESIGNPREVIEW;
	};

	//���������ڸ��£�Ȼ��ǿ��ˢ����������
	class UILIB_API CLockWindowUpdateUI
	{
	public:
		CLockWindowUpdateUI(CPaintManagerUI *pManager);
		~CLockWindowUpdateUI();
	private:
		CPaintManagerUI *m_pManager;
	};
} // namespace DuiLib

#endif // __UIMANAGER_H__
