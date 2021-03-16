#include "StdAfx.h"
#include "UIComboEx.h"
#include "UIComboEditWnd.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CComboExUI)
CComboExUI::CComboExUI(void) : m_pEditWindow(NULL), m_type(CBS_DROPDOWNLIST)
{
	m_szDropButtonSize.cx = 16;
	m_szDropButtonSize.cy = 16;
}


CComboExUI::~CComboExUI(void)
{
}

LPCTSTR CComboExUI::GetClass() const
{
	return DUI_CTR_COMBOEX;
}

LPVOID CComboExUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_COMBOEX) == 0 ) return static_cast<CComboExUI*>(this);
	return __super::GetInterface(pstrName);
}

bool CComboExUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	if(!CControlUI::DoPaint(hDC, rcPaint, pStopControl))
		return false;

	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_dbDisabledImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbDisabledImage) )
				return true;
		}
	}
	else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if( !m_dbPushedImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbPushedImage) )
				return true;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_dbHotImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbHotImage) ) 
				return true;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_dbFocusedImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbFocusedImage) )
				return true;
		}
	}

	if( !m_dbNormalImage.IsEmpty() ) {
		if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbNormalImage) )
			return true;
	}

	return true;
}

bool CComboExUI::DrawDropButtonImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify)
{
	RECT rc = m_rcItem;
	rc.top++;
	rc.bottom--;
	rc.right--;
	rc.left = rc.right - (rc.bottom - rc.top);

	SIZE sz = m_szDropButtonSize;
	RECT rcButton;
	rcButton.left = rc.left + (rc.right - rc.left)/2 - sz.cx/2;
	rcButton.right = rcButton.left + sz.cx;
	rcButton.top = rc.top + (rc.bottom - rc.top)/2 - sz.cy/2;
	rcButton.bottom = rcButton.top + sz.cy;

	return CRenderEngine::DrawImageString(hDC, m_pManager, rcButton, rcButton, pStrImage, pStrModify, m_instance);
}

CControlUI *CComboExUI::AddString(LPCTSTR pstrText, UINT_PTR ptrItemData)
{
	CListLabelElementUI *pLabel = new CListLabelElementUI;
	pLabel->SetTag(ptrItemData);
	pLabel->SetText(pstrText);
	if(!Add(pLabel))
	{
		delete pLabel;
		return NULL;
	}
	pLabel->SetFixedWidth(m_rcItem.right - m_rcItem.left);
	return pLabel;
}

bool CComboExUI::SelectString(LPCTSTR pstrText)
{
	for( int it = 0; it < GetCount(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
		if( !pControl->IsVisible() ) continue;
		
		if(pControl->GetText() == pstrText)
		{
			SelectItem(it);
			return true;
		}
	}
	return false;
}

bool CComboExUI::SetCurSel(int iIndex, bool bTakeFocus)
{
	return SelectItem(iIndex, bTakeFocus);
}

bool CComboExUI::SetCurSelFromItemData(UINT_PTR ptrItemData)
{
	for( int it = 0; it < GetCount(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
		if( !pControl->IsVisible() ) continue;

		if(pControl->GetTag() == ptrItemData)
		{
			SelectItem(it);
			return true;
		}
	}
	return false;
}

bool CComboExUI::SetItemData(CControlUI *pControl, UINT_PTR ptrItemData)
{
	pControl->SetTag(ptrItemData);
	return true;
}

UINT_PTR CComboExUI::GetCurSelItemData()
{
	if(m_iCurSel < 0) return 0;
	CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
	return pControl->GetTag();
}

void CComboExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("dropbuttonsize")) == 0 )
	{
		SIZE cx = { 0 };
		LPTSTR pstr = NULL;
		cx.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		cx.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		m_szDropButtonSize = cx;
	}
	else if( _tcsicmp(pstrName, _T("dropbuttonnormalimage")) == 0 ) SetdbNormalImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttonhotimage")) == 0 ) SetdbHotImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttonpushedimage")) == 0 ) SetdbPushedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttonfocusedimage")) == 0 ) SetdbFocusedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttondisabledimage")) == 0 ) SetdbDisabledImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("type")) == 0 ) 
	{
		if( _tcsicmp(pstrValue , _T("dropdown")) == 0)
			SetDropType(CBS_DROPDOWN);
		else
			SetDropType(CBS_DROPDOWNLIST);
	}
	else CComboUI::SetAttribute(pstrName, pstrValue);
}

LPCTSTR CComboExUI::GetdbNormalImage() const
{
	return m_dbNormalImage;
}

void CComboExUI::SetdbNormalImage(LPCTSTR pStrImage)
{
	m_dbNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbHotImage() const
{
	return m_dbHotImage;
}

void CComboExUI::SetdbHotImage(LPCTSTR pStrImage)
{
	m_dbHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbPushedImage() const
{
	return m_dbPushedImage;
}

void CComboExUI::SetdbPushedImage(LPCTSTR pStrImage)
{
	m_dbPushedImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbFocusedImage() const
{
	return m_dbFocusedImage;
}

void CComboExUI::SetdbFocusedImage(LPCTSTR pStrImage)
{
	m_dbFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbDisabledImage() const
{
	return m_dbDisabledImage;
}

void CComboExUI::SetdbDisabledImage(LPCTSTR pStrImage)
{
	m_dbDisabledImage = pStrImage;
	Invalidate();
}

int CComboExUI::GetDropType() const
{
	return m_type;
}

void CComboExUI::SetDropType(int type)
{
	m_type = type;
}

void CComboExUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN )
	{
		if(GetDropType() == CBS_DROPDOWNLIST)
			return __super::DoEvent(event);
		if(!OnLbuttonDown(event))
			return __super::DoEvent(event);

		return;
	}

	if( event.Type == UIEVENT_BUTTONUP )
	{
		if(GetDropType() == CBS_DROPDOWNLIST)
			return __super::DoEvent(event);
		if(!OnLbuttonUp(event))
			return __super::DoEvent(event);
		return;
	}

	return __super::DoEvent(event);
}

RECT CComboExUI::GetDropButtonRect()
{
	RECT rc = m_rcItem;
	rc.top++;
	rc.bottom--;
	rc.right--;
	rc.left = rc.right - (rc.bottom - rc.top);

	SIZE sz = m_szDropButtonSize;
	RECT rcButton;
	rcButton.left = rc.left + (rc.right - rc.left)/2 - sz.cx/2;
	rcButton.right = rcButton.left + sz.cx;
	rcButton.top = rc.top + (rc.bottom - rc.top)/2 - sz.cy/2;
	rcButton.bottom = rcButton.top + sz.cy;

	return rcButton;
}

bool CComboExUI::OnLbuttonDown(TEventUI& event)
{
	RECT rcButton = GetDropButtonRect();

	//���������ť
	if(::PtInRect(&rcButton, event.ptMouse))
		return false;

	if(!IsEnabled())
		return false;

	GetManager()->ReleaseCapture();
	if( IsFocused() && m_pEditWindow == NULL )
	{
		m_pEditWindow = new CComboEditWnd();
		ASSERT(m_pEditWindow);
		m_pEditWindow->Init(this);

		if( PtInRect(&m_rcItem, event.ptMouse) )
		{
			int nSize = GetWindowTextLength(*m_pEditWindow);
			if( nSize == 0 ) nSize = 1;
			Edit_SetSel(*m_pEditWindow, 0, nSize);
		}
	}
	else if( m_pEditWindow != NULL )
	{
		POINT pt = event.ptMouse;
		pt.x -= m_rcItem.left + m_rcTextPadding.left;
		pt.y -= m_rcItem.top + m_rcTextPadding.top;
		Edit_SetSel(*m_pEditWindow, 0, 0);
		::SendMessage(*m_pEditWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
	}

	return true;
}

bool CComboExUI::OnLbuttonUp(TEventUI& event)
{
	RECT rcButton = GetDropButtonRect();

	//���������ť
	if(::PtInRect(&rcButton, event.ptMouse))
		return false;

	return true;
}

bool CComboExUI::Activate()
{
	if( !CControlUI::Activate() ) return false;
// 	if(GetDropType() == CBS_DROPDOWNLIST)
// 	{
		return __super::Activate();
//	}
	return true;
}

void CComboExUI::PaintText(HDC hDC)
{
	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

	rc.right -= m_szDropButtonSize.cx + 2;

	CDuiString sText = GetText();
	if( sText.IsEmpty() ) return;
	int nLinks = 0;
	if( IsEnabled() ) {
		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, m_dwTextColor, NULL, NULL, nLinks, m_iFont, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, m_iFont, m_uTextStyle);
	}
	else {
		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, NULL, NULL, nLinks, m_iFont, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, m_iFont, m_uTextStyle);
	}
}

}