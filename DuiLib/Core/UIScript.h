#ifndef __UISCRIPT_H__
#define __UISCRIPT_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	//�ű���������
	enum emUIArgTypes
	{
		UIArg_void		= 0,
		UIArg_BYTE,
		UIArg_WORD,
		UIArg_DWORD,
		UIArg_float,
		UIArg_double,
		UIArg_address,
		UIArg_object,
	};

	//ͨ�ýű���������
	class UILIB_API IScriptFunction
	{
	public:
		struct TArgItem
		{
			emUIArgTypes _type;
			BYTE _byte;
			WORD _word;
			DWORD _dword;
			float _float;
			double _double;
			void *_addr;
			void *_obj;
		};
	public:
		IScriptFunction();
		virtual ~IScriptFunction();

		//���ú�������ֵ����
		virtual void SetReturnType(emUIArgTypes argType);

		//���ú�����
		virtual void SetFuctionName(LPCTSTR sFunName);

		//��������ջ����
		virtual void PushArgByte(BYTE value);
		virtual void PushArgWord(WORD value);
		virtual void PushArgDWord(DWORD value);
		virtual void PushArgFloat(float value);
		virtual void PushArgDouble(double value);
		virtual void PushArgAddress(void *addr);
		virtual void PushArgObject(void *obj);

		//��ȡ����ֵ
		virtual BYTE	GetReturnByte();
		virtual WORD	GetReturnWord();
		virtual DWORD	GetReturnDWord();
		virtual float	GetReturnFloat();
		virtual double	GetReturnDouble();
		virtual void *	GetReturnAddress();
		virtual void *	GetReturnObject();


		emUIArgTypes m_emReturnArgTypes;
		CDuiString m_sFunName;
		CStdPtrArray m_arrArgs;
		TArgItem m_result;
	};
	

} // namespace DuiLib

#endif // __UISCRIPT_H__
