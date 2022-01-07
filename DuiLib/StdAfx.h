// StdAfx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E30B2003_188B_4EB4_AB99_3F3734D6CE6C__INCLUDED_)
#define AFX_STDAFX_H__E30B2003_188B_4EB4_AB99_3F3734D6CE6C__INCLUDED_

#pragma once

#ifdef __GNUC__
// ��ô��û�ҵ�min��max��ͷ�ļ�-_-
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#endif

#ifndef __FILET__
#define __DUILIB_STR2WSTR(str)	L##str
#define _DUILIB_STR2WSTR(str)	__DUILIB_STR2WSTR(str)
#ifdef _UNICODE
#define __FILET__	_DUILIB_STR2WSTR(__FILE__)
#define __FUNCTIONT__	_DUILIB_STR2WSTR(__FUNCTION__)
#else
#define __FILET__	__FILE__
#define __FUNCTIONT__	__FUNCTION__
#endif
#endif

#define _CRT_SECURE_NO_DEPRECATE

// Remove pointless warning messages
#ifdef _MSC_VER
#pragma warning (disable : 4511) // copy operator could not be generated
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
#pragma warning (disable : 4786) // identifier was truncated
#pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // eliminate deprecation warnings for VS2005
#endif
#endif // _MSC_VER
#ifdef __BORLANDC__
#pragma option -w-8027		   // function not expanded inline
#endif



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Required for VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WINXP//0x0501
#endif

#ifndef WINVER                 // Specify that the minimum required platform is Windows 7.
#define WINVER 0x0601          //WIN7
#endif
#define MOUSEEVENTF_FROMTOUCH 0xFF515700

#include "UIlib.h"

#include <olectl.h>

#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

//////////////////////////////////////////////////////////////////////////

#define PUGIXML_HEADER_ONLY
#ifdef _UNICODE
#define PUGIXML_WCHAR_MODE
#endif
#include "Utils/pugixml/pugixml.hpp"




///////////////////////////////////////////////////////////////////////////
//
//
/*
inline bool sIsFocused(UINT uState)
{
	return (uState & UISTATE_FOCUSED) == UISTATE_FOCUSED;
}

inline void sSetFocused(UINT &uState, bool bTrue)
{
	if(bTrue)
		uState |= UISTATE_FOCUSED;
	else
		uState &= ~UISTATE_FOCUSED;
}

inline bool sIsSelected(UINT uState)
{
	return (uState & UISTATE_SELECTED) == UISTATE_SELECTED;
}

inline void sSetSelected(UINT &uState, bool bTrue)
{
	if(bTrue)
		uState |= UISTATE_SELECTED;
	else
		uState &= ~UISTATE_SELECTED;
}

inline bool sIsDisable(UINT uState)
{
	return (uState & UISTATE_DISABLED) == UISTATE_DISABLED;
}

inline void sSetDisable(UINT &uState, bool bTrue)
{
	if(bTrue)
		uState |= UISTATE_DISABLED;
	else
		uState &= ~UISTATE_DISABLED;
}

inline bool sIsHot(UINT uState)
{
	return (uState & UISTATE_HOT) == UISTATE_HOT;
}

inline void sSetHot(UINT &uState, bool bTrue)
{
	if(bTrue)
		uState |= UISTATE_HOT;
	else
		uState &= ~UISTATE_HOT;
}

inline bool sIsPushed(UINT uState)
{
	return (uState & UISTATE_PUSHED) == UISTATE_PUSHED;
}

inline void sSetPushed(UINT &uState, bool bTrue)
{
	if(bTrue)
		uState |= UISTATE_PUSHED;
	else
		uState &= ~UISTATE_PUSHED;
}

inline bool sIsReadonly(UINT uState)
{
	return (uState & UISTATE_READONLY) == UISTATE_READONLY;
}

inline void sSetReadonly(UINT &uState, bool bTrue)
{
	if(bTrue)
		uState |= UISTATE_READONLY;
	else
		uState &= ~UISTATE_READONLY;
}

inline bool sIsCaptured(UINT uState)
{
	return (uState & UISTATE_CAPTURED) == UISTATE_CAPTURED;
}

inline void sSetCaptured(UINT &uState, bool bTrue)
{
	if(bTrue)
		uState |= UISTATE_CAPTURED;
	else
		uState &= ~UISTATE_CAPTURED;
}
*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E30B2003_188B_4EB4_AB99_3F3734D6CE6C__INCLUDED_)
