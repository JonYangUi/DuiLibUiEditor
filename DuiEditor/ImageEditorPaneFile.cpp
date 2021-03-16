// ImageEditorFrame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorPaneFile.h"

#include "UIManager.h"
#include "ImageEditor.h"
#include "ImageEditorFrame.h"

BEGIN_MESSAGE_MAP(CImageEditorListCtrl, CMFCShellListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CImageEditorListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_CLICK, &CImageEditorListCtrl::OnNMClick)
END_MESSAGE_MAP()

CImageEditorListCtrl::CImageEditorListCtrl()
{
	
}

CImageEditorListCtrl::~CImageEditorListCtrl()
{
	ClearThumbnail();
}

void CImageEditorListCtrl::OnSetColumns()
{
	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		DeleteColumn(0);
	}

	const TCHAR* szName [] = {
		_T("�ļ�"), _T("��С"), _T("����"), _T("�޸�ʱ��"), };

		for (int iColumn = 0; iColumn < 4; iColumn++)
		{
			int nFormat = (iColumn == AFX_ShellList_ColumnSize) ? LVCFMT_RIGHT : LVCFMT_LEFT;

			InsertColumn(iColumn, szName [iColumn], nFormat, 100, iColumn);
		}
}

void CImageEditorListCtrl::DoDefault(int iItem)
{
	LVITEM lvItem;

	ZeroMemory(&lvItem, sizeof(lvItem));
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = iItem;

	if (!GetItem(&lvItem))
	{
		return;
	}

	LPAFX_SHELLITEMINFO pInfo = (LPAFX_SHELLITEMINFO) lvItem.lParam;
	if (pInfo == NULL || pInfo->pParentFolder == NULL || pInfo->pidlRel == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	IShellFolder *psfFolder = pInfo->pParentFolder;
	if (psfFolder == NULL)
	{
		HRESULT hr = SHGetDesktopFolder(&psfFolder);
		if (FAILED(hr))
		{
			ASSERT(FALSE);
			return;
		}
	}
	else
	{
		psfFolder->AddRef();
	}

	if (psfFolder == NULL)
	{
		return;
	}

	// If specified element is a folder, try to display it:
	ULONG ulAttrs = SFGAO_FOLDER;
	psfFolder->GetAttributesOf(1, (const struct _ITEMIDLIST **) &pInfo->pidlRel, &ulAttrs);

	if (ulAttrs & SFGAO_FOLDER)
	{
		DisplayFolder(pInfo);
	}
	else
	{
		CString strPath;
		if(GetItemPath(strPath, iItem))
		{
			SHFILEINFO sfi;
			SHGetFileInfo((LPCTSTR)pInfo->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME);
			int x = 0;
			CImageEditorFrame *pFrame = (CImageEditorFrame *)GetParent()->GetParent();
			pFrame->OnSelectedFile(strPath);
		}
	}

	psfFolder->Release();
}

void CImageEditorListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = GetNextItem(-1, LVNI_FOCUSED);
	if (nItem != -1)
	{
		DoDefault(nItem);
	}

	*pResult = 0;
}

void CImageEditorListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//*pResult = CDRF_DODEFAULT; //�ؼ��Լ�����
	//return;
	if((GetStyle () & LVS_TYPEMASK) != LVS_ICON)
		return;

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;

	// Request item-specific notifications if this is the
	// beginning of the paint cycle.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )	//�ؼ���ʼ����ǰ
	{
		*pResult = CDRF_NOTIFYITEMDRAW; //���ظ��ؼ������Ҫ�Լ���
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage ) //�ؼ���ʼ����Itemǰ
	{
		// This is the beginning of an item's paint cycle.
		LVITEM   rItem;
		int      nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		CDC*     pDC   = CDC::FromHandle ( pLVCD->nmcd.hdc );
		BOOL     bListHasFocus;
		CRect    rcItem;
		CRect    rcText;
		CString  sText;
		UINT     uFormat;

		//bListHasFocus = ( m_agentList.GetSafeHwnd() == ::GetFocus() );
		//bListHasFocus = ( GetSafeHwnd() == ::GetFocus() );
		bListHasFocus = TRUE;

		// Get the image index and selected/focused state of the
		// item being drawn.
		ZeroMemory ( &rItem, sizeof(LVITEM) );
		rItem.mask  = LVIF_IMAGE | LVIF_STATE;
		rItem.iItem = nItem;
		rItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		//m_agentList.GetItem ( &rItem );
		GetItem ( &rItem );

		CRect rcBounds;
		GetItemRect ( nItem, rcBounds, LVIR_BOUNDS );
		if(rItem.state & LVIS_SELECTED)
		{
			pDC->FillSolidRect(rcBounds, RGB(0xe6,0xe6,0xe6));
		}

		// Get the rect that holds the item's icon.
		//m_agentList.GetItemRect ( nItem, &rcItem, LVIR_ICON );
		GetItemRect ( nItem, &rcItem, LVIR_ICON );

		// Draw the icon.
		uFormat = ILD_TRANSPARENT;

		if ( ( rItem.state & LVIS_SELECTED ) && bListHasFocus )
			uFormat |= ILD_FOCUS;

		//��ͼ�꣬���CxImage���ܼ��صģ�����ϵͳĬ��ͼ����ʾ
		CRect rcIcon = rcItem;
		rcIcon.left += 5;
		rcIcon.right -= 5;
		CxImage *pImg = GetThumbnail(nItem);
		if(pImg)
		{
			CRect rcImage;
			CRect rcTemp(0, 0, pImg->GetWidth(), pImg->GetHeight());
			if( rcTemp.Width() < rcIcon.Width())
			{
				CPoint pt = rcItem.CenterPoint();
				rcImage.left = pt.x - rcTemp.Width()/2;
				rcImage.right = pt.x + rcTemp.Width()/2;
			}
			else
			{
				rcImage.left = rcIcon.left;
				rcImage.right = rcIcon.right;
			}
			if(rcTemp.Height() < rcIcon.Height())
			{
				CPoint pt = rcItem.CenterPoint();
				rcImage.top = pt.y - rcTemp.Height()/2;
				rcImage.bottom = pt.y + rcTemp.Height()/2;
			}
			else
			{
				rcImage.top = rcIcon.top;
				rcImage.bottom = rcIcon.bottom;
			}

			pImg->Draw(pDC->m_hDC, rcImage);
		}
		else
		{
			IMAGEINFO info;
			if(m_imageShell.GetImageInfo(nItem, &info))
			{
				CRect rcImage = info.rcImage;
				CRect rcTemp = rcImage;
				CPoint pt = rcItem.CenterPoint();
				rcImage.left = pt.x - rcTemp.Width()/2;
				rcImage.right = pt.x + rcTemp.Width()/2;
				rcImage.top = pt.y - rcTemp.Height()/2;
				rcImage.bottom = pt.y + rcTemp.Height()/2;

				m_imageShell.Draw ( pDC, rItem.iImage, rcImage.TopLeft(), uFormat );
			}
			else
				m_imageShell.Draw ( pDC, rItem.iImage, rcItem.TopLeft(), uFormat );
		}


		// Get the rect that bounds the text label.
		//m_agentList.GetItemRect ( nItem, rcItem, LVIR_LABEL );
		GetItemRect ( nItem, rcItem, LVIR_LABEL );

		pDC->SetBkMode ( TRANSPARENT );

		// Tweak the rect a bit for nicer-looking text alignment.
		rcText = rcItem;
		// Draw the text.
		//sText = m_agentList.GetItemText ( nItem, 0 );
		sText = GetItemText ( nItem, 0 );

		pDC->DrawText ( sText, CRect::CRect(rcText.left+3,rcText.top,rcText.right,rcText.bottom+60), DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL );



		// Draw a focus rect around the item if necessary.
		if ( bListHasFocus && ( rItem.state & LVIS_FOCUSED ))
		{
			pDC->DrawFocusRect ( rcIcon );
		}

		*pResult = CDRF_SKIPDEFAULT;    // We've painted everything. ���ظ��ؼ������Լ����ƺ��ˣ��ؼ�����
	}
	
}


HRESULT CImageEditorListCtrl::DisplayFolder(LPCTSTR lpszPath)
{
	return __super::DisplayFolder(lpszPath);
}

HRESULT CImageEditorListCtrl::DisplayFolder(LPAFX_SHELLITEMINFO lpItemInfo)
{
	HRESULT hr = __super::DisplayFolder(lpItemInfo);
	if(!SUCCEEDED(hr))	return hr;

	ClearThumbnail();
	Sort(0);
	return hr;
}

BOOL CImageEditorListCtrl::InitList()
{
	//Ū���ٵ�ImageList��ListCtrl���Ӷ���ͼ���������ˡ�
	m_imageEmpty.Create(60, 60, ILC_COLOR32 | ILC_MASK, 1, 1);
	SetImageList(&m_imageEmpty, LVSIL_NORMAL);

	//SetIconSpacing(100,100);
	m_imageShell.Attach(GetShellImageList(TRUE));
	return TRUE;
}

CxImage *CImageEditorListCtrl::GetThumbnail(int nItem)
{
	std::map<int, CxImage *>::iterator it = m_map.find(nItem);
	if(it != m_map.end())
	{
		return it->second;
	}
	else
	{
		CString strPath;
		GetItemPath(strPath, nItem);
		CxImage img;
		if(img.Load(strPath))
		{
			CxImage *pImg = new CxImage;
			pImg->Transfer(img);
			m_map[nItem] = pImg;
			return pImg;
		}
		else //���Խ���svg
		{
			if(CImageEditor::svg_2_cximage(img, strPath))
			{
				CxImage *pImg = new CxImage;
				pImg->Transfer(img);
				m_map[nItem] = pImg;
				return pImg;
			}		
		}
	}
	return NULL;
}

void CImageEditorListCtrl::ClearThumbnail()
{
	std::map<int,CxImage*>::iterator it;
	for(it=m_map.begin(); it!=m_map.end(); it++)
	{
		delete it->second;
	}
	m_map.clear();
}

//////////////////////////////////////////////////////////////////////////
/*
//CImageEditorListToolBarMenuButton
IMPLEMENT_SERIAL(CImageEditorListToolBarMenuButton, CMFCToolBarMenuButton, 1)
CImageEditorListToolBarMenuButton::CImageEditorListToolBarMenuButton(HMENU hMenu) 
	: CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
{
}
CImageEditorListToolBarMenuButton::CImageEditorListToolBarMenuButton(UINT uiID, HMENU hMenu, int iImage, LPCTSTR lpszText, BOOL bUserButton)
	: CMFCToolBarMenuButton(uiID, hMenu, iImage, lpszText, bUserButton)
{
}

BOOL CImageEditorListToolBarMenuButton::OpenPopupMenu(CWnd* pWnd)
{
	//pWnd->SendMessage(WM_CONTEXTMENU, 0, 0);
	CPoint point;
	if(!GetCursorPos(&point))	return TRUE;
	//ScreenToClient(&point); 

	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_EXPLORER_VIEWS);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, pWnd, TRUE);
	return TRUE;
}
*/
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CImageEditorPaneFile, CDockablePane)

CImageEditorPaneFile::CImageEditorPaneFile()
{

}

CImageEditorPaneFile::~CImageEditorPaneFile()
{
}


BEGIN_MESSAGE_MAP(CImageEditorPaneFile, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EXPLORER_UP, OnExplorerUp)
	ON_COMMAND(ID_EXPLORER_BACK, &CImageEditorPaneFile::OnExplorerBack)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_BACK, &CImageEditorPaneFile::OnUpdateExplorerBack)
	ON_COMMAND(ID_EXPLORER_FORWARD, &CImageEditorPaneFile::OnExplorerForward)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_FORWARD, &CImageEditorPaneFile::OnUpdateExplorerForward)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_UP, &CImageEditorPaneFile::OnUpdateExplorerUp)
	ON_COMMAND(ID_EXPLORER_VIEWS_LARGE_ICON, &CImageEditorPaneFile::OnExplorerViewsLargeIcon)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_LARGE_ICON, &CImageEditorPaneFile::OnUpdateExplorerViewsLargeIcon)
	ON_COMMAND(ID_EXPLORER_VIEWS_SMALL_ICON, &CImageEditorPaneFile::OnExplorerViewsSmallIcon)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_SMALL_ICON, &CImageEditorPaneFile::OnUpdateExplorerViewsSmallIcon)
	ON_COMMAND(ID_EXPLORER_VIEWS_DETAILS, &CImageEditorPaneFile::OnExplorerViewsDetails)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_DETAILS, &CImageEditorPaneFile::OnUpdateExplorerViewsDetails)
	ON_COMMAND(ID_EXPLORER_VIEWS_LIST, &CImageEditorPaneFile::OnExplorerViewsList)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_LIST, &CImageEditorPaneFile::OnUpdateExplorerViewsList)
END_MESSAGE_MAP()

// CViewControlWnd ��Ϣ�������
int CImageEditorPaneFile::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	m_wndList.Create (WS_CHILD | WS_VISIBLE | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_REPORT, rectDummy, this, IDC_SHELL_LIST_CTRL);
	m_wndList.EnableShellContextMenu(FALSE);
	m_wndList.SetColumnWidth(0, 200);

	DWORD dwStyle = m_wndList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_wndList.SetExtendedStyle(dwStyle); //������չ���

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TB_IMAGE_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_TB_IMAGE_EXPLORER, 0, 0, TRUE /* ������*/);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);
	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_EXPLORER_BACK));
	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_EXPLORER_FORWARD));
	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_EXPLORER_UP));

	m_wndList.InitList();


	CString strSkinDir = g_pEditorImage->GetUIManager()->GetDocument()->GetSkinPath();
	CString strPathName = strSkinDir + XML2T(g_pEditorImage->m_nodedata.attribute(XTEXT("file")).value());

	BOOL bGoToPath = FALSE;
	TCHAR szPath[256];
	_tcscpy_s(szPath, 256, strPathName);
	if(PathRemoveFileSpec(szPath))
	{
		HRESULT hr = m_wndList.DisplayFolder(szPath);
		if(SUCCEEDED(hr))
		{
			for (int i=0; i<m_wndList.GetItemCount(); i++)
			{
				CString temp;
				if(m_wndList.GetItemPath(temp, i))
				{
					if(temp == strPathName)
					{
						//.....ûЧ����
						m_wndList.SetItemState(i, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
						bGoToPath = TRUE;
						break;
					}
				}
			}
		}
	}
	
	if(!bGoToPath)
		m_wndList.DisplayFolder(strSkinDir);
	return 0;
}

void CImageEditorPaneFile::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	//m_wndList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndList.Refresh();
}


void CImageEditorPaneFile::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴������Ϣ����������
	m_wndList.SetFocus();
}


void CImageEditorPaneFile::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_EXPLORER);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, TRUE);
}

void CImageEditorPaneFile::OnExplorerBack()
{
	// TODO: �ڴ���������������
}


void CImageEditorPaneFile::OnUpdateExplorerBack(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CImageEditorPaneFile::OnExplorerForward()
{
	// TODO: �ڴ���������������
}


void CImageEditorPaneFile::OnUpdateExplorerForward(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CImageEditorPaneFile::OnExplorerUp()
{
	m_wndList.DisplayParentFolder();
}

void CImageEditorPaneFile::OnUpdateExplorerUp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_wndList.IsDesktop());
}


void CImageEditorPaneFile::OnExplorerViewsLargeIcon()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsLargeIcon(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_ICON);
}


void CImageEditorPaneFile::OnExplorerViewsSmallIcon()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsSmallIcon(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_SMALLICON);
}


void CImageEditorPaneFile::OnExplorerViewsDetails()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsDetails(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_REPORT);
}


BOOL CImageEditorPaneFile::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: �ڴ����ר�ô����/����û���
// 	if(nCode == CN_UPDATE_COMMAND_UI)
// 	{
// 		CString str;
// 		str.Format(_T("id=%d"), nID);
// 		InsertMsg(str);
// 	}
	return CDockablePane::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CImageEditorPaneFile::OnExplorerViewsList()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsList(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_LIST);
}

