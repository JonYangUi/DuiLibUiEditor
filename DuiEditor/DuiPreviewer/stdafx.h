// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
/*
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
*/
#pragma warning(disable:4996)

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
//////////////////////////////////////////////////////////////////////////
/*
	ʹ�ùٷ�����߸��ݹٷ�������ͬѧ���������꣨UILIB_VERSION_ORIGINAL�����б���
	���������ֻ��Ϊ��ʹ�ùٷ����ͬѧ�ܹ�����ɹ�������ϸ������Ҫ���Լ�Ŭ���ˡ�
	һ��Ҫά�����Լ��Ŀؼ������ļ��� DuiLib.xml
	�벻Ҫֱ��ʹ������Ŀ���ṩ��duilibԭ��⣬Ӧ��ʹ�����Լ��Ŀ���б��롣
*/
//#define DUILIB_VERSION_ORIGINAL

#ifdef DUILIB_VERSION_ORIGINAL
	#include "../DuiLibOriginal/DuiLib/UIlib.h"
	#ifdef _DEBUG
		#pragma comment (lib, "../DuiLibOriginal/Lib/DuiLib_ud.lib")
	#else
		#pragma comment (lib, "../DuiLibOriginal/Lib/DuiLib_u.lib")
	#endif
	using namespace DuiLib;
#else
	#include "../../DuiLib/DuiLib.h"
	using namespace DuiLib;
#endif

#ifdef _UNICODE
#define PUGIXML_WCHAR_MODE
#endif
#include "../pugixml/pugixml.hpp"
	using namespace pugi;