#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once
#include "OAIdl.h"
#include <vector>
#include <map>

#include "DuiString.h"
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

	//////////////////////////////////////////////////////////////////////////
	//
	//
	struct IObjRef
	{
		virtual long AddRef() PURE;

		virtual long Release() PURE;

		virtual void OnFinalRelease() PURE;
	};

	//////////////////////////////////////////////////////////////////////////
	//  
	//
	template<class T>
	class TObjRefImpl :  public T
	{
	public:
		TObjRefImpl():m_cRef(1)
		{
		}
		virtual ~TObjRefImpl() {}

		//�������
		virtual long AddRef()
		{
			return InterlockedIncrement(&m_cRef);
		}

		//!�ͷ�����
		virtual long Release()
		{
			long lRet = InterlockedDecrement(&m_cRef);
			if(lRet==0)
			{
				OnFinalRelease();
			}
			return lRet;
		}

		//!�ͷŶ���
		virtual void OnFinalRelease()
		{
			delete this;
		}
	protected:
		volatile LONG m_cRef;
	};

	template <class T>
	class MakeRefPtr
	{
	public:
		MakeRefPtr(T *lp) { p = lp; }
		T *p;
	};

	//////////////////////////////////////////////////////////////////////////
	/*  ע�⣺ֱ�Ӹ�ֵָ��ᵼ���������ӣ�����������Ҫֱ�Ӹ�ֵ��Ӧ��ʹ��MakeRefPtr��������std::make_shared�� 
	  UIFont *pFont = ...CreateFont();							//���� = 1
	  1, CStdRefPtr<UIFont> font = pFont;						//���� = 2;
	  2, CStdRefPtr<UIFont> font = MakeRefPtr<UIFont>(pFont)	//���� = 1;  
	  3, *((UIFont**)&font) = pFont;							//���� = 1;  ��ȡCStdRefPtr�ڲ�ָ��ֱ�Ӹ�ֵ��
	  4, Ϊ�˷�ֹ���󣬽�ֹdelete pFont; ֻ��pFont->Release();
	*/
	template <class T>
	class CStdRefPtr
	{
	public:
		CStdRefPtr() throw()
		{
			p = NULL;
		}
		CStdRefPtr(int nNull) throw()
		{
			(void)nNull;
			p = NULL;
		}
		CStdRefPtr(T* lp) throw()
		{
			p = lp;
			if (p != NULL)
			{
				p->AddRef();
			}
		}

		CStdRefPtr(const CStdRefPtr & src) throw()
		{
			p=src.p;
			if(p)
			{
				p->AddRef();
			}
		}

		CStdRefPtr(const MakeRefPtr<T> & src) throw()
		{
			p = src.p;
			//��ҪAddRef();
		}

		~CStdRefPtr() throw()
		{
			if (p)
			{
				p->Release();
			}
		}

		T* operator->() const throw()
		{
			return p;
		}

		operator T*() const throw()
		{
			return p;
		}

		T& operator*() const
		{
			return *p;
		}

		bool operator!() const throw()
		{
			return (p == NULL);
		}

		bool operator<(T* pT) const throw()
		{
			return p < pT;
		}

		bool operator!=(T* pT) const
		{
			return !operator==(pT);
		}

		bool operator==(T* pT) const throw()
		{
			return p == pT;
		}

		T* operator=(T* lp) throw()
		{
			if(*this!=lp)
			{
				if(p)
				{
					p->Release();
				}
				p=lp;
				if(p)
				{
					p->AddRef();
				}
			}
			return *this;
		}

		T* operator=(const MakeRefPtr<T> &mk) throw()
		{
			if(*this != mk.p)
			{
				if(p)
				{
					p->Release();
				}
				p=mk.p;
				//��ҪAddRef();
			}
			return *this;
		}

		T* operator=(const CStdRefPtr<T>& lp) throw()
		{
			if(*this!=lp)
			{
				if(p)
				{
					p->Release();
				}
				p=lp;
				if(p)
				{
					p->AddRef();
				}
			}
			return *this;	
		}

		// Release the interface and set to NULL
		void Release() throw()
		{
			T* pTemp = p;
			if (pTemp)
			{
				p = NULL;
				pTemp->Release();
			}
		}

		// Attach to an existing interface (does not AddRef)
		void Attach(T* p2) throw()
		{
			if (p)
			{
				p->Release();
			}
			p = p2;
		}
		// Detach the interface (does not Release)
		T* Detach() throw()
		{
			T* pt = p;
			p = NULL;
			return pt;
		}
		HRESULT CopyTo(T** ppT) throw()
		{
			if (ppT == NULL)
				return E_POINTER;
			*ppT = p;
			if (p)
			{
				p->AddRef();
			}
			return S_OK;
		}

	protected:
		T* p;
	};

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

	//////////////////////////////////////////////////////////////////////////
	//lock��
	class CDuiInnerLock
	{
	public:
		CDuiInnerLock(CDuiLock *p) : ptr(p) { ptr->Lock(); }
		~CDuiInnerLock(){ ptr->Unlock(); }
	private:
		CDuiLock *ptr;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//˫��������ӿ�
	struct ILinkedList
	{
		ILinkedList() 
		{ 
			pLinkedPrev = NULL; 
			pLinkedNext = NULL; 
		}

		ILinkedList *prev() { return pLinkedPrev; }
		ILinkedList *next() { return pLinkedNext; }

		ILinkedList *pLinkedPrev;
		ILinkedList *pLinkedNext;
	};

	//˫��������
	template <class T>
	class CStdLinkList
	{
	public:
		CStdLinkList()
		{
			m_pHeader = NULL;
			m_pTail = NULL;
			m_count = 0;
		}

		void push_front(T *pItem)
		{
			if(m_pHeader == NULL)
			{
				pItem->pLinkedPrev = NULL;
				pItem->pLinkedNext = NULL;
				m_pHeader = pItem;
				m_pTail = pItem;
			}
			else
			{
				m_pHeader->pLinkedPrev = pItem;
				pItem->pLinkedPrev = NULL;
				pItem->pLinkedNext = m_pHeader;
				m_pHeader = pItem;
			}
			m_count++;
		}

		void push_back(T *pItem)
		{
			if(m_pHeader == NULL)
			{
				pItem->pLinkedPrev = NULL;
				pItem->pLinkedNext = NULL;
				m_pHeader = pItem;
				m_pTail = pItem;
			}
			else
			{
				m_pTail->pLinkedNext = pItem;
				pItem->pLinkedPrev = m_pTail;
				pItem->pLinkedNext = NULL;
				m_pTail = pItem;
			}
			m_count++;
		}

		void insert(T *pItemPosition, T *pItem)
		{
			if(pItemPosition == m_pHeader)
			{
				push_front(pItem);
				m_count++;
			}
			else if(pItemPosition == m_pTail)
			{
				push_back(pItem);
				m_count++;
			}
			else
			{
				pItem->pLinkedNext = pItemPosition;
				pItem->pLinkedPrev = pItemPosition->pLinkedPrev;
				pItem->pLinkedPrev->pLinkedNext = pItem;
				pItemPosition->pLinkedPrev = pItem;
				m_count++;
			}
		}

		void erase(T *pItem)
		{
			if(pItem == m_pHeader && m_pHeader == m_pTail)
			{
				m_pHeader = m_pTail = NULL;
				m_count--;
			}
			else if(pItem == m_pHeader)
			{
				m_pHeader = (T *)m_pHeader->pLinkedNext;
				m_pHeader->pLinkedPrev = NULL;
				m_count--;
			}
			else if(pItem = m_pTail)
			{
				m_pTail = (T *)m_pTail->pLinkedPrev;
				m_pTail->pLinkedNext = NULL;
				m_count--;
			}
			else
			{
				pItem->pLinkedPrev->pLinkedNext = pItem->pLinkedNext;
				pItem->pLinkedNext->pLinkedPrev = pItem->pLinkedPrev;
				m_count--;
			}
		}

		T* pop_front()
		{
			if(m_pHeader == NULL)
				return NULL;

			T *pTemp = NULL;

			if(m_pHeader == m_pTail)
			{
				pTemp = m_pHeader;
				m_pHeader = NULL;
				m_pTail = NULL;
			}
			else
			{
				pTemp = m_pHeader;
				m_pHeader = (T *)m_pHeader->pLinkedNext;
				m_pHeader->pLinkedPrev = NULL;
			}

			m_count--;
			return pTemp;
		}

		T* pop_back()
		{
			if(m_pHeader == NULL)
				return NULL;

			T *pTemp = NULL;

			if(m_pHeader == m_pTail)
			{
				pTemp = m_pHeader;
				m_pHeader = NULL;
				m_pTail = NULL;
			}
			else
			{
				pTemp = m_pTail;
				m_pTail = (T *)m_pTail->pLinkedPrev;
				m_pTail->pLinkedNext = NULL;
			}

			m_count--;
			return pTemp;
		}

		bool empty()
		{
			return m_pHeader == NULL;
		}

		T* header()
		{
			return m_pHeader;
		}

		T* tail()
		{
			return m_pTail;
		}

		int size()
		{
			return m_count;
		}
	private:
		T *m_pHeader;
		T *m_pTail;
		int m_count;
	};

	//////////////////////////////////////////////////////////////////////////
	//�ؼ��ڴ�
	template <class T>
	class CStdControlPool
	{
	public:
		CStdControlPool()
		{
			_blockcountnext = 32;
			_nMaxMemoryPageSize = 1024 * 5;
#ifdef _DEBUG
			OutputDebugStringA("construct pool\r\n");
#endif
		}

		~CStdControlPool()
		{
			ClearMemory();
#ifdef _DEBUG
			OutputDebugStringA("destroy pool\r\n");
#endif
		}

		//�趨ÿ�������ڴ�ʱ���ؼ����������ޡ� �����˷��ڴ棬����Ӱ��Ч�ʣ�����ʹ�ó���������
		void SetMaxMemoryPageSize(int nSize)
		{
			_nMaxMemoryPageSize = nSize;
		}

		T* Alloc()
		{
			if(m_listControl.empty())
			{
				MakePool();
			}

			T *tt = (T *)m_listControl.pop_back();
			new (tt)T; //ʹ��tt����ڴ��ַnew T()
			return tt;
		}

		void Free(T *t)
		{
			t->~T();
			memset(t, 0, sizeof(T));
			m_listControl.push_back(t);
		}

		//�����ڴ棬��������������֮ǰ�����ڴ�û�й黹��Ҳ������������Ұָ�롣
		//ҪС��ʹ�ã����ܵ����޷��ж�����ʱ�ڴ�й©��
		void ClearMemory() 
		{
			//DWORD tk = GetTickCount();

			for (int i=0; i<m_pListMemBlock.GetSize(); i++)
			{
				free( (BYTE*)m_pListMemBlock[i] );
			}

			// 			CDuiString s;
			// 			s.Format(_T("%d"), GetTickCount() - tk);
			// 			MessageBox(NULL, s, _T("�ͷ�ʱ��"), MB_OK);
		}

	protected:
		void MakePool()
		{
			//����һ�������ռ䣬��Ϊ���ÿ��object��newһ�Σ�new��delete����ķѴ���ʱ�䡣
			int tagSize = sizeof(T);
			BYTE *pBlock = (BYTE *)malloc(tagSize * _blockcountnext);
			memset(pBlock, 0, tagSize);
			for (int i=0; i<_blockcountnext; i++)
			{
				T *p = (T *)(pBlock + i*tagSize);
				m_listControl.push_back(p);
			}
			m_pListMemBlock.Add(pBlock);

			//���´η����ڴ�ʱ����Ҫ�򵥴ֱ��ĳ���2���趨һ������
			if(_blockcountnext < _nMaxMemoryPageSize)
				_blockcountnext *= 2;
			if(_blockcountnext > _nMaxMemoryPageSize)
				_blockcountnext = _nMaxMemoryPageSize;
		}

	private:
		CStdLinkList<T> m_listControl;
		int _blockcountnext;
		int _nMaxMemoryPageSize;
		CStdPtrArray m_pListMemBlock;
	};
	
	/*
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
#ifdef _DEBUG
			OutputDebugStringA("construct pool\r\n");
#endif
		}

		~CStdControlPool()
		{
			ClearMemory();
#ifdef _DEBUG
			OutputDebugStringA("destroy pool\r\n");
#endif
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
				free( (BYTE*)m_pListMemBlock[i] );
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
	*/
}// namespace DuiLib

#endif // __UTILS_H__