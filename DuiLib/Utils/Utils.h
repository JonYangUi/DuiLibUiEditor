#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once
#include "OAIdl.h"
#include <vector>
#include <map>

namespace DuiLib
{
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API STRINGorID
	{
	public:
		STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
		{ }
		STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
		{ }
		LPCTSTR m_lpstr;
	};

	class CDuiString;
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiPoint : public tagPOINT
	{
	public:
		CDuiPoint();
		CDuiPoint(const POINT& src);
		CDuiPoint(int x, int y);
		CDuiPoint(LPARAM lParam);
		CDuiPoint(LPCTSTR pstrValue);
		bool FromString(LPCTSTR pstrValue); //��"x,y"����POINT
		CDuiString ToString();				//����ַ���"x,y"
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiSize : public tagSIZE
	{
	public:
		CDuiSize();
		CDuiSize(const SIZE& src);
		CDuiSize(const RECT rc);
		CDuiSize(int cx, int cy);
		CDuiSize(LPCTSTR pstrValue);
		bool FromString(LPCTSTR pstrValue); //��"cx,cy"����SIZE
		CDuiString ToString();				//����ַ���"cx,cy"
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiRect : public tagRECT
	{
	public:
		CDuiRect();
		CDuiRect(const RECT& src);
		CDuiRect(int iLeft, int iTop, int iRight, int iBottom);
		CDuiRect(LPCTSTR pstrValue);
		bool FromString(LPCTSTR pstrValue); //��"left,top,right,bottom"����RECT
		CDuiString ToString();				//����ַ���"left,top,right,bottom"

		int GetWidth() const;
		int GetHeight() const;
		void Empty();
		bool IsNull() const;
		void Join(const RECT& rc);
		void ResetOffset();
		void Normalize();
		void Offset(int cx, int cy);
		void Inflate(int cx, int cy);
		void Deflate(int cx, int cy);
		void Union(CDuiRect& rc);
		BOOL IntersectRect(const RECT &rect1, const RECT &rect2);
		POINT CenterPoint() const;
		void AlignRect(const RECT &rc, UINT uAlign = DT_CENTER|DT_VCENTER);
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdPtrArray
	{
	public:
		CStdPtrArray(int iPreallocSize = 0);
		CStdPtrArray(const CStdPtrArray& src);
		~CStdPtrArray();

		void Empty();
		void Resize(int iSize);
		bool IsEmpty() const;
		int Find(LPVOID iIndex) const;
		int FindInMap(LPVOID iIndex);
		bool Add(LPVOID pData);
		bool SetAt(int iIndex, LPVOID pData);
		bool InsertAt(int iIndex, LPVOID pData);
		bool Remove(int iIndex);
		bool Remove(LPVOID pData);
		int GetSize() const;
		LPVOID* GetData();

		LPVOID GetAt(int iIndex) const;
		LPVOID operator[] (int nIndex) const;

		void SetSaveIndexMap(bool bIndexMap);
	protected:
		LPVOID* m_ppVoid;
		int m_nCount;
		int m_nAllocated;

		bool m_bSaveIndexMap;
		std::map<LPVOID, int> m_mapIndex;	//�洢������ţ���߲���ָ���Ч��
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdValArray
	{
	public:
		CStdValArray(int iElementSize, int iPreallocSize = 0);
		~CStdValArray();

		void Empty();
		bool IsEmpty() const;
		bool Add(LPCVOID pData);
		bool Remove(int iIndex);
		int GetSize() const;
		LPVOID GetData();

		LPVOID GetAt(int iIndex) const;
		LPVOID operator[] (int nIndex) const;

	protected:
		LPBYTE m_pVoid;
		int m_iElementSize;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiString
	{
	public:
		enum { MAX_LOCAL_STRING_LEN = 63 };

		CDuiString();
		CDuiString(const TCHAR ch);
		CDuiString(const CDuiString& src);
		CDuiString(LPCTSTR lpsz, int nLen = -1);
		~CDuiString();

		void Empty();
		int GetLength() const;
		bool IsEmpty() const;
		TCHAR GetAt(int nIndex) const;
		void Append(LPCTSTR pstr);
		void Assign(LPCTSTR pstr, int nLength = -1);
		LPCTSTR GetData() const;

		void SetAt(int nIndex, TCHAR ch);
		operator LPCTSTR() const;

		TCHAR operator[] (int nIndex) const;
		const CDuiString& operator=(const CDuiString& src);
		const CDuiString& operator=(const TCHAR ch);
		const CDuiString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
		const CDuiString& operator=(LPCSTR lpStr);
		const CDuiString& operator+=(LPCSTR lpStr);
#else
		const CDuiString& operator=(LPCWSTR lpwStr);
		const CDuiString& operator+=(LPCWSTR lpwStr);
#endif
		CDuiString operator+(const CDuiString& src) const;
		CDuiString operator+(LPCTSTR pstr) const;
		const CDuiString& operator+=(const CDuiString& src);
		const CDuiString& operator+=(LPCTSTR pstr);
		const CDuiString& operator+=(const TCHAR ch);

		bool operator == (LPCTSTR str) const;
		bool operator != (LPCTSTR str) const;
		bool operator <= (LPCTSTR str) const;
		bool operator <  (LPCTSTR str) const;
		bool operator >= (LPCTSTR str) const;
		bool operator >  (LPCTSTR str) const;

		int Compare(LPCTSTR pstr) const;
		int CompareNoCase(LPCTSTR pstr) const;

		void MakeUpper();
		void MakeLower();

		CDuiString Left(int nLength) const;
		CDuiString Mid(int iPos, int nLength = -1) const;
		CDuiString Right(int nLength) const;

		int Find(TCHAR ch, int iPos = 0) const;
		int Find(LPCTSTR pstr, int iPos = 0) const;
		int ReverseFind(TCHAR ch) const;
		int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

		int __cdecl Format(LPCTSTR pstrFormat, ...);
		int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

		int __cdecl InnerFormat(LPCTSTR pstrFormat, va_list Args);

	protected:
		LPTSTR m_pstr;
		TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
	};

	static std::vector<CDuiString> StrSplit(CDuiString text, CDuiString sp)
	{
		std::vector<CDuiString> vResults;
		int pos = text.Find(sp, 0);
		while (pos >= 0)
		{
			CDuiString t = text.Left(pos);
			vResults.push_back(t);
			text = text.Right(text.GetLength() - pos - sp.GetLength());
			pos = text.Find(sp);
		}
		vResults.push_back(text);
		return vResults;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	//

	struct TITEM
	{
		CDuiString Key;
		LPVOID Data;
		struct TITEM* pPrev;
		struct TITEM* pNext;
	};

	class UILIB_API CStdStringPtrMap
	{
	public:
		CStdStringPtrMap(int nSize = 83);
		~CStdStringPtrMap();

		void Resize(int nSize = 83);
		LPVOID Find(LPCTSTR key, bool optimize = true) const;
		bool Insert(LPCTSTR key, LPVOID pData);
		LPVOID Set(LPCTSTR key, LPVOID pData);
		bool Remove(LPCTSTR key);
		void RemoveAll();
		int GetSize() const;
		LPCTSTR GetAt(int iIndex) const;
		LPCTSTR operator[] (int nIndex) const;

	protected:
		TITEM** m_aT;
		int m_nBuckets;
		int m_nCount;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDuiString������
	class UILIB_API CDuiStringArray
	{
	public:
		CDuiStringArray();
		CDuiStringArray(const CDuiStringArray& src);
		~CDuiStringArray();

		int GetSize() const;
		bool IsEmpty() const;
		void RemoveAll();

		bool Add(LPCTSTR newElement, bool bNoRepeat=false);
		bool Remove(LPCTSTR element);

		bool Find(LPCTSTR element);

		CDuiString GetAt(int iIndex) const;
		CDuiString& operator[] (int nIndex) const;

		//�ָ��ַ������������飬src=��Ҫ�ָ���ַ�����sp=�ָ��. ���ָ���ΪNULL�������ĸ����Ϊ�ָ���š�
		void SplitString(LPCTSTR src, LPCTSTR sp = NULL);

	private:
		CStdStringPtrMap m_map;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiWaitCursor
	{
	public:
		CDuiWaitCursor();
		~CDuiWaitCursor();

	protected:
		HCURSOR m_hOrigCursor;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CDuiVariant : public VARIANT
	{
	public:
		CDuiVariant() 
		{ 
			VariantInit(this); 
		}
		CDuiVariant(int i)
		{
			VariantInit(this);
			this->vt = VT_I4;
			this->intVal = i;
		}
		CDuiVariant(float f)
		{
			VariantInit(this);
			this->vt = VT_R4;
			this->fltVal = f;
		}
		CDuiVariant(LPOLESTR s)
		{
			VariantInit(this);
			this->vt = VT_BSTR;
			this->bstrVal = s;
		}
		CDuiVariant(IDispatch *disp)
		{
			VariantInit(this);
			this->vt = VT_DISPATCH;
			this->pdispVal = disp;
		}

		~CDuiVariant() 
		{ 
			VariantClear(this); 
		}
	};

	//////////////////////////////////////////////////////////////////////////
	////
	//lock��
	class CDuiLock
	{
	public:
		CDuiLock(){ InitializeCriticalSectionAndSpinCount(&m_lock, 5000); }
		~CDuiLock(){ DeleteCriticalSection(&m_lock); }
		void Lock(){ EnterCriticalSection(&m_lock); }
		void Unlock(){ LeaveCriticalSection(&m_lock); }
	private:
		CRITICAL_SECTION m_lock;
	};

	//lock��
	class CDuiInnerLock
	{
	public:
		CDuiInnerLock(CDuiLock *p) : ptr(p) { ptr->Lock(); }
		~CDuiInnerLock(){ ptr->Unlock(); }
	private:
		CDuiLock *ptr;
	};	

	//ͨ�ÿؼ��ڴ��
	template <class T>
	class CStdControlPool
	{
		struct tagObject
		{
			tagObject *prev;
			tagObject *next;
		};
	public:
		CStdControlPool()
		{
			_blockcountnext = 32;
			_nMaxMemoryPageSize = 1024 * 5;
			m_pFirstNode = NULL;
			m_pLastNode = NULL;
		}

		~CStdControlPool()
		{
			ClearMemory();
		}

		//�趨ÿ�������ڴ�ʱ���ؼ����������ޡ� �����˷��ڴ棬����Ӱ��Ч�ʣ�����ʹ�ó���������
		void SetMaxMemoryPageSize(int nSize)
		{
			_nMaxMemoryPageSize = nSize;
		}

		T* Alloc()
		{
			if(m_pFirstNode == NULL) MakePool();
			if(m_pFirstNode == NULL) return NULL;

			tagObject *p = m_pFirstNode;
			pop(p); //�ӳ���ȡ����
			T *tt = (T*)(p+1);
			new (tt)T; //ʹ��tt����ڴ��ַnew T()
			return tt;
		}

		void Free(T *t)
		{
			t->~T();
			memset(t, 0, sizeof(T));
			tagObject *p = (tagObject *)t-1;
			pushback(p); //�Żس���
			t = NULL;
		}

		//�����ڴ棬��������������֮ǰ�����ڴ�û�й黹��Ҳ������������Ұָ�롣
		//ҪС��ʹ�ã����ܵ����޷��ж�����ʱ�ڴ�й©��
		void ClearMemory() 
		{
			//DWORD tk = GetTickCount();

			for (int i=0; i<m_pListMemBlock.GetSize(); i++)
			{
				free( (void*)m_pListMemBlock[i] );
			}

// 			CDuiString s;
// 			s.Format(_T("%d"), GetTickCount() - tk);
// 			MessageBox(NULL, s, _T("�ͷ�ʱ��"), MB_OK);
		}

	protected:
		void MakePool()
		{
			//����һ�������ռ䣬��Ϊ���ÿ��object��newһ�Σ�new��delete����ķѴ���ʱ�䡣
			int tagSize = sizeof(tagObject) + sizeof(T);
			BYTE *pBlock = (BYTE *)malloc(tagSize * _blockcountnext);
			for (int i=0; i<_blockcountnext; i++)
			{
				tagObject *p = new (pBlock + i*tagSize)tagObject;
				p->prev = NULL;
				p->next = NULL;

				T *tt = (T*)(p+1);
				memset(tt, 0, sizeof(T));
				pushback(p);
			}
			m_pListMemBlock.Add(pBlock);

			//���´η����ڴ�ʱ����Ҫ�򵥴ֱ��ĳ���2���趨һ������
			if(_blockcountnext < _nMaxMemoryPageSize)
				_blockcountnext *= 2;
			if(_blockcountnext > _nMaxMemoryPageSize)
				_blockcountnext = _nMaxMemoryPageSize;
		}

		void pushback(tagObject *pBlock)
		{
			if(m_pFirstNode == NULL)	//�޶���ͷ
			{
				pBlock->prev = NULL;
				pBlock->next = NULL;
				m_pFirstNode = pBlock;
				m_pLastNode = pBlock;
			}
			else	//�ڶ���β������
			{
				m_pLastNode->next = pBlock;
				pBlock->prev = m_pLastNode;
				pBlock->next = NULL;
				m_pLastNode = pBlock;
			}
		}

		void pop(tagObject *pBlock)
		{
			if(m_pFirstNode == m_pLastNode)
			{
				m_pFirstNode = NULL;
				m_pLastNode = NULL;
			}
			else if(pBlock == m_pFirstNode)
			{
				m_pFirstNode = pBlock->next;
				m_pFirstNode->prev = NULL;

			}
			else if(pBlock == m_pLastNode)
			{
				m_pLastNode = pBlock->prev;
				m_pLastNode->next = NULL;
			}
			else
			{
				pBlock->prev->next = pBlock->next;
				pBlock->next->prev = pBlock->prev;
			}
		}

	private:
		tagObject *m_pFirstNode;
		tagObject *m_pLastNode;
		int _blockcountnext;
		int _nMaxMemoryPageSize;
		CStdPtrArray m_pListMemBlock;
	};
	///////////////////////////////////////////////////////////////////////////////////////
	////
	//struct TImageInfo;
	//class CPaintManagerUI;
	//class UILIB_API CImageString
	//{
	//public:
	//	CImageString();
	//	CImageString(const CImageString&);
	//	const CImageString& operator=(const CImageString&);
	//	virtual ~CImageString();

	//	const CDuiString& GetAttributeString() const;
	//	void SetAttributeString(LPCTSTR pStrImageAttri);
	//	void ModifyAttribute(LPCTSTR pStrModify);
	//	bool LoadImage(CPaintManagerUI* pManager);
	//	bool IsLoadSuccess();

	//	RECT GetDest() const;
	//	void SetDest(const RECT &rcDest);
	//	const TImageInfo* GetImageInfo() const;

	//private:
	//	void Clone(const CImageString&);
	//	void Clear();
	//	void ParseAttribute(LPCTSTR pStrImageAttri);

	//protected:
	//	friend class CRenderEngine;
	//	CDuiString	m_sImageAttribute;

	//	CDuiString	m_sImage;
	//	CDuiString	m_sResType;
	//	TImageInfo	*m_imageInfo;
	//	bool		m_bLoadSuccess;

	//	RECT	m_rcDest;
	//	RECT	m_rcSource;
	//	RECT	m_rcCorner;
	//	BYTE	m_bFade;
	//	DWORD	m_dwMask;
	//	bool	m_bHole;
	//	bool	m_bTiledX;
	//	bool	m_bTiledY;
	//};
}// namespace DuiLib

#endif // __UTILS_H__