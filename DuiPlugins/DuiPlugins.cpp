// DuiPlugins.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DuiPlugins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CDuiPluginsApp

BEGIN_MESSAGE_MAP(CDuiPluginsApp, CWinApp)
END_MESSAGE_MAP()


// CDuiPluginsApp ����

CDuiPluginsApp::CDuiPluginsApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDuiPluginsApp ����

CDuiPluginsApp theApp;


// CDuiPluginsApp ��ʼ��

BOOL CDuiPluginsApp::InitInstance()
{
	CWinApp::InitInstance();

	CString temp;
	TCHAR czProgName[MAX_PATH];
	int  iNiValue = GetModuleFileName(m_hInstance, czProgName, MAX_PATH);
	if(iNiValue > 0)
	{
		temp = czProgName;
		g_strDuiPluginsPath = temp.Left(temp.ReverseFind('\\') + 1);
	}
	return TRUE;
}

#include "Include/DuiPlugins.h"


UILIB_PLUGIN_API void DuiPluginsRegister()
{
	REGIST_DUICONTROL(CGridCtrlUI);
//	REGIST_DUICONTROL(CMsgWndUI);
	REGIST_DUICONTROL(CKeyBoardUI);
	REGIST_DUICONTROL(CKeyBoardNumberUI);
	REGIST_DUICONTROL(CKeyBoardSimpleUI);
	REGIST_DUICONTROL(CImageBoxUI);
	REGIST_DUICONTROL(CPictureControlUI);
// 	REGIST_DUICONTROL(CComboExUI);
// 	REGIST_DUICONTROL(CIconButtonUI);
// 	REGIST_DUICONTROL(CDateTimeExUI);
// 	REGIST_DUICONTROL(CAccordionPaneUI);
// 	REGIST_DUICONTROL(CImageBoxExUI);
// 	REGIST_DUICONTROL(CRollTextExUI);
	REGIST_DUICONTROL(CSciEditUI);
	REGIST_DUICONTROL(CGridListUI);
	REGIST_DUICONTROL(CGridListHeaderUI);
	REGIST_DUICONTROL(CGridListBodyUI);
	REGIST_DUICONTROL(CGridListRowUI);
	REGIST_DUICONTROL(CGridListCellUI);
}


extern "C" UILIB_PLUGIN_API CControlUI *CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("GridCtrl")) == 0 ) 
	{
		return new CGridCtrlUI;
	}
// 	else if( _tcscmp(pstrClass, _T("MsgWnd")) == 0 ) 
// 	{
// 		return new CMsgWndUI;
// 	}
	else if( _tcscmp(pstrClass, _T("KeyBoard")) == 0 ) 
	{
		return new CKeyBoardUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardNumber")) == 0 ) 
	{
		return new CKeyBoardNumberUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardSimple")) == 0 ) 
	{
		return new CKeyBoardSimpleUI;
	}
	else if( _tcscmp(pstrClass, _T("ImageBox")) == 0 ) 
	{
		return new CImageBoxUI;
	}
	else if( _tcscmp(pstrClass, _T("PictureControl")) == 0 ) 
	{
		return new CPictureControlUI;
	}
	else if( _tcscmp(pstrClass, _T("ComboEx")) == 0 ) 
	{
		return new CComboExUI;
	}
// 	else if( _tcscmp(pstrClass, _T("IconButton")) == 0 ) 
// 	{
// 		return new CIconButtonUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("DateTimeEx")) == 0 ) 
// 	{
// 		return new CDateTimeExUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("AccordionPane")) == 0 ) 
// 	{
// 		return new CAccordionPaneUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("ImageBoxEx")) == 0 ) 
// 	{
// 		return new CImageBoxExUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("RollTextEx")) == 0 ) 
// 	{
// 		return new CRollTextExUI;
// 	}
	else if( _tcscmp(pstrClass, _T("SciEdit")) == 0 ) 
	{
		return new CSciEditUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLIST) == 0 ) 
	{
		return new CGridListUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTHEADER) == 0 ) 
	{
		return new CGridListHeaderUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTBODY) == 0 ) 
	{
		return new CGridListBodyUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTROW) == 0 ) 
	{
		return new CGridListRowUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTCELL) == 0 ) 
	{
		return new CGridListCellUI;
	}
	return NULL;
}

UILIB_PLUGIN_API void InsertMsgUI(LPCTSTR pstring, COLORREF cr)
{
	CMsgWndUI::InsertMsg(pstring, cr);
}

UILIB_PLUGIN_API void InsertMsgUiV(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	CString strText;

	va_list argList;
	va_start(argList, lpszFormat);
	strText.FormatV(lpszFormat, argList);
	va_end(argList);

	CMsgWndUI::InsertMsg(strText, RGB(0,0,0));
}