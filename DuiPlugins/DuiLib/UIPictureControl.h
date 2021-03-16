#pragma once

namespace DuiLib
{

class UILIB_PLUGIN_API CPictureControlUI : public CContainerUI
{
	DECLARE_DUICONTROL(CPictureControlUI)
public:
	CPictureControlUI(void);
	virtual ~CPictureControlUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	virtual void PaintForeImage(HDC hDC);
	virtual bool Activate();
	virtual void DoEvent(TEventUI& event);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetFrameDelay(int nDelay);
	int  GetFrameDelay();

	void	SetAutoPlay(bool bIsAuto = true );
	bool	IsAutoPlay() const;
	void	SetAutoSize(bool bIsAuto = true );
	bool	IsAutoSize() const;

	void LoadFile(LPCTSTR fileName); //��Ӳ���ļ�����·������ͼƬ,��ͬ��xml��fileimage����
	void SetCxImage(const void *pCxImage);

	void StartAnim();
	void StopAnim();
	void OnTimer(UINT_PTR idEvent);

protected:
	void InitImage();
protected:
	struct Imp;
	Imp* m_pImp;

private:
	bool			m_bIsAutoPlay;				// �Ƿ��Զ�����gif
	bool			m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
	int				m_nDelay;					//ѭ��������

private:
	int m_imagetype;	//ͼƬ����
	UINT_PTR			m_idEventTimer;		//������ʱ��
	UINT				m_nFrameCount;		//gifͼƬ��֡��
	UINT				m_nFramePosition;	//��ǰ�ŵ��ڼ�֡
};

}