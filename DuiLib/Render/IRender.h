#ifndef __IRENDER_H__
#define __IRENDER_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	typedef struct UILIB_API tagTDrawInfo
	{
		tagTDrawInfo();
		void Parse(LPCTSTR pStrImage, LPCTSTR pStrModify, CPaintManagerUI *paintManager);
		void Clear();

		CDuiString sDrawString;	//xml�ж����bkimage���Եȵȣ��磺file='images\background.png' dest='0,0,357,220' 
		CDuiString sDrawModify; //�����Ϊ�գ� sDrawString + sDrawModify Ϊ��ʶ�ؼ���

		//------���¶��Ǵ�sDrawString����������----------//

		CDuiString sImageName;	//ͼƬ���ļ�������������UIImageInfo�б����ؼ��֣��ô���ͬһ��ͼƬ���ڴ���ֻ����һ�ݡ� (svg����, ͬһ��svg��Ⱦ��ͬʱ���ڴ��б����ݡ�)
		CDuiString sResType;	//��Դ�ļ��ж�������ͣ��磺"ZIPRES", "PNG"
		RECT rcDest;			//��ʾ��ʲôλ��
		RECT rcSource;			//��Ҫ��ͼƬ���Ĳ��֣�
		RECT rcCorner;			//Բ��
		DWORD dwMask;			//λͼ��͸��ɫ
		BYTE uFade;				//͸����
		bool bHole;				//����.......rcCorner���򲻻���
		bool bTiledX;			//����ƽ��
		bool bTiledY;			//����ƽ��
		bool bHSL;				//HSLɫ��ģʽ
		RECT rcPadding;			//��߾�
		UINT uAlign;			//���뷽ʽ
		int width;				//SVG�Ŀ��
		int height;				//SVG�ĸ߶�
		DWORD fillcolor;		//SVG�������ɫ
	} TDrawInfo;

	/////////////////////////////////////////////////////////////////////////////////////
	//
	enum emRenderEngine
	{
		DuiLib_Render_Default,	//Ĭ�ϵģ�����ԭ����ģʽ��
		DuiLib_Render_GdiPlus,	//ͳһʹ��Gdiplus����
		//DuiLib_Render_Skia,	//ʹ��skiaͼ���
	};
	
	//
	enum emUIOBJTYPE
	{
		OT_NULL=0,
		OT_HDC,
		OT_FONT,
		OT_PEN,
		OT_BRUSH,
		OT_BITMAP,
		OT_IMAGE,
	};
	
	class UILIB_API UIObject : public IObjRef
	{
	public:
		virtual const	emUIOBJTYPE ObjectType() const	= 0;	//{ return OT_NULL; }
		virtual void	DeleteObject()					= 0;
	};

	class UILIB_API UIFont : public TObjRefImpl<UIObject>
	{
	protected:
		UIFont()
		{
			id			= -1;
			iSize		= 16;
			bBold		= false;
			bUnderline	= false;
			bItalic		= false;
			bDefault	= false;
			bShared		= false;
		}
		virtual ~UIFont() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_FONT;  }

		virtual HFONT GetHFont(CPaintManagerUI *pManager) = 0;

		//����Ĭ������
		BOOL CreateDefaultFont();

		//�Ӳ�����������
		BOOL CreateFont(CPaintManagerUI *pManager, int id, LPCTSTR sFontName, int iSize, 
			bool bBold, bool bUnderline, bool bItalic, bool bDefault, bool bShared);

		//��Xml�ڵ㹹������, �磺<Font id="0" name=....... />
		BOOL CreateFont(CPaintManagerUI *pManager, CXmlNodeUI node);

		//���¹�������, ��dpi�Ķ�ʱ
		BOOL RebuildFont(CPaintManagerUI *pManager);

		//��¡����
		virtual UIFont* Clone(CPaintManagerUI *pManager) = 0;

		//��ȡ����߶�
		virtual int GetHeight(CPaintManagerUI *pManager) = 0;

		virtual int GetId()					{ return id;			}
		virtual CDuiString GetFontName()	{ return sFontName;		}
		virtual int GetSize()				{ return iSize;			}
		virtual bool IsBold()				{ return bBold;			}
		virtual bool IsUnderline()			{ return bUnderline;	}
		virtual bool IsItalic()				{ return bItalic;		}
		virtual bool IsDefault()			{ return bDefault;		}
		virtual bool IsShared()				{ return bShared;		}

	protected:
		virtual BOOL _buildFont(CPaintManagerUI *pManager=NULL) = 0;
	protected:
		int id;
		CDuiString sFontName;
		int iSize;
		bool bBold;
		bool bUnderline;
		bool bItalic;
		bool bDefault;
		bool bShared;
	};

	class UILIB_API UIPen  : public TObjRefImpl<UIObject>
	{
	protected:
		UIPen();
		virtual ~UIPen() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_PEN;  }
		virtual HPEN GetHPen() const = 0;

		virtual BOOL CreatePen(int nStyle, int nWidth, DWORD dwColor) = 0;

		bool operator == (UIPen &obj) const;

		int GetStyle() const		{ return m_nStyle;	}
		int GetWidth() const		{ return m_nWidth;	}
		DWORD GetColor() const		{ return m_dwColor; }
	protected:
		int m_nStyle;
		int m_nWidth;
		DWORD m_dwColor;
	};

	class UILIB_API UIBitmap  : public TObjRefImpl<UIObject>
	{
	protected:
		virtual ~UIBitmap() {}

	public:	
		const emUIOBJTYPE ObjectType() const override {  return OT_BITMAP;  }

		//hBitmap���ڲ��ͷ�
		virtual BOOL CreateFromHBitmap(HBITMAP hBitmap) = 0;

		virtual BOOL CreateARGB32Bitmap(HDC hDC, int width, int height) = 0;

		virtual BOOL CreateCompatibleBitmap(HDC hDC, int width, int height) = 0;

		virtual HBITMAP GetBitmap()		= 0;
		virtual BYTE* GetBits()			= 0;
		virtual int	GetWidth()			= 0;
		virtual int GetHeight()			= 0;

		virtual void Clear() = 0;

		virtual void ClearAlpha(const RECT &rc, int alpha = 0) = 0;

		virtual BOOL SaveFile(LPCTSTR pstrFileName) = 0;
	};

	class UILIB_API UIImage  : public TObjRefImpl<UIObject>
	{
	protected:
		virtual ~UIImage() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_IMAGE;  }

		virtual BOOL CreateImage(HBITMAP hBitmap, bool bAlpha) = 0;

		virtual BOOL LoadImage(const TDrawInfo *pDrawInfo, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL) = 0;
		virtual BOOL LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL) = 0;
		virtual BOOL LoadImage(LPCTSTR pStrImage, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL) = 0;
		virtual BOOL LoadImage(UINT nID, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL) = 0;
		
		//���ڴ�������ͼ��֧��ͼ���ʽ��bmp, jpg, png, svg
		virtual BOOL LoadImageFromMemory(const LPBYTE pData, DWORD dwSize, DWORD mask=0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL) = 0;

		//ͼ��HSLת��
		virtual void AdjustHslImage(bool bUseHSL, short H, short S, short L) = 0;

		//���ļ�������gifͼ��, arrImageInfo����ÿ֡ͼ�� ʹ����ǵ��ͷ�arrImageInfo
		virtual bool LoadGifImageFromFile(LPCTSTR fileName, CStdPtrArray &arrImageInfo) = 0;

		//���ڴ�������gifͼ��, arrImageInfo����ÿ֡ͼ�� ʹ����ǵ��ͷ�arrImageInfo
		virtual bool LoadGifImageFromMemory(const LPBYTE pData, DWORD dwSize, CStdPtrArray &arrImageInfo) = 0;

		int GetWidth() const { return nWidth; }
		int GetHeight() const { return nHeight; }
	public:
		UIBitmap *bitmap;		//ͼ������
		LPBYTE pBits;			//ת��HSLʱ���õ���
		LPBYTE pSrcBits;		//ת��HSLʱ���õ��ģ�����ת��֮ǰ��pBits
		int nWidth;				//���
		int nHeight;			//�߶�
		bool bAlpha;			//͸��
		bool bUseHSL;			//HSLģʽ
		CDuiString sResType;	//��Դ�ļ��ж�������ͣ��磺"ZIPRES", "PNG"
		DWORD dwMask;			//͸��ɫ
		int delay;				//gif����ÿ֡��ʱ
	};

	class UILIB_API UIBrush  : public TObjRefImpl<UIObject>
	{
	protected:
		virtual ~UIBrush() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_BRUSH;  }
		virtual HBRUSH GetHBrush() const = 0;

		//hBrush���ڲ��ͷ�
		virtual BOOL CreateFromHBrush(HBRUSH hBrush) = 0;

		//��ָ����ʵ�߳�ʼ����ˢ
		virtual BOOL CreateSolidBrush(DWORD clr) = 0;

		//��λͼ���컭ˢ
		virtual BOOL CreateBitmapBrush(UIBitmap *bitmap) = 0;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIRender : public IObjRef
	{
	protected:
		virtual ~UIRender() {}

	public:
		virtual void Init(CPaintManagerUI *pManager, HDC hDC = NULL) = 0;
		virtual void AttachDC(CPaintManagerUI *pManager, HDC hDC) = 0;

		virtual CPaintManagerUI *GetManager()	= 0;
		virtual HDC GetDC() = 0;
		virtual UIBitmap *GetBitmap() = 0;

		virtual bool Resize(int width, int height) = 0;
		virtual bool Resize(const RECT &rc) = 0;

		//������ǰ������
		virtual void Clear() = 0;

		virtual void SaveDC() = 0;
		virtual void RestoreDC() = 0;

		virtual UIObject* SelectObject(UIObject *pObject) = 0;
		virtual void RestoreObject(UIObject *pObject = NULL) = 0;
		virtual void RestoreDefaultObject() = 0;

		//BitBlt (bit block transfer)
		virtual BOOL BitBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, DWORD dwRop = SRCCOPY ) = 0;
		
		//stretch BitBlt
		virtual BOOL StretchBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop = SRCCOPY) = 0;

		//����λͼ
		virtual void DrawBitmap(HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, BYTE uFade = 255, bool hole = false, bool xtiled = false, bool ytiled = false) = 0;

		//����ɫ(�����ɫ)
		virtual void DrawColor(const RECT& rc, const SIZE &round, DWORD color) = 0;

		//������ɫ
		virtual void DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps) = 0;

		//����  MoveTo(rc.left, rc.top)��LineTo(rc.right, rc.bottom)
		virtual void DrawLine(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) = 0;

		//���߿�
		virtual void DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) = 0;

		//��Բ�Ǳ߿�
		virtual void DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle = PS_SOLID) = 0;

		//����Բ
		virtual void DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID) = 0;

		//�����ı�
		virtual void DrawText(RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle) = 0;

		//�������ֵ�ռ�õĿռ��С
		virtual SIZE GetTextSize(LPCTSTR pstrText, int iFont, UINT uStyle) = 0;

		//////////////////////////////////////////////////////////////////////////

		//������ɫ
		void DrawBackColor(const RECT& rc, const SIZE &round, DWORD dwBackColor, DWORD dwBackColor2=0, DWORD dwBackColor3=0, bool bVertical=true);

		//���߿�
		void DrawBorder(const RECT &rcItem, int nBorderSize, SIZE szBorderRound, RECT rcBorderSize, DWORD dwColor, int nBorderStyle);

		//�����ı�
		void DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle);

		//���ƴ�������ɫ���ı�
		void DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText,DWORD dwTextColor, int iFont, UINT uStyle, DWORD dwTextBKColor);

		//����TDrawInfo����ͼ��
		bool DrawImageInfo(const RECT& rcItem, const RECT& rcPaint, const TDrawInfo* pDrawInfo, HINSTANCE instance = NULL);

		//�����ַ������Ի���ͼ��
		bool DrawImageString(const RECT& rcItem, const RECT& rcPaint, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL, HINSTANCE instance = NULL);

	protected:
		CPaintManagerUI *m_pManager;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIClip
	{
	public:
		UIClip();
		virtual ~UIClip();
	public:
		void GenerateClip(UIRender *pRender, RECT rc);
		void GenerateRoundClip(UIRender *pRender, RECT rc, RECT rcItem, int roundX, int roundY);
		void UseOldClipBegin(UIRender *pRender);
		void UseOldClipEnd(UIRender *pRender);

	protected:
		RECT m_rcItem;
		SIZE m_szRound;
		HDC m_hDC;
		HRGN m_hRgn;
		HRGN m_hOldRgn;
	};

	class UILIB_API UIRenderFactory : public IObjRef
	{
	protected:
		virtual ~UIRenderFactory() {}
	public:
		virtual UIRender *CreateRenderTarget()		= 0;

		virtual UIFont *CreateFont()				= 0;
		virtual UIPen *CreatePen()					= 0;
		virtual UIBitmap *CreateBitmap()			= 0;
		virtual UIBrush *CreateBrush()				= 0;
		virtual UIImage *CreateImage()				= 0;
	};

} // namespace DuiLib

#endif // __UIRENDER_H__
