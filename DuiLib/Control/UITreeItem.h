#ifndef UITreeNodeEx_h__
#define UITreeNodeEx_h__

#pragma once

namespace DuiLib
{
	//Tree�ڲ��ĵ�Option�ؼ�
	class UILIB_API CTreeInnerCheckBoxUI : public CCheckBoxUI
	{
	public:
		enum eType { BTN_FOLDER, BTN_CHECKBOX, BTN_ICON, BTN_TEXT };
		CTreeInnerCheckBoxUI();

		void SetOwner(CControlUI *pCellUI) { m_pOwner = pCellUI; }
		CControlUI *GetOwner() const { return m_pOwner; }

		virtual bool Activate() override;
		virtual bool IsSelected() const override;
		virtual void Selected(bool bSelected, bool bTriggerEvent=true) override;

		virtual CDuiString GetText() const override;

		virtual void DoEvent(TEventUI& event) override;

		CControlUI *m_pOwner;
		eType m_type;
	};

	class UILIB_API CTreeItemUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CTreeItemUI)
	public:
		CTreeItemUI();
		~CTreeItemUI(void);

		void SetOwner(ITreeUI *pGrid) { m_pOwner = pGrid; }
		ITreeUI *GetOwner() const { return m_pOwner; }

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		void SetNodeData(TNodeData *pNodeData);
		TNodeData *GetNodeData() const { return m_pNodeData; }

		virtual void DoInit() override;
		int GetCxNeeded(SIZE szAvailable);
		virtual void DoEvent(TEventUI& event) override;
	protected:
		ITreeUI *m_pOwner;
		TNodeData *m_pNodeData;
	
		CTreeInnerCheckBoxUI			*m_pFolderButton;	//չ��������ť
		CTreeInnerCheckBoxUI			*m_pCheckBox;		//ѡ�а�ť
		CTreeInnerCheckBoxUI			*m_pIcon;			//ͼ��
		CTreeInnerCheckBoxUI			*m_pText;			//�ı���
	};
}


#endif // UITreeNodeEx_h__
