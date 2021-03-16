#pragma once

//namespace DuiLib{
using namespace DuiLib;

#define TMD_ADD		1
#define TMD_MODIFY	2
#define TMD_DELETE	3

class UILIB_API CUIDialog : public CUIFrameWnd
{
public:
	CUIDialog(void);
	virtual ~CUIDialog(void);
	int m_nMode;

	virtual void OnFinalMessage( HWND hWnd );
/*
	UINT DoModal();
	void ShowDialog();

	UINT DoModal(CUIFrameWnd *pParentWnd);
	void ShowDialog(CUIFrameWnd *pParentWnd);

	UINT DoModal(CUIForm *pParentWnd);
	void ShowDialog(CUIForm *pParentWnd);
	*/
	UINT DoModal(CUIFrmBase *pParentWnd = NULL);
	void ShowDialog(CUIFrmBase *pParentWnd = NULL); //ע���ģ̬���ڱ�����new�����ġ�

	virtual void Notify(TNotifyUI& msg);

	virtual void InitWindow();

	virtual void OnClickOK();
	virtual void OnClickCancel();

	virtual void on_tmd_add_initwindow();
	virtual bool on_tmd_add();

	virtual void on_tmd_modify_initwindow();
	virtual bool on_tmd_modify();

	virtual void on_tmd_delete_initwindow();
	virtual bool on_tmd_delete();

private:
	bool _bModal;
};

//}