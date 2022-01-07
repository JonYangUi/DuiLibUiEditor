#ifndef UI_PALLET_H
#define UI_PALLET_H
#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API CColorPaletteUI : public CControlUI
	{
		DECLARE_DUICONTROL(CColorPaletteUI)
	public:
		CColorPaletteUI();
		virtual ~CColorPaletteUI();

		//��ȡ���ձ�ѡ�����ɫ������ֱ����������duilib����ɫ
		DWORD GetSelectColor();
		void SetSelectColor(DWORD dwColor);

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		//����/��ȡ Pallet����ɫ�������棩�ĸ߶�
		void SetPalletHeight(int nHeight);
		int	GetPalletHeight() const;

		//����/��ȡ �·�Bar������ѡ�������ĸ߶�
		void SetBarHeight(int nHeight);
		int GetBarHeight() const;

		//����/��ȡ ѡ��ͼ���·��
		void SetThumbImage(LPCTSTR pszImage);
		LPCTSTR GetThumbImage() const;

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void DoInit() override;
		virtual void DoEvent(TEventUI& event) override;
		virtual void PaintBkColor(UIRender *pRender) override;
		void PaintPallet(UIRender *pRender);

	protected:
		//��������
		void UpdatePalletData();
		void UpdateBarData();

	private:
		HDC			m_MemDc;
		HBITMAP		m_hMemBitmap;
		BITMAP		m_bmInfo;
		BYTE		*m_pBits;
		bool		m_bIsInBar;
		bool		m_bIsInPallet;
		int			m_nCurH;
		int			m_nCurS;
		int			m_nCurB;

		int			m_nPalletHeight;
		int			m_nBarHeight;
		CDuiPoint		m_ptLastPalletMouse;
		CDuiPoint		m_ptLastBarMouse;
		CDuiString  m_strThumbImage;
	};
}

#endif // UI_PALLET_H