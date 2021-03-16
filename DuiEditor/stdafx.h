
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once
#pragma warning(disable:4996)

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef _UNICODE
//#define PUGIXML_WCHAR_MODE
#endif
 
#ifdef PUGIXML_WCHAR_MODE
	#define XTEXT(x) L ## x
	#define XString	CString
#else
	#define XTEXT(x) x
	#define XString CStringA
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*
	ʹ�ùٷ�����߸��ݹٷ�������ͬѧ���������꣨UILIB_VERSION_ORIGINAL�����б���
	���������ֻ��Ϊ��ʹ�ùٷ����ͬѧ�ܹ�����ɹ�������ϸ������Ҫ���Լ�Ŭ���ˡ�
	һ��Ҫά�����Լ��Ŀؼ������ļ��� DuiLib.xml
	�벻Ҫֱ��ʹ������Ŀ���ṩ��duilibԭ��⣬Ӧ��ʹ�����Լ��Ŀ���б��롣
*/
//#define DUILIB_VERSION_ORIGINAL

#ifdef DUILIB_VERSION_ORIGINAL
	#include "DuiLibOriginal/DuiLib/UIlib.h"
	#ifdef _DEBUG
		#pragma comment (lib, "DuiLibOriginal/Lib/DuiLib_ud.lib")
	#else
		#pragma comment (lib, "DuiLibOriginal/Lib/DuiLib_u.lib")
	#endif
	using namespace DuiLib;
#else
	#include "../DuiLib/DuiLib.h"
	using namespace DuiLib;
	#include "../DuiPlugins/Include/DuiPlugins.h"
#endif

#include "../3rd/CxImage/Include/image.h"
//////////////////////////////////////////////////////////////////////////
#define INCLUDE_DEPRECATED_FEATURES
#include "../3rd/scintilla/include/SciLexer.h"
#include "../3rd/scintilla/include/Scintilla.h"
#include "../3rd/scintilla/SciWnd.h"

#include "pugixml/pugiconfig.hpp"
#include "pugixml/pugixml.hpp"
using namespace pugi;
#define XML_PARSER_OPTIONS pugi::parse_full//parse_default|parse_declaration|parse_comments

#include "../DuiScript/Include/DuiScriptInterface.h"
//////////////////////////////////////////////////////////////////////////
#include "MemDCEx.h"
#include "LsStringConverter.h"

#define RULEBAR_SIZE_X	20
#define RULEBAR_SIZE_Y	20

#define WM_CREATE_UIWINDOW		WM_USER + 1

class CLockWindowUpdate
{
public:
	CLockWindowUpdate(CWnd *pWnd) 
	{ 
		m_pWnd = pWnd;
		//m_pWnd->LockWindowUpdate();
		SetWindowRedraw(m_pWnd->GetSafeHwnd(), FALSE);
	}
	~CLockWindowUpdate() 
	{ 
		//m_pWnd->UnlockWindowUpdate(); 
		SetWindowRedraw(m_pWnd->GetSafeHwnd(), TRUE);
		::InvalidateRect(m_pWnd->GetSafeHwnd(), NULL, TRUE);
	}
private:
	CWnd *m_pWnd;
};


#include "EditorConfig.h"
extern tagEditorConfig g_cfg;

//////////////////////////////////////////////////////////////////////////

