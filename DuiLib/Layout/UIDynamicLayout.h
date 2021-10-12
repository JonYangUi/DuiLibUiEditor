#ifndef __UIDYNAMICLAYOUT_H__
#define __UIDYNAMICLAYOUT_H__

#pragma once

namespace DuiLib
{
	enum emLayoutType
	{
		Layout_HorizontalLayout = 0,
		Layout_VerticalLayout,
		Layout_Tile
	};

	//����̬���ò��ַ�ʽ����
	class UILIB_API CDynamicLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CDynamicLayoutUI)
	public:
		enum emSepAction
		{
			eSepNull = 0,
			eSepWidth,		//�϶����
			eSepHeight,		//�϶��߶�
			eSepWidthHeight	//�϶��߽�
		};
		CDynamicLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetLayout(emLayoutType eMode);
		emLayoutType GetLayout() const;

		void SetSepWidth(int iWidth);
		int GetSepWidth() const;
		void SetSepHeight(int iHeight);
		int GetSepHeight() const;
		void SetSepImmMode(bool bImmediately);
		bool IsSepImmMode() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void DoEvent(TEventUI& event);

		virtual SIZE EstimateSize(SIZE szAvailable);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void SetPosHorizontalLayout(RECT rc, bool bNeedInvalidate = true);
		void SetPosVerticalLayout(RECT rc, bool bNeedInvalidate = true);
		virtual bool CalcPos(CControlUI *pChildControl, RECT &rcChild); //�ӿؼ�����ѯ�ʸ��ؼ����㽫����ҷ������rect��
		void DoPostPaint(HDC hDC, const RECT& rcPaint);

		RECT GetThumbRect(bool bUseNew = false) const;

	protected:
		int m_iSepWidth;
		int m_iSepHeight;
		POINT m_ptLastMouse;
		RECT m_rcNewPos;
		bool m_bImmMode;

		emLayoutType m_eLayout; 
		emSepAction m_eSepAction;

	private:
		CControlUI *m_pCalcControl;
		RECT m_rcCalcChild;
		bool m_bCalcResult;
	};
}
#endif // __UIHORIZONTALLAYOUT_H__
