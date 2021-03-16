#ifndef __UILABELLAYOUT_H__
#define __UILABELLAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CLabelLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CLabelLayoutUI)
	public:
		CLabelLayoutUI();
		~CLabelLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetTextStyle(UINT uStyle);
		UINT GetTextStyle() const;
		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetDisabledTextColor(DWORD dwTextColor);
		DWORD GetDisabledTextColor() const;
		void SetFont(int index);
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		bool IsShowHtml();
		void SetShowHtml(bool bShowHtml = true);

		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);

		virtual bool GetAutoCalcWidth() const;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth);
		virtual void SetText(LPCTSTR pstrText);
		virtual void SetText(int n);		//add by liqs99
		
	protected:
		DWORD	m_dwTextColor;
		DWORD	m_dwDisabledTextColor;
		int		m_iFont;
		UINT	m_uTextStyle;
		RECT	m_rcTextPadding;
		bool	m_bShowHtml;
		bool	m_bAutoCalcWidth;
	};
}

#endif // __UILABEL_H__