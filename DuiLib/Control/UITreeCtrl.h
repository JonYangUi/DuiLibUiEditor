#ifndef UITreeViewEx_h__
#define UITreeViewEx_h__
#pragma once

namespace DuiLib
{
	class UILIB_API CTreeCtrlUI : public CVerticalLayoutUI//, public ITreeNodeUI
	{
		DECLARE_DUICONTROL(CTreeCtrlUI)
	public:
		CTreeCtrlUI(void);
		~CTreeCtrlUI(void);

		friend class CTreeItemUI;
	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID	GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		CTreeItemUI *GetFocusNode() const;
		void SetFocusNode(CTreeItemUI *pNode);

		void SetStyleHoriz(LPCTSTR pstrValue)		{ m_sStyleHoriz = pstrValue;		}
		LPCTSTR GetStyleHoriz()						{ return m_sStyleHoriz;				}
		void SetStyleFolder(LPCTSTR pstrValue)		{ m_sStyleFolder = pstrValue;		}
		LPCTSTR GetStyleFolder()					{ return m_sStyleFolder;			}
		void SetStyleCheckBox(LPCTSTR pstrValue)	{ m_sStyleCheckBox = pstrValue;		}
		LPCTSTR GetStyleCheckBox()					{ return m_sStyleCheckBox;			}
		void SetStyleIcon(LPCTSTR pstrValue)		{ m_sStyleIcon = pstrValue;			}
		LPCTSTR GetStyleIcon()						{ return m_sStyleIcon;				}
		void SetStyleText(LPCTSTR pstrValue)		{ m_sStyleText = pstrValue;			}
		LPCTSTR GetStyleText()						{ return m_sStyleText;				}

		void SetIndent(int nWidth)					{ m_nIndent = nWidth;				}
		int  GetIndent()							{ return m_nIndent;					}

		void SetItemBkColor(DWORD dwColor)			{ m_dwItemBkColor = dwColor;		}
		DWORD GetItemBkColor()						{ return m_dwItemBkColor;			}	
			
		void SetItemSelectedBkColor(DWORD dwColor)	{ m_dwItemSelectedBkColor = dwColor; }
		DWORD GetItemSelectedBkColor()				{ return m_dwItemSelectedBkColor;	}

		void SetItemHotBkColor(DWORD dwColor)		{ m_dwItemHotBkColor = dwColor;		}
		DWORD GetItemHotBkColor()					{ return m_dwItemHotBkColor;		}

		void SetItemFocusBkColor(DWORD dwColor)		{ m_dwItemFocusBkColor = dwColor;	}
		DWORD GetItemFocusBkColor()					{ return m_dwItemFocusBkColor;		}

		void SetItemLineColor(DWORD dwColor)		{ m_dwItemLineColor = dwColor;		}
		DWORD GetItemLineColor()					{ return m_dwItemLineColor;			}

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);	

		virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl, bool bDoNotDestroy=false);
		void RemoveAll();

		virtual void DoInit();
		virtual SIZE EstimateSize(SIZE szAvailable);

		//��ȡ��һ�����ڵ�
		CTreeItemUI *GetRootNode();

		//����ڵ㣬
		//pParentNode=NULLʱ�� ���뵽root�£���root�����ڣ���ǰ����Ϊroot
		//pAfterNode=NULLʱ�����뵽pParentNode���һ���������
		CTreeItemUI *InsertNode(LPCTSTR lpszItem, CTreeItemUI *pParentNode, CTreeItemUI *pAfterNode);
		BOOL InsertNode(CTreeItemUI *pNewNode, CTreeItemUI *pParentNode, CTreeItemUI *pAfterNode);

		//ɾ���ڵ㣬������һ��ɾ���ġ�
		BOOL DeleteNode(CTreeItemUI *pNode);

		//�ж��Ƿ���ڵ� ( ���ڵ���ܲ�ֹһ�� )
		BOOL IsRootNode(CTreeItemUI *pNode);

		//���pNode����һ�����������������
		CTreeItemUI *GetNextNode(CTreeItemUI *pNode);

		//���pNode��һ������
		CTreeItemUI *GetFirstChildren(CTreeItemUI *pNode);

		//���pNode����һ���ֵ�
		CTreeItemUI *GetNextSiblingNode(CTreeItemUI *pNode);

		//���pNode����һ���ֵ�
		CTreeItemUI *GetNextPrevNode(CTreeItemUI *pNode);

		//����Tag��ǲ���Node
		CTreeItemUI *FindNodeByTag(UINT_PTR pTag);

		//�ƶ��ڵ�, pNewParent=NULLʱ���ƶ������ڵ�
		BOOL MoveNode(CTreeItemUI *pNode, CTreeItemUI *pNewParent, CTreeItemUI *pAfterNode);
	private:
		CTreeItemUI *m_pFocusNode;

		CDuiString m_sStyleHoriz;
		CDuiString m_sStyleFolder;
		CDuiString m_sStyleCheckBox;
		CDuiString m_sStyleIcon;
		CDuiString m_sStyleText;

		int m_nIndent;

		DWORD m_dwItemBkColor;
		DWORD m_dwItemSelectedBkColor;
		DWORD m_dwItemHotBkColor;
		DWORD m_dwItemFocusBkColor;
		DWORD m_dwItemLineColor;
	};
}


#endif // UITreeViewEx_h__
