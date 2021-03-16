#ifndef __UITEXTSCROLLEXH__
#define __UITEXTSCROLLEXH__

#pragma once

namespace DuiLib
{
	class UILIB_API CRollTextExUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CRollTextExUI)
	public:
		CRollTextExUI(void);
		~CRollTextExUI(void);

	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

	public:	
		virtual void DoInit();
		void OnTimer(UINT_PTR idEvent);
		virtual void PaintText(HDC hDC);
		virtual void DoEvent(TEventUI& event);
		virtual void SetPos(RECT rc);
		virtual void SetText(LPCTSTR pstrText);

		void SetRollDirection(int nDirect);
		int GetRollDirection() const;

		void SetRollSpeed(int nMillisecond);
		int GetRollSpeed() const;

		void SetRollStep(int nStep);
		int GetRollStep() const;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	public:
		void BeginRoll();
		void EndRoll();

	private:
		UINT m_idEventTimer;
		int m_nStep;
		int m_nScrollPos;
		BOOL m_bUseRoll;
		int m_nRollDirection;   
		int m_nText_W_H;

		bool m_bPauseRoll;
		RECT m_rcClient;
		int m_nRollSpeed;
	};

}	// namespace DuiLib

#endif // __UITEXTSCROLLH__