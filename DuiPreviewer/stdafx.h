// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#define VC_EXTRALEAN
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>
// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
#include <atlstr.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
//////////////////////////////////////////////////////////////////////////
#include "../DuiLib/Duilib.h"
#include "../DuiPlugins/Include/DuiPlugins.h"
#include "../DuiScript/Include/DuiScriptInterface.h"
//#include "E:/MyLibrary/DuiLib/3rd/CxImage/Include/image.h"
using namespace DuiLib;

#define PUGIXML_HEADER_ONLY
#ifdef _UNICODE
#define PUGIXML_WCHAR_MODE
#endif
#include "pugixml/pugixml.hpp"
using namespace pugi;


#include "LsStringConverter.h"

extern CString g_strSkinPath;
extern CString g_strSkinFile;
extern HANDLE g_hOutput;
inline void DuiWriteConsole(LPCTSTR szText)
{
	if(g_hOutput)
	{
		DWORD w;
		WriteFile(g_hOutput, szText, _tcslen(szText) * sizeof(TCHAR), &w, NULL);
	}
	else
	{
		_tprintf(szText);
		_tprintf(_T("\n"));
	}
}