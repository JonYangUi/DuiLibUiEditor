#pragma once


namespace DuiLib
{

class CScriptEngine
{
public:
	CScriptEngine(void);
	~CScriptEngine(void);

	asIScriptEngine *GetEngine();
protected:
	void Init();
	void reg_GlobalProperty(); //ע��ȫ������
	void reg_ControlHierarchies(); //ͳһע����Ĳ�ι�ϵ

	void MessageCallback(const asSMessageInfo &msg);
	asIScriptEngine  *engine;
};

extern CScriptEngine g_ScriptEngine;

} //namespace DuiLib