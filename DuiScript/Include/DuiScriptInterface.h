#pragma once	

#ifndef __DUISCRIPT_H__
#define __DUISCRIPT_H__

#ifdef UILIB_STATIC
#define UISCRIPT_API 
#else
#if defined(UISCRIPT_EXPORTS)
#	if defined(_MSC_VER)
#		define UISCRIPT_API __declspec(dllexport)
#	else
#		define UISCRIPT_API 
#	endif
#else
#	if defined(_MSC_VER)
#		define UISCRIPT_API __declspec(dllimport)
#	else
#		define UISCRIPT_API 
#	endif
#endif
#endif

#include "../angelscript/angelscript/include/angelscript.h"

#define SCRIPT_DEBUG_CONTINUE		1
#define SCRIPT_DEBUG_SETP_INTO		2
#define SCRIPT_DEBUG_SETP_OVER		3
#define SCRIPT_DEBUG_SETP_OUT		4
#define SCRIPT_DEBUG_SETP_CURSOR	5
#define SCRIPT_DEBUG_ABORT			6

typedef struct tagScriptMessage
{
	//0=GotoLine; 1=CheckBreakPoint; 2=PrintContext; 3=�����Ϣ, 4=��ʼ���У�5=����������6=�쳣����, 7=�ű�����, 8=�趨��ں�����9=�趨��ڲ���
	int nType; 
	int line;
	const char *lpszNotifyText;
	asIScriptContext *ctx;
}TScriptMessage;
typedef BOOL(CALLBACK *SCRIPTMESSAGECALLBACK)(TScriptMessage *pScriptMsg, UINT_PTR userdata);

class IScriptHelper
{
public:
	virtual void SetScriptMessageCallBack(SCRIPTMESSAGECALLBACK pFun, UINT_PTR userdata) = 0;
	virtual asIScriptEngine *GetEngine() const = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual bool CreateModule(LPCTSTR moduleName) = 0;
	virtual void DeleteModule() = 0;
	virtual bool AddScriptFile(LPCTSTR pstrFileName) = 0;
	virtual bool CompileScript() = 0;

	virtual BOOL IsRunning()			= 0;
	virtual void DebugRun()				= 0;
	virtual BOOL IsCanDebugRun()		= 0;
	virtual void DebugStop()			= 0;
	virtual BOOL IsCanDebugStop()		= 0;
	virtual void DebugStepInto()		= 0;
	virtual BOOL IsCanDebugStepInto()	= 0;
	virtual void DebugStepOver()		= 0;
	virtual BOOL IsCanDebugStepOver()	= 0;
	virtual void DebugStepReturn()		= 0;
	virtual BOOL IsCanDebugStepReturn() = 0;
	virtual void DebugStepCursor()		= 0;
	virtual BOOL IsCanDebugStepCursor() = 0;

	//������ں���
	virtual bool SetMainFun(LPCTSTR lpszMainFun) = 0;

	//������ں�������
	virtual BOOL	SetArgByte(UINT arg, BYTE value) = 0;
	virtual BOOL	SetArgWord(UINT arg, WORD value) = 0;
	virtual BOOL	SetArgDWord(UINT arg, DWORD value) = 0;
	virtual BOOL	SetArgFloat(UINT arg, float value) = 0;
	virtual BOOL	SetArgDouble(UINT arg, double value) = 0;
	virtual BOOL	SetArgAddress(UINT arg, void *addr) = 0;
	virtual BOOL	SetArgObject(UINT arg, void *obj) = 0;
	virtual void *  GetAddressOfArg(UINT arg) = 0;

	//ִ�нű�
	virtual bool	Excute() = 0;

	//��ȡ�����ķ���ֵ
	virtual BYTE	GetReturnByte() = 0;
	virtual WORD	GetReturnWord() = 0;
	virtual DWORD	GetReturnDWord() = 0;
	virtual float	GetReturnFloat() = 0;
	virtual double	GetReturnDouble() = 0;
	virtual void *	GetReturnAddress() = 0;
	virtual void *	GetReturnObject() = 0;
	virtual void *	GetAddressOfReturnValue() = 0;
};

UISCRIPT_API IScriptManager* __stdcall CreateScriptEngine();
UISCRIPT_API void __stdcall DeleteScriptEngine(IScriptManager *pEngine);

UISCRIPT_API void __stdcall DuiScriptRegister();

typedef IScriptHelper* (__stdcall *CREATE_SCRIPT_HELPER)();
typedef void (__stdcall *DELETE_SCRIPT_HELPER)(IScriptHelper *pHelper);

UISCRIPT_API IScriptHelper* __stdcall CreateScriptHelper();
UISCRIPT_API void __stdcall DeleteScriptHelper(IScriptHelper *pHelper);


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifndef UISCRIPT_EXPORTS

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef UILIB_STATIC

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64usd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64us.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64sd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64s.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_usd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_us.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_sd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_s.lib"))
#		endif
#	endif
#endif

#else //#ifndef UISCRIPT_EXPORTS

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_64.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript_d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiScript.lib"))
#		endif
#	endif
#endif

#endif //#ifdef UILIB_STATIC
#endif //#ifndef UILIB_EXPORTS


#endif // __DUISCRIPT_H__
