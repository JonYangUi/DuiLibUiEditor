#include "StdAfx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(void)
{
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::InitWindow()
{
	UI_BINDCONTROL(CTreeUI, m_tree, _T("tree_main"));
	
	TNodeData* hRoot = m_tree->InsertNode(_T("Ӫ����˾"), _T("images/edtico.png"), TNODE_ROOT, TNODE_LAST);

	TNodeData* h1 = m_tree->InsertNode(_T("�з�����"), hRoot);
	TNodeData* h1_1 = m_tree->InsertNode(_T("�з���"), h1);
 	m_tree->InsertNode(_T("Windows"), h1_1);
	m_tree->InsertNode(_T("LinuxǶ��ʽ"), h1_1);
	m_tree->InsertNode(_T("��׿"), h1_1);
	m_tree->InsertNode(_T("WEBǰ��"), h1_1);
	TNodeData* h1_1_1 = m_tree->InsertNode(_T("�㷨"), h1_1, TNODE_FIRST);
	m_tree->InsertNode(_T("�̼�����"), h1_1, h1_1_1);

 	m_tree->InsertNode(_T("��Ʋ�"), h1);
	m_tree->InsertNode(_T("������"), h1);

	TNodeData* h2 = m_tree->InsertNode(_T("Ӫ������"), hRoot);
	m_tree->InsertNode(_T("�г���"), h2);
	m_tree->InsertNode(_T("���۲�"), h2);
	m_tree->InsertNode(_T("������"), h2);
	m_tree->InsertNode(_T("��Ӫ��"), h2);
	m_tree->InsertNode(_T("������"), h2);

	TNodeData* h3 = m_tree->InsertNode(_T("��������"), hRoot);
	m_tree->InsertNode(_T("����"), h3);

	TNodeData* h4 = m_tree->InsertNode(_T("�������"), hRoot);
	m_tree->InsertNode(_T("������"), h4);
	m_tree->InsertNode(_T("���²�"), h4);
	
	//�����ݲ���	
	TNodeData* h5 = m_tree->InsertNode(_T("N�����ݿ�ʼ"), hRoot);
	TNodeData* ha = TNODE_LAST;
	for (int a=0; a<100000; a++)
	{
		CDuiString sa;
		sa.Format(_T("a-%04d"), a);
		ha = m_tree->InsertNode(sa, h5, ha);

// 		for (int b=0; b<2; b++)
// 		{
// 			CDuiString sb;
// 			sb.Format(_T("%s  -  b-%04d"), sa.GetData(), b);
// 			TNodeData* hb = m_tree->InsertNode(sb, ha);
// 
// 			for (int c=0; c<2; c++)
// 			{
// 				CDuiString sc;
// 				sc.Format(_T("%s  -  c-%04d"), sb.GetData(), c);
// 				TNodeData* hc = m_tree->InsertNode(sc, hb);
// 
// 				for (int d=0; d<2; d++)
// 				{
// 					CDuiString sd;
// 					sd.Format(_T("%s  -  d-%04d"), sc.GetData(), d);
// 					TNodeData* hd = m_tree->InsertNode(sd, hc);
// 
// 					for (int e=0; e<2; e++)
// 					{
// 						CDuiString se;
// 						se.Format(_T("%s  -  e-%04d"), sd.GetData(), e);
// 						TNodeData* he = m_tree->InsertNode(se, hd);
// 					}
// 				}
// 			}
// 		}
	}
	
	return;
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{

}