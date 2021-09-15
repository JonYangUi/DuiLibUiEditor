#pragma once

namespace DuiLib {

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
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	//������Ӧ�س�����ESC��Ĭ�ϴ���ʽ
	virtual void SetDefaultKeyEvent(BOOL bEnterCloseOK=TRUE, BOOL bEscCloseCancel=TRUE);
	virtual BOOL IsEnterCloseOK() const;
	virtual BOOL IsEscCloseCancel() const;

	//��ʾģ̬�Ի�����
	virtual UINT DoModal(CUIFrmBase *pParentWnd = NULL);

	//��ʾ��ģ̬�Ի�����, ע���ģ̬���ڱ�����new�����ġ�
	virtual void ShowDialog(CUIFrmBase *pParentWnd = NULL);

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
	BOOL m_bEnterCloseOK;
	BOOL m_bEscCloseCancel;
};

} //namespace DuiLib {