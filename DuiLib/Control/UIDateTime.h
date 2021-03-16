#ifndef __UIDATETIME_H__
#define __UIDATETIME_H__

#pragma once

namespace DuiLib
{
	class CDateTimeWnd;

	/// ʱ��ѡ��ؼ�
	class UILIB_API CDateTimeUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CDateTimeUI)
		friend class CDateTimeWnd;
	public:
		CDateTimeUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		SYSTEMTIME& GetTime();
		void SetTime(SYSTEMTIME* pst);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void SetFormatStyle(UINT uStyle);
		UINT GetFormatStyle();

		virtual void SetText(LPCTSTR pstrText);
		virtual CDuiString GetText() const;

		virtual void SetText(int n);

		void DoEvent(TEventUI& event);

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		SYSTEMTIME m_sysTime;
		bool       m_bReadOnly;
		int        m_uFormatStyle; //0=date, 1=time, 2=datetime  add by liqs99

		CDateTimeWnd* m_pWindow;
	};
}
#endif // __UIDATETIME_H__