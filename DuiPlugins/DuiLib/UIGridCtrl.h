#pragma once

namespace DuiLib
{

class UILIB_PLUGIN_API CGridCtrlUI : public CContainerUI
{
	DECLARE_DUICONTROL(CGridCtrlUI)
public:
	CGridCtrlUI(void);
	virtual ~CGridCtrlUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	CGridCtrl *GetGrid();
	CGridCell *GetCell(int nRow, int nCol);

	CGridCtrl &Grid();
	CGridCell &Cell(int nRow, int nCol);				//������ͨ��Ԫ������
	CGridCellCombo &ComboBox(int nRow, int nCol);		//����ComboBox��Ԫ������	
	CGridCellCheck &CheckBox(int nRow, int nCol);		//����CheckBox��Ԫ������	
	CGridCellDateTime &DateTime(int nRow, int nCol);	//����DateTime��Ԫ������
	
	CGridDefaultCell &CellDef();			//����Ĭ�ϵ�Ԫ������
	CGridDefaultCell &CellFixedRowDef();	//����Ĭ�ϵ�Ԫ������
	CGridDefaultCell &CellFixedColDef();	//����Ĭ�ϵ�Ԫ������
	CGridDefaultCell &CellFixedRowColDef();	//����Ĭ�ϵ�Ԫ������
	

	virtual void DoInit();
	virtual void DoEvent(TEventUI& event);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void SetPos(RECT rc, bool bNeedInvalidate  = true);
	void SetInternVisible(bool bVisible);

	void OnGridNotify(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	//////////////////////////////////////////////////////////////////////////
	//����
	//////////////////////////////////////////////////////////////////////////
	void SetRowHeight(int nHeight);
	int GetRowHeight();

	bool SetRowCount(int count);
	int GetRowCount();

	bool SetColumnCount(int count);
	int GetColumnCount();

	bool SetFixedRowCount(int count);
	int GetFixedRowCount();

	bool SetFixedColumnCount(int count);
	int GetFixedColumnCount();


	void SetEditable(bool bEditable);
	bool GetEditable();

	void SetSingleRowSelection(bool bSingleRow);
	bool GetSingleRowSelection();

	void SetListMode(bool bListMode);
	bool GetListMode();

	void SetViewListNumber(bool bView);
	bool GetViewListNumber();

	void SetFont(int index);
	int GetFont() const;

	void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;

	void SetVirtualGrid(bool bVirtual);
	bool GetVirtualGrid();

	void  SetGridBkColor(DWORD dwColor);
	DWORD GetGridBkColor();

	void  SetGridLineColor(DWORD dwColor);
	DWORD GetGridLineColor();

	void SetEnableHighLightBkColor(bool bValue);
	bool GetEnableHighLightBkColor();

	void SetHighLightBkColor(DWORD dwColor);
	DWORD GetHighLightBkColor();

	void SetEnableHighLightTextClr(bool bValue);
	bool GetEnableHighLightTextClr();

	void SetHighLightTextClr(DWORD dwColor);
	DWORD GetHighLightTextClr();

	void SetFocusCellBorder(bool bBorder);
	bool GetFocusCellBorder();

	void SetFocusCellBorderColor(DWORD dwColor);
	DWORD GetFocusCellBorderColor();

//////////////////////////////////////////////////////////////////////////
//����
//////////////////////////////////////////////////////////////////////////
public:
	void Refresh();
	void SetAutoRedraw(bool bAuto);
	void ExpandColumnsToFit(BOOL bExpandFixed = TRUE);
	int  InsertRow(int rowPosition=-1);
	void SetHeaderTexts(LPCTSTR pstrValue);

public:
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

public:
	CWindowWnd *m_pWindow;


private:
	bool m_bVirtualGrid;

	int m_nRowCount;
	int m_nColumnCount;
	int m_nFixedRowCount;
	int m_nFixedColumnCount;

	int m_nDefaultRowHeight;

	bool m_bEditable;
	bool m_bSingleRowSelection;
	bool m_bListMode;
	bool m_bViewListNo;

	DWORD	m_dwTextColor;
	int		m_iFont;

	DWORD m_dwGridBkColor;
	DWORD m_dwGridLineColor;


	DWORD m_crHighLightBkColor;
	bool m_bHighLightBkColor;
	DWORD m_crHighLightTextClr;
	bool m_bHighLightTextClr;

	bool m_bFocusCellBorder;
	DWORD m_dwFocusCellBorderColor;

	CStdPtrArray m_arrHeaderText;
};

}