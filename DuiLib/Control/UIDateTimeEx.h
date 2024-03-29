#ifndef __UIDateTimeEx_H__
#define __UIDateTimeEx_H__

#pragma once

#define UIDTS_DATE		0
#define UIDTS_TIME		1
#define UIDTS_DATETIME	2


namespace DuiLib
{
	//class CDateTimeExWnd;

	/// ʱ��ѡ��ؼ�
	class UILIB_API CDateTimeExUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CDateTimeExUI)
	public:
		CDateTimeExUI();
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void DoInit();

		virtual bool OnEventLabelDate(void* param);
		virtual bool OnEventLabelTime(void* param);

		SYSTEMTIME& GetTime();
		void SetTime(SYSTEMTIME* pst);

		void SetFormatStyle(UINT uStyle);	//UIDTS_DATE=date, UIDTS_TIME=time, UIDTS_DateTimeEx=DateTimeEx  add by liqs99
		UINT GetFormatStyle();				//UIDTS_DATE=date, UIDTS_TIME=time, UIDTS_DateTimeEx=DateTimeEx  add by liqs99

		virtual void SetText(LPCTSTR pstrText);
		virtual CDuiString GetText() const;

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		void DoEvent(TEventUI& event);

		CLabelUI *GetDateLabel() const;
		CLabelUI *GetTimeLabel() const;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	protected:
		SYSTEMTIME m_sysTime;
		int        m_uFormatStyle; //UIDTS_DATE=date, UIDTS_TIME=time, UIDTS_DateTimeEx=DateTimeEx  add by liqs99
		
		CLabelUI *m_pLabelDate;
		CLabelUI *m_pLabelTime;
// 		CDateTimeExWnd* m_pWindowDate;
// 		CDateTimeExWnd* m_pWindowTime;
	public:
		CWindowWnd* m_pWindowDate;
		CWindowWnd* m_pWindowTime;
	};
}
#endif // __UIDateTimeEx_H__