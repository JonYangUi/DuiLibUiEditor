#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

namespace DuiLib
{
	class UILIB_API CGifAnimUI : public CControlUI
	{
		enum
		{ 
			EVENT_TIEM_ID = 100,
		};
		DECLARE_DUICONTROL(CGifAnimUI)
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID	GetInterface(LPCTSTR pstrName) override;
		virtual void	DoInit() override;
		virtual bool	DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;
		virtual void	DoEvent(TEventUI& event) override;
		virtual void	SetVisible(bool bVisible = true ) override;
		virtual void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		virtual void	SetBkImage(LPCTSTR pStrImage) override;

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	PlayGif();
		void	PauseGif();
		void	StopGif();

	private:
		void	InitGifImage();
		void	DeleteGif();
		void    OnTimer( UINT_PTR idEvent );
		void	DrawFrame( UIRender *pRender );		// ����GIFÿ֡

		Gdiplus::Image*	GdiplusLoadImage(LPCTSTR pstrPath);
	private:
		Gdiplus::Image	*m_pGifImage;
		UINT			m_nFrameCount;				// gifͼƬ��֡��
		UINT			m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡
		Gdiplus::PropertyItem*	m_pPropertyItem;	// ֡��֮֡����ʱ��

		bool			m_bIsAutoPlay;				// �Ƿ��Զ�����gif
		bool			m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
		bool			m_bIsPlaying;
	};
}

#endif // GifAnimUI_h__
