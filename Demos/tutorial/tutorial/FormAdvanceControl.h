#pragma once

//��Ϣ��������
class CFormAdvanceControl : public CUIForm
{
public:
	CFormAdvanceControl(void);
	virtual ~CFormAdvanceControl(void);

	virtual void InitWindow();
	virtual void OnNotifyWindowPrepare(TNotifyUI& msg);


	CGridCtrlUI *m_pGrid;
};

