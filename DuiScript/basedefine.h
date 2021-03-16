#pragma once

#define DEFUNC(x)	#x

#define REG_OBJECT_TYPE(class) { \
	r = engine->RegisterObjectType(#class, 0, asOBJ_REF|asOBJ_NOCOUNT);	\
	assert( r >= 0 );	\
}

//�����๤������
#define DECL_FACTORY(x) static x *x##_Ref_Factory() { return new x(); }

//ע���๤��
#define REG_FACTORY(x) {	\
	r = engine->RegisterObjectBehaviour(#x, asBEHAVE_FACTORY, DEFUNC(x##@ f()), asFUNCTION( x##_Ref_Factory), asCALL_CDECL);  \
	assert( r >= 0 );	\
}

//ע��AddRef
#define DECL_ADD_REF(class, fun) {	\
	r = engine->RegisterObjectBehaviour(#class, asBEHAVE_ADDREF, "void f()", asMETHOD(class, fun), asCALL_THISCALL); \
	assert( r >= 0 );	\
}

//ע��Release
#define DECL_RELEASE_REF(class, fun) { \
	r = engine->RegisterObjectBehaviour(#class, asBEHAVE_RELEASE, "void f()", asMETHOD(class, fun), asCALL_THISCALL); \
	assert( r >= 0 );	\
}

//////////////////////////////////////////////////////////////////////////
//ע���ຯ��
#define REG_METHOD_FUNPR(class, ret, n, p)	{	\
	CStringA sFun = DEFUNC(ret n##p);			\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");	\
	r = engine->RegisterObjectMethod(#class, sFun, asMETHODPR(class, n, p, ret), asCALL_THISCALL);  \
	assert( r >= 0 ); \
}

//ע�����Ա
#define REG_OBJECT_PROPERTY(class, type, value) { \
	CStringA sFun = DEFUNC(type value);	\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");	\
	r = engine->RegisterObjectProperty(#class, sFun, asOFFSET(class, value)); \
	assert( r >= 0 ); \
}

//////////////////////////////////////////////////////////////////////////
//ע��ȫ�ֺ���, ע��ʱ������ľ�̬������Class::Fun(), ��Ҫ��"Class::"�Ƴ���ע��Ϊȫ�ֺ���;
#define REG_GLOBAL_FUNPR(ret, n, p)	{			\
	CStringA sFun = DEFUNC(ret n##p);	\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");						\
	if(sFun.Find("::") >= 0)					\
	{											\
		int start = sFun.Find(" ");				\
		if(start < 0) start = 0;				\
		sFun.Delete(start+1, sFun.Find("::")+2-start-1);	\
	}											\
	r = engine->RegisterGlobalFunction(sFun, asFUNCTIONPR(n, p, ret), asCALL_CDECL);  \
	assert( r >= 0 ); \
}

//����ؼ��๤��
#define DECL_CONTROL_FACTORY(x)	static x *x##_Ref_Factory() { return new x(); }
#define DECL_CONTROL_REGFACT(x) virtual void reg_factory() { int r = 0; REG_FACTORY(x); }
#define REG_CONTROL_FACTORY() reg_factory()


//ע��ؼ��ĳ�Ա����
#define REG_CONTROL_FUNPR(ret,n,p)	{			\
	CStringA sFun = DEFUNC(ret n##p);			\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");	\
	r = engine->RegisterObjectMethod(classname, sFun, asMETHODPR(T, n, p, ret), asCALL_THISCALL);  \
	assert( r >= 0 ); \
}

//ע��duilib�ؼ�
#define REGISTER_CONTROL(x) { reg##x<x> ctrl; ctrl.classname=#x;  ctrl.reg(engine); }	


#define DECL_PROP_UINT(m)		static UINT m##property = m;		//����UINTȫ�ֱ���
#define DECL_PROP_STRING(m)		static CDuiString m##property = m;	//����stringȫ�ֱ���
#define PROP_PARAPETER(type,value)	type##value	//����typevalue�ַ���
#define REGI_PROP_UINT(m)		r = engine->RegisterGlobalProperty(PROP_PARAPETER("const uint ",#m), &##m##property); assert( r >= 0 );		//ע��UINTȫ�ֱ���
#define REGI_PROP_STRING(m)		r = engine->RegisterGlobalProperty(PROP_PARAPETER("const string ",#m), &##m##property); assert( r >= 0 );	//ע��stringȫ�ֱ���


// Example opCast behaviour
template<class A, class B>
B* refCast(A* a)
{
	// If the handle already is a null handle, then just return the null handle
	if( !a ) return 0;

	// Now try to dynamically cast the pointer to the wanted type
	B* b = dynamic_cast<B*>(a);
	// 	if( b != 0 )
	// 	{
	// 		// Since the cast was made, we need to increase the ref counter for the returned handle
	// 		b->addref();
	// 	}
	return b;
}
//ע����Ĳ�ι�ϵ: �̳�
#define REG_CLASS_HIERARCHIES(base,derived) {	\
	r = engine->RegisterObjectMethod(#base, DEFUNC(derived@ opCast()), asFUNCTION((refCast<base,derived>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );					\
	r = engine->RegisterObjectMethod(#derived, DEFUNC(base@ opImplCast()), asFUNCTION((refCast<derived,base>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );				\
	r = engine->RegisterObjectMethod(#base, DEFUNC(const derived@ opCast() const), asFUNCTION((refCast<base,derived>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );		\
	r = engine->RegisterObjectMethod(#derived, DEFUNC(const base@ opImplCast() const), asFUNCTION((refCast<derived,base>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );	\
}

