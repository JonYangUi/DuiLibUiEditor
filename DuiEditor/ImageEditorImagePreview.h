#pragma once
#include "Tracker.h"


// CImageEditorImagePreview ��ͼ

//��ߵ�ԭͼ��ʾ�����ڵ�ͼƬ����
class CImageEditorImagePreview : public CScrollView
{
	DECLARE_DYNCREATE(CImageEditorImagePreview)

public:
	CImageEditorImagePreview();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CImageEditorImagePreview();

public:
	virtual void OnInitialUpdate();     // �����ĵ�һ��
	void InitData();
	void RecalcImageRect();
	void OnChangeRect();

	CRect m_rcImage;
	CTracker m_tracker;
	double m_zoom;

	CDuiRect m_rcSource;
protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


