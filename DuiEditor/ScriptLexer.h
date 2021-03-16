#pragma once

struct tagMyIdentifier
{
	int Type;	// 1=keyword, 2=ȫ�ֺ�����3=ȫ�����ԣ�4=ϵͳ�࣬ 5=����
	CStringA strName;			//����
	CStringA strNameNoCase;		//Сд������
	CStringA strDataTypeName;	//����Ǳ��������ʾ��������
};

class CScriptLexer
{
public:
	CScriptLexer(void);
	~CScriptLexer(void);

	void SetEngine(asIScriptEngine *eng);

	virtual BOOL InitLexer(std::string &strKeyWord, std::string &strKeyWord2, std::string &strKeyWord3, std::string &strKeyWord4); 
	virtual std::string GetLexerText(const char *szCode, int parselength, const char *szWord);
	virtual std::string GetFunctionTips(const char *szWord);
	virtual std::string GetMethodTips(const char *szClass, const char *szMethod);
protected:	
	void GetLexerTextMethod(const char *szClass, const char *var, const char *szMethod);
	std::string MakeLexerText();
	void EmptyLexerText();
	BOOL AddLexerText(const char *text);

public:
	void ParseToken(const char *szCode, int nCodeLen);
protected:
	BOOL IsClass(const char *szWord);
	BOOL IsClassMember(const char *szWord);
	BOOL IsDataTypeIdentifier(const char *szWord);

	std::list<tagMyIdentifier *> m_arrToken;
	std::list<tagMyIdentifier *> m_arrUser;

	CStringA m_sLexerText;	//��ǰ���﷨��ʾ�Ľ��, ���ظ��������
public:
	asIScriptEngine  *engine;
};
