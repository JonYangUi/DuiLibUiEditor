#pragma once



// CImageEditorView ��ͼ

class CImageEditorView : public CScrollView
{
	DECLARE_DYNCREATE(CImageEditorView)

public:
	CImageEditorView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CImageEditorView();

public:
	void InitData();
	void RecalcImageRect();
	void OnChangeRect();

	CPaintManagerUI *m_pManager;
	TDrawInfo m_drawInfo;

	CRect m_rcControl;
	double m_zoom;

	CRectTracker m_tracker;
protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
};


