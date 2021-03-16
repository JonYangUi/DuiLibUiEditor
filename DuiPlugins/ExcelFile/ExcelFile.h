#pragma once

#ifndef XF_EXCEL_FILE_CTRL
#define XF_EXCEL_FILE_CTRL __declspec(dllimport)
#endif

#include "ExcelApplication.h"
#include "ExcelBook.h"
#include "ExcelBooks.h"
#include "ExcelSheet.h"
#include "ExcelSheets.h"
#include "ExcelRange.h"

#include "ExcelRangeEx.h"

class CExcelFile
{
public:
	CExcelFile(void);
	virtual ~CExcelFile(void);

	BOOL InitExcel();
	void UnInitExcel();

	//��excel�ļ�, 
	BOOL OpenFile(LPCTSTR fileName);

	//����excel�ļ�
	BOOL CreateFile();

	BOOL Save();
	BOOL SaveAs(LPCTSTR fileName);

	//���sheet����
	int GetSheetCount();

	//����sheet, ��ȡsheet���е�Ԫ��
	BOOL LoadSheet(int iIndex);

	//��ȡsheet����
	int GetColumnsCount();

	//��ȡsheet����
	int GetRowsCount();

	BOOL SetCell(int iRow, int iColumn, const CString &ret);
	BOOL SetCell(int iRow, int iColumn, const int &ret);
	BOOL SetCell(int iRow, int iColumn, const COleDateTime &dt);
	BOOL SetCell(int iRow, int iColumn, const COleCurrency &cy);

	BOOL GetCell(int iRow, int iColumn, CString &ret);
	BOOL GetCell(int iRow, int iColumn, int &ret);
	BOOL GetCell(int iRow, int iColumn, COleDateTime &dt);
	BOOL GetCell(int iRow, int iColumn, COleCurrency &cy);

	void ShowInExcel(bool bShow);

	void SetRowHeight(int row, int nHeight);
	void SetColumnWidth(int col, int nWidth);
public:
	CExcelApplication _app;
	CExcelBook	_book;
	CExcelBooks  _books;
	CExcelSheet	_sheet;
	CExcelSheets _sheets;
	CExcelRange _range;
private:  
	CString m_strPath;
};