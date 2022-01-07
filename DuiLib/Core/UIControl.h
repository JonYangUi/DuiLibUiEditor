#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#pragma once

namespace DuiLib {

	/////////////////////////////////////////////////////////////////////////////////////
	//

	typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

	class UILIB_API CControlUI : public CUIAnimation
	{
		DECLARE_DUICONTROL(CControlUI)
	public:
		CControlUI();
		virtual ~CControlUI();

	public:
		virtual CDuiString GetName() const;
		virtual void SetName(LPCTSTR pstrName);
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual UINT GetControlFlags() const;

		virtual bool Activate();
		virtual CPaintManagerUI* GetManager() const;
		virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
		virtual CControlUI* GetParent() const;
	    void setInstance(HINSTANCE instance = NULL) {m_instance = instance;};
		
		// ��ʱ��
		bool SetTimer(UINT nTimerID, UINT nElapse);
		void KillTimer(UINT nTimerID);

		// �ı�����
		virtual CDuiString GetText() const;
		virtual void SetText(LPCTSTR pstrText);

		//�������͵��ı�����
		virtual int  GetTextN();
		virtual void SetTextN(int n);

		//��ʽ���ı�����
		virtual void SetTextV(LPCTSTR lpszFormat, ...);

		//�ı�����
		void SetTextStyle(UINT uStyle);
		UINT GetTextStyle() const;

		//�ı���߾�
		virtual RECT GetTextPadding() const;
		virtual void SetTextPadding(RECT rc);

		//�Ƿ�ʹ����Դ�滻�ı�
		virtual bool IsResourceText() const;
		virtual void SetResourceText(bool bResource);

		//////////////////////////////////////////////////////////////////////////
		//��һ�ֵĶ����Է���

		//��Դid
		virtual int  GetResourceID() const;	
		virtual void SetResourceID(int resid);

		//�����Թ�����
		virtual CLangPackageUI *GetLangPackage();

		//��ǰ�ؼ������ĸ�xml�ļ�
		virtual void SetSkinFile(LPCTSTR lpstrSkinFile);
		virtual CDuiString GetSkinFile();

		//////////////////////////////////////////////////////////////////////////
		//�϶�

		//�����϶�
		virtual bool IsDragEnabled() const;
		virtual void SetDragEnable(bool bDrag);

		//�����϶����
		virtual bool IsDropEnabled() const;
		virtual void SetDropEnable(bool bDrop);

		//////////////////////////////////////////////////////////////////////////
		//����
		int GetFont() const;
		virtual void SetFont(int index);
		int GetHotFont() const;
		void SetHotFont(int index);
		int GetPushedFont() const;
		void SetPushedFont(int index);
		int GetFocusedFont() const;
		void SetFocusedFont(int index);
		int GetDisabledFont() const;
		void SetDisabledFont(int index);
		int GetSelectedFont() const;
		void SetSelectedFont(int index);

		//////////////////////////////////////////////////////////////////////////
		//������ɫ
		DWORD GetTextColor() const;
		virtual void SetTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetFocusedTextColor() const;
		void SetFocusedTextColor(DWORD dwColor);
		DWORD GetDisabledTextColor() const;
		void SetDisabledTextColor(DWORD dwColor);
		DWORD GetSelectedTextColor() const;
		void SetSelectedTextColor(DWORD dwColor);

		//////////////////////////////////////////////////////////////////////////
		//��������ɫ

		//����ɫ���䷽��
		bool GetGradient();
		virtual void SetGradient(bool bVertical);

		//����ɫ���ݶ�
		DWORD GetBkColor2() const;
		void SetBkColor2(DWORD dwBackColor);

		//����ɫ���ݶ�
		DWORD GetBkColor3() const;
		void SetBkColor3(DWORD dwBackColor);

		//����ɫ
		DWORD GetBkColor() const;
		void SetBkColor(DWORD dwBackColor);
		DWORD GetHotBkColor() const;
		void SetHotBkColor(DWORD dwColor);
		DWORD GetFocusBkColor() const;
		void SetFocusBkColor(DWORD dwColor);
		DWORD GetPushedBkColor() const;
		void SetPushedBkColor(DWORD dwColor);
		DWORD GetDisabledBkColor() const;
		void SetDisabledBkColor(DWORD dwColor);
		DWORD GetSelectBkColor();
		void SetSelectedBkColor(DWORD dwBkColor);

		//////////////////////////////////////////////////////////////////////////
		//����ͼ
		CDuiString GetBkImage();
		virtual void SetBkImage(LPCTSTR pStrImage);

		//////////////////////////////////////////////////////////////////////////
		//ǰ��ɫ
		DWORD GetForeColor() const;
		void SetForeColor(DWORD dwForeColor);

		//////////////////////////////////////////////////////////////////////////
		//ǰ��ͼ
		CDuiString	GetForeImage() const;
		void		SetForeImage(LPCTSTR pStrImage);
		CDuiString	GetHotForeImage() const;
		void		SetHotForeImage(LPCTSTR pStrImage);
		CDuiString	GetPushedForeImage() const;
		void		SetPushedForeImage(LPCTSTR pStrImage);
		CDuiString	GetFocusedForeImage() const;
		void		SetFocusedForeImage(LPCTSTR pStrImage);
		CDuiString	GetDisabledForeImage() const;
		void		SetDisabledForeImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedForeImage() const;
		void		SetSelectedForedImage(LPCTSTR pStrImage);

		//////////////////////////////////////////////////////////////////////////
		//״̬���
		CDuiString	GetNormalImage() const;
		virtual void SetNormalImage(LPCTSTR pStrImage);
		CDuiString	GetHotImage() const;
		void		SetHotImage(LPCTSTR pStrImage);
		CDuiString	GetPushedImage() const;
		void		SetPushedImage(LPCTSTR pStrImage);
		CDuiString	GetFocusedImage() const;
		void		SetFocusedImage(LPCTSTR pStrImage);
		CDuiString	GetDisabledImage() const;
		void		SetDisabledImage(LPCTSTR pStrImage);

		CDuiString	GetSelectedImage() const;
		void		SetSelectedImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedHotImage() const;
		void		SetSelectedHotImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedPushedImage() const;
		void		SetSelectedPushedImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedFocusedImage() const;
		void		SetSelectedFocusedImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedDisabledImage() const;
		void		SetSelectedDisabledImage(LPCTSTR pStrImage);

		//////////////////////////////////////////////////////////////////////////
		//�߿����

		//�߿�size
		int GetBorderSize() const;
		void SetBorderSize(int nSize);

		//�߿�size������������������
		void SetBorderRectSize(RECT rc);
		RECT GetBorderRectSize() const;

		//��߿�
		int GetLeftBorderSize() const;
		void SetLeftBorderSize(int nSize);

		//�ϱ߿�
		int GetTopBorderSize() const;
		void SetTopBorderSize(int nSize);

		//�ұ߿�
		int GetRightBorderSize() const;
		void SetRightBorderSize(int nSize);

		//�±߿�
		int GetBottomBorderSize() const;
		void SetBottomBorderSize(int nSize);

		//�߿���ʽ
		int GetBorderStyle() const;
		void SetBorderStyle(int nStyle);

		//�߿�Բ��
		SIZE GetBorderRound() const;
		void SetBorderRound(SIZE cxyRound);

		//�߿���ɫ����ͨ״̬
		DWORD GetBorderColor() const;
		void SetBorderColor(DWORD dwBorderColor);

		//�߿���ɫ�������ͣ
		DWORD GetHotBorderColor() const;
		void SetHotBorderColor(DWORD dwBorderColor);

		//�߿���ɫ����갴��
		DWORD GetPushedBorderColor() const;
		void SetPushedBorderColor(DWORD dwBorderColor);

		//�߿���ɫ������״̬
		DWORD GetFocusBorderColor() const;
		void SetFocusBorderColor(DWORD dwBorderColor);

		//�߿���ɫ������ʱ
		DWORD GetDisableBorderColor() const;
		void SetDisableBorderColor(DWORD dwBorderColor);

		//�߿���ɫ��ѡ��ʱ
		DWORD GetSelectedBorderColor() const;
		void SetSelectedBorderColor(DWORD dwBorderColor);

		//////////////////////////////////////////////////////////////////////////

		//HSL��ɫ
		bool IsColorHSL() const;
		void SetColorHSL(bool bColorHSL);

		//��ͼ
		bool DrawImage(UIRender *pRender, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

		// ���(���ؼ�)λ��, ������
		virtual RECT GetRelativePos() const; 

		// �ͻ����򣨳�ȥscrollbar��inset��
		virtual RECT GetClientPos() const; 

		//�ؼ�λ�ô�С
		virtual const RECT& GetPos() const;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

		//�ƶ�����float��Ч
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);

		//�ӿؼ�����, ѯ�ʸ��ؼ����㽫����ҷ������rect��
		virtual bool CalcPos(CControlUI *pChildControl, RECT &rcChild); 

		//ʵ�ʿ�Ⱥ͸߶�
		virtual int GetWidth() const;
		virtual int GetHeight() const;

		//ʵ��λ�����Ͻ�����
		virtual int GetX() const;
		virtual int GetY() const;

		//��߾࣬���ϲ㴰�ڻ���
		virtual RECT GetPadding() const;
		virtual void SetPadding(RECT rcPadding); 

		//float�ؼ����Ͻ����꣬��floatΪtrueʱ��Ч
		virtual SIZE GetFixedXY() const;
		virtual void SetFixedXY(SIZE szXY);

		//Ԥ��Ŀ��
		virtual int GetFixedWidth() const;
		virtual void SetFixedWidth(int cx);

		//Ԥ��ĸ߶�
		virtual int GetFixedHeight() const;
		virtual void SetFixedHeight(int cy);

		//Ԥ��İٷֱȿ��
		virtual int GetFixedWidthPercent() const;
		virtual void SetFixedWidthPercent(int cx);

		//Ԥ��İٷֱȸ߶�
		virtual int GetFixedHeightPercent() const;
		virtual void SetFixedHeightPercent(int cy);

		//��С���
		virtual int GetMinWidth() const;
		virtual void SetMinWidth(int cx);
		//�����
		virtual int GetMaxWidth() const;
		virtual void SetMaxWidth(int cx);
		//��С�߶�
		virtual int GetMinHeight() const;
		virtual void SetMinHeight(int cy);
		//���߶�
		virtual int GetMaxHeight() const;
		virtual void SetMaxHeight(int cy);

		//float�ؼ��ٷֱ�
		virtual TPercentInfo GetFloatPercent() const;
		virtual void SetFloatPercent(TPercentInfo piFloatPercent);

		//float�ؼ����뷽ʽ
		virtual void SetFloatAlign(UINT uAlign);
		virtual UINT GetFloatAlign() const;

		//float�ؼ���������Ͻǰٷֱ�λ��
		virtual void SetFloatPosition(POINT ptPosition);
		virtual POINT GetFloatPosition() const;

		//�����ӿؼ������ı��ȵ��Զ�������
		virtual bool IsAutoCalcWidth() const;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth);

		//�����ӿؼ������ı��ȵ��Զ�����߶�
		virtual bool IsAutoCalcHeight() const;
		virtual void SetAutoCalcHeight(bool bAutoCalcHeight);

		// �����ʾ
		virtual CDuiString GetToolTip() const;
		virtual void SetToolTip(LPCTSTR pstrText);
		// ����ToolTip��������
		virtual void SetToolTipWidth(int nWidth);
		virtual int GetToolTipWidth(void);	
		
		// ���
		virtual WORD GetCursor();
		virtual void SetCursor(WORD wCursor);

		// ��ݼ�
		virtual TCHAR GetShortcut() const;
		virtual void SetShortcut(TCHAR ch);

		// ��Ӧ�˵���Ϣ��DUI_MSGTYPE_MENU
		virtual bool IsContextMenuUsed() const;
		virtual void SetContextMenuUsed(bool bMenuUsed);

		//�û��Զ�������
		virtual CDuiString GetUserData(); 
		virtual void SetUserData(LPCTSTR pstrText);

		//�û��Զ�������
		virtual UINT_PTR GetTag() const;
		virtual void SetTag(UINT_PTR pTag);

		//�Ƿ���ʾ
		virtual bool IsVisible() const;
		virtual void SetVisible(bool bVisible = true);

		// �����ڲ����ã���ЩUIӵ�д��ھ������Ҫ��д�˺���
		virtual void SetInternVisible(bool bVisible = true); 

		//��Ӧ���
		virtual bool IsMouseEnabled() const;
		virtual void SetMouseEnabled(bool bEnable = true);

		//��Ӧ����
		virtual bool IsKeyboardEnabled() const;
		virtual void SetKeyboardEnabled(bool bEnable = true);

		// �Ƿ�float�ؼ�
		virtual bool IsFloat() const;
		virtual void SetFloat(bool bFloat = true);

		//����״̬
		virtual bool IsEnabled() const;
		virtual void SetEnabled(bool bEnable = true);

		//����״̬
		virtual bool IsFocused() const;
		virtual void SetFocusState(bool bFocus);
		virtual void SetFocus();

		//�Ƿ������ͣ״̬
		virtual bool IsHotState() const;
		virtual void SetHotState(bool bHot = true);

		virtual bool IsCaptureState() const;
		virtual void SetCaptureState(bool bCaptured);

		//��갴��״̬
		virtual void SetPushedState(bool bPushed);
		virtual bool IsPushedState() const;

		//ѡ��״̬
		virtual void SetSelectedState(bool bSelected);
		virtual bool IsSelectedState() const;

		virtual void SetReadOnly(bool bSelected);
		virtual bool IsReadOnly() const;

		virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		//�ػ��ؼ�������������SetPos
		virtual void Invalidate();

		//����SetPos�������ػ��ؼ��������ؼ���λ�ô�С����ı�
		bool IsUpdateNeeded() const;
		void NeedUpdate();

		//���ؼ�����SetPos�������ػ��ؼ��������ؼ���λ�ô�С�ᱻ���ؼ����¼��㡣
		void NeedParentUpdate();

		//����HSL��ɫ
		DWORD GetAdjustColor(DWORD dwColor);

		virtual void Init();
		virtual void DoInit();

		virtual void Event(TEventUI& event);
		virtual void DoEvent(TEventUI& event);

		// �û��Զ�������ԣ�UI�ڲ���������
		void AddCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
		LPCTSTR GetCustomAttribute(LPCTSTR pstrName) const;
		bool RemoveCustomAttribute(LPCTSTR pstrName);
		void RemoveAllCustomAttribute();

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CControlUI* ApplyAttributeList(LPCTSTR pstrList);

		virtual SIZE EstimateSize(SIZE szAvailable);
		virtual bool Paint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl = NULL); // ����Ҫ��Ҫ��������
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl);
		virtual void PaintBkColor(UIRender *pRender);
		virtual void PaintBkImage(UIRender *pRender);
		virtual void PaintStatusImage(UIRender *pRender);
		virtual void PaintForeColor(UIRender *pRender);
		virtual void PaintForeImage(UIRender *pRender);
		virtual void PaintText(UIRender *pRender);
		virtual void PaintBorder(UIRender *pRender);

		//����ʱ���ܿؼ������С������
		virtual void DoPostPaint(UIRender *pRender, const RECT& rcPaint);

		//���ⴰ�ڲ���
		void SetVirtualWnd(LPCTSTR pstrValue);
		CDuiString GetVirtualWnd() const;

		//�ؼ���ý����ԣ��Ƿ���������ӦĬ�ϰ����¼�������Enter��Esc�رմ���
		virtual bool OnEnableResponseDefaultKeyEvent(WPARAM wParam) { return true; }
	public:
		CEventSource OnInit;
		CEventSource OnDestroy;
		CEventSource OnSize;
		CEventSource OnEvent;
		CEventSource OnNotify;
		CEventSource OnPaint;

	protected:
		CPaintManagerUI* m_pManager;
		CControlUI* m_pParent;			//���ؼ�
		CDuiString m_sVirtualWnd;		//���ⴰ��
		CDuiString m_sName;				//�ؼ�����
		bool m_bUpdateNeeded;			//�ؼ��Ƿ���Ҫˢ��
		bool m_bMenuUsed;				//�Ƿ���Ӧ�˵��¼���DUI_MSGTYPE_MENU
		RECT m_rcItem;					//�ؼ���λ�úʹ�С��ʵ����ʾʱ��
		RECT m_rcPadding;				//��߾�
		SIZE m_cXY;						//���Զ�λʱ���ؼ����Ͻ�����
		SIZE m_cxyFixed;				//Ԥ��Ŀؼ���С
		SIZE m_cxyFixedPercent;			//Ԥ��Ŀؼ���С�ٷֱ�
		SIZE m_cxyMin;					//�����Сֵ
		SIZE m_cxyMax;					//������ֵ
		bool m_bVisible;				//�Ƿ���ʾ
		bool m_bInternVisible;			//�ڲ����ã� m_bVisible && m_bInternVisible = ʵ���Ƿ���ʾ��
		bool m_bPaneVisible;			//�Ƿ���ʾ�����ڶ���
		bool m_bMouseEnabled;			//��Ӧ���
		bool m_bKeyboardEnabled;		//��Ӧ����
		TButtonState m_state;			//�ؼ�״̬

		bool m_bFloat;					//�Ƿ���Զ�λ
		TPercentInfo m_piFloatPercent;	//���Զ�λ�ٷֱ�
		UINT m_uFloatAlign;				//���Զ�λ��������ʽ
		POINT m_ptFloatPosition;		//���Զ�λ, ���ؼ����Ͻǵİٷֱ�λ��

		bool m_bSetPos;					// ��ֹSetPosѭ������

		bool m_bAutoCalcWidth;			//�����ӿؼ������ı��ȵ��Զ�������
		bool m_bAutoCalcHeight;			//�����ӿؼ������ı��ȵ��Զ�����߶�

		bool m_bDragEnabled;			//�����϶�
		bool m_bDropEnabled;			//���������ؼ����϶�ʱ���µ���ǰ�ؼ�

		bool m_bResourceText;			//�Ƿ�ʹ����Դ�滻�ı�

		int  m_nResourceID;				//��Դid
		CDuiString m_sSkinFile;			//�������ĸ��ļ��� add by liqs99

		CDuiString m_sUserData;			//�û��Զ�������
		UINT_PTR m_pTag;				//�û��Զ�������

		CDuiString m_sText;				//�ı�����
		UINT m_uTextStyle;				//�ı�����
		RECT m_rcTextPadding;			//�ı���߾�
		CDuiString m_sToolTip;			//������ʾ���ı�
		int m_nTooltipWidth;			//������ʾ����
		TCHAR m_chShortcut;				//��ݼ�, Alt+��ĸ

		POINT m_ptLastMouse;
		//////////////////////////////////////////////////////////////////////////
		//����
		int	m_iFont;
		int	m_iHotFont;
		int	m_iPushedFont;
		int	m_iFocusedFont;
		int	m_iDisabledFont;
		int	m_iSelectedFont;

		//////////////////////////////////////////////////////////////////////////
		//�ı���ɫ
		DWORD m_dwTextColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwPushedTextColor;
		DWORD m_dwFocusedTextColor;
		DWORD m_dwDisabledTextColor;
		DWORD m_dwSelectedTextColor;

		//////////////////////////////////////////////////////////////////////////
		bool m_bGradientVertical;		//�Ƿ����򽥱�
		DWORD m_dwBackColor2;			//����ɫ���ݶ�
		DWORD m_dwBackColor3;			//����ɫ���ݶ�

		DWORD m_dwBackColor;			//����ɫ
		DWORD m_dwHotBkColor;			//����ɫ����ͣ
		DWORD m_dwFocusBkColor;			//����ɫ������
		DWORD m_dwPushedBkColor;		//����ɫ����갴��
		DWORD m_dwDisabledBkColor;		//����ɫ������ʱ
		DWORD m_dwSelectedBkColor;		//����ɫ��ѡ��ʱ

		//////////////////////////////////////////////////////////////////////////
		DWORD m_dwForeColor;			//ǰ��ɫ

		//////////////////////////////////////////////////////////////////////////
		CDuiString m_sBkImage;			//����ͼƬ

		//////////////////////////////////////////////////////////////////////////
		CDuiString m_sForeImage;		//ǰ��ͼƬ
		CDuiString m_sHotForeImage;
		CDuiString m_sPushedForeImage;
		CDuiString m_sFocusedForeImage;
		CDuiString m_sDisabledForeImage;
		CDuiString m_sSelectedForeImage;

		//////////////////////////////////////////////////////////////////////////
		//״̬ͼ
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sPushedImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;

		CDuiString  m_sSelectedImage;
		CDuiString	m_sSelectedHotImage;
		CDuiString	m_sSelectedPushedImage;
		CDuiString  m_sSelectedFocusedImage;
		CDuiString  m_sSelectedDisabledImage;

		//////////////////////////////////////////////////////////////////////////
		//�߿�
		int m_nBorderSize;				//�߿��С
		RECT m_rcBorderSize;			//�߿��С, �ֱ������������±߿��С
		SIZE m_cxyBorderRound;			//�߿�Բ��
		int m_nBorderStyle;				//�߿���ʽ
		DWORD m_dwBorderColor;			//�߿���ɫ, ��ͨ״̬
		DWORD m_dwHotBorderColor;		//�߿���ɫ, ��ͣ
		DWORD m_dwPushedBorderColor; 	//�߿���ɫ, ��갴��
		DWORD m_dwFocusBorderColor;		//�߿���ɫ, ����
		DWORD m_dwDisableBorderColor;	//�߿���ɫ, ����
		DWORD m_dwSelectedBorderColor;	//�߿���ɫ��ѡ��ʱ

		bool m_bColorHSL;				//HSL��ɫ

		WORD m_wCursor;					//�����ʽ

		RECT m_rcPaint;					//ʵ����Ҫ���Ƶ�����
	    HINSTANCE m_instance;

		CStdStringPtrMap m_mCustomAttrHash;	//�Զ��������б�

	public:
		virtual void SetPaneVisible(bool bVisible = true);
		virtual bool IsPaneVisible() const;
		virtual void SetAnimation(DuiAnim emAnim);
		virtual DuiAnim GetAnimation() const;
		virtual void SetFrameCount(int framecount);
		virtual int GetFrameCount() const;
		virtual void SetFrameDelay(int nDelay);
		virtual int GetFrameDelay() const;
		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop) override;
		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) override;
		virtual void OnAnimationStop(int nAnimationID) override;

	protected:
		DuiAnim m_animation;			//��������
		int m_nFrameCount;				//������֡��
		int m_nFrameDelay;				//����֡��ʱ
		SIZE m_szAnimationTotal;		//�ܵĶ�������
		SIZE m_szAnimationCurrect;		//��ǰ��������

	public:
		void *m_asOnInit;		//�¼�����ʱ�����ýű��ĺ���ָ��
		void *m_asOnEvent;		
		void *m_asOnNotify;	
		void *m_asOnDestroy;	
		void *m_asOnSize;
		void *m_asOnPaint;
		void *m_asOnPaintBkColor;
		void *m_asOnPaintBkImage;
		void *m_asOnPaintStatusImage;
		void *m_asOnPaintForeColor;
		void *m_asOnPaintForeImage;
		void *m_asOnPaintText;
		void *m_asOnPaintBorder;

		void __AddRef();
		void __ReleaseRef();
	private:
		int __refCount;

	public:
		void SetExtraParent(CControlUI *pControl);
		CControlUI *GetExtraParent() const;
	protected:
		CControlUI *m_pExtraParent;
	};

} // namespace DuiLib

#endif // __UICONTROL_H__
