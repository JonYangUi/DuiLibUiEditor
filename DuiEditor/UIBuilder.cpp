#include "StdAfx.h"
#include "DuiEditor.h"
#include "UIBuilder.h"

#include "DlgCustomControl.h"

CUIBuilder::CUIBuilder()  : m_pCallback(NULL), m_pstrtype(NULL)
{
	m_instance = NULL;
	m_bOpenConfigFile = false;
}


CUIBuilder::~CUIBuilder(void)
{
}
/*
CControlUI* CUIBuilder::Create(STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback,
	CPaintManagerUI* pManager, CControlUI* pParent)
{
	//��ԴIDΪ0-65535�������ֽڣ��ַ���ָ��Ϊ4���ֽ�
	//�ַ�����<��ͷ��Ϊ��XML�ַ�����������Ϊ��XML�ļ�
	if(HIWORD(xml.m_lpstr) != NULL && *(xml.m_lpstr) != _T('<')) {
		LPCTSTR xmlpath = CResourceManager::GetInstance()->GetXmlPath(xml.m_lpstr);
		if (xmlpath != NULL) {
			xml = xmlpath;
		}
	}

	if( HIWORD(xml.m_lpstr) != NULL ) {
		if( *(xml.m_lpstr) == _T('<') ) {
			if( !m_xml.load_string(xml.m_lpstr, XML_PARSER_OPTIONS) ) return NULL;
		}
		else {
			if( !m_xml.load_file(xml.m_lpstr, XML_PARSER_OPTIONS) ) return NULL;
		}
	}
	else {
		HINSTANCE dll_instence = NULL;
		if (m_instance)
		{
			dll_instence = m_instance;
		}
		else
		{
			dll_instence = CPaintManagerUI::GetResourceDll();
		}
		HRSRC hResource = ::FindResource(dll_instence, xml.m_lpstr, type);
		if( hResource == NULL ) return NULL;
		HGLOBAL hGlobal = ::LoadResource(dll_instence, hResource);
		if( hGlobal == NULL ) {
			FreeResource(hResource);
			return NULL;
		}

		m_pCallback = pCallback;
		if( !m_xml.load_buffer((BYTE*)::LockResource(hGlobal), ::SizeofResource(dll_instence, hResource) )) return NULL;
		::FreeResource(hResource);
		m_pstrtype = type;
	}

	return Create(m_xml, pCallback, pManager, pParent);
}
*/
CControlUI* CUIBuilder::Create(pugi::xml_node xmldoc, IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
{
	m_pCallback = pCallback;
	//pugi::xml_node root = xmldoc.child(_T("Window"));
	//if( !root) return NULL;

	pugi::xml_node root;
	for (xml_node node=xmldoc.first_child(); node; node=node.next_sibling())
	{
		if(node.type() == node_element) //�ҵ���һ���ڵ�, ��������Window�ڵ�, ��Ϊ��д��Include�Ӵ��ڲ���window�ڵ�
		{
			root = node;
			break;
		}
	}
	if( !root) return NULL;

	if(!g_proj.IsLoadedConfig())
	{
		g_proj.LoadConfig(pManager);
	}

	if( pManager ) {
		CString pstrClass = NULL;
		int nAttributes = 0;
		CString pstrName = NULL;
		CString pstrValue = NULL;
		LPTSTR pstr = NULL;
		for( xml_node node = root.first_child() ; node; node = node.next_sibling() ) {
			if(node.type() != node_element)	continue;
			pstrClass = XML2T(node.name());
			if( _tcsicmp(pstrClass, _T("Image")) == 0 ) {
				CString pImageName = NULL;
				CString pImageResType = NULL;
				bool shared = false;
				DWORD mask = 0;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = XML2T(attr.name());
					pstrValue = XML2T(attr.value());
					if( _tcsicmp(pstrName, _T("name")) == 0 ) {
						pImageName = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("restype")) == 0 ) {
						pImageResType = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("mask")) == 0 ) {
						if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
						mask = _tcstoul(pstrValue, &pstr, 16);
					}
					else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
						shared = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
				}
				if( pImageName ) pManager->AddImage(pImageName, pImageResType, mask, false, shared);
			}
			else if( _tcsicmp(pstrClass, _T("Font")) == 0 ) {
				int id = -1;
				CString pFontName = NULL;
				int size = 12;
				bool bold = false;
				bool underline = false;
				bool italic = false;
				bool defaultfont = false;
				bool shared = false;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = XML2T(attr.name());
					pstrValue = XML2T(attr.value());
					if( _tcsicmp(pstrName, _T("id")) == 0 ) {
						id = _tcstol(pstrValue, &pstr, 10);
					}
					else if( _tcsicmp(pstrName, _T("name")) == 0 ) {
						pFontName = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("size")) == 0 ) {
						size = _tcstol(pstrValue, &pstr, 10);
					}
					else if( _tcsicmp(pstrName, _T("bold")) == 0 ) {
						bold = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
					else if( _tcsicmp(pstrName, _T("underline")) == 0 ) {
						underline = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
					else if( _tcsicmp(pstrName, _T("italic")) == 0 ) {
						italic = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
					else if( _tcsicmp(pstrName, _T("default")) == 0 ) {
						defaultfont = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
					else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
						shared = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
				}
				if( id >= 0 ) {
					pManager->AddFont(id, pFontName, size, bold, underline, italic, shared);
					if( defaultfont ) pManager->SetDefaultFont(pFontName, pManager->GetDPIObj()->Scale(size), bold, underline, italic, shared);
				}
			}
			else if( _tcsicmp(pstrClass, _T("Default")) == 0 ) {
				CString pControlName = NULL;
				CString pControlValue = NULL;
				bool shared = false;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = XML2T(attr.name());
					pstrValue = XML2T(attr.value());
					if( _tcsicmp(pstrName, _T("name")) == 0 ) {
						pControlName = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("value")) == 0 ) {
						pControlValue = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
						shared = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
				}
				if( pControlName ) {
					pManager->AddDefaultAttributeList(pControlName, pControlValue, shared);
				}
			}
			else if( _tcsicmp(pstrClass, _T("Style")) == 0 ) {
				CString pName = NULL;
				CString pStyle = NULL;
				bool shared = false;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = XML2T(attr.name());
					pstrValue = XML2T(attr.value());
					if( _tcsicmp(pstrName, _T("name")) == 0 ) {
						pName = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("value")) == 0 ) {
						pStyle = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
						shared = (_tcsicmp(pstrValue, _T("true")) == 0);
					}
				}
				if( pName ) {
					pManager->AddStyle(pName, pStyle, shared);
				}
			}
			else if (_tcsicmp(pstrClass, _T("Import")) == 0) {
				CString pstrPath = NULL;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = XML2T(attr.name());
					pstrValue = XML2T(attr.value());
					if (_tcsicmp(pstrName, _T("fontfile")) == 0) {
						pstrPath = pstrValue;
					}
				}
				if (pstrPath) {
					pManager->AddFontArray(pstrPath);
				}
			}
		}

		pstrClass = XML2T(root.name());
		if( _tcsicmp(pstrClass, _T("Window")) == 0 ) 
		{
			if( pManager->GetPaintWindow()  && !pManager->IsInitWindowParameter()) //modify by liqs99
			{
				SetWindowAttribute(pManager, root);
				pManager->SetInitWindowParameter(true); //modify by liqs99
			}
		}
	}

	return _Parse(root, pParent, pManager);
}

void CUIBuilder::SetWindowAttribute(CPaintManagerUI *pManager, xml_node node)
{
	if( !pManager->GetPaintWindow() ) return;

	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		CString pstrName = XML2T(attr.name());
		CString pstrValue = XML2T(attr.value());

		if( _tcsicmp(pstrName, _T("size")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetInitSize(pManager->GetDPIObj()->Scale(cx), pManager->GetDPIObj()->Scale(cy));
		} 
		else if( _tcsicmp(pstrName, _T("sizebox")) == 0 ) {
			RECT rcSizeBox = { 0 };
			LPTSTR pstr = NULL;
			rcSizeBox.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			pManager->SetSizeBox(rcSizeBox);
		}
		else if( _tcsicmp(pstrName, _T("caption")) == 0 ) {
			RECT rcCaption = { 0 };
			LPTSTR pstr = NULL;
			rcCaption.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			pManager->SetCaptionRect(rcCaption);
		}
		else if( _tcsicmp(pstrName, _T("roundcorner")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetRoundCorner(cx, cy);
		} 
		else if( _tcsicmp(pstrName, _T("mininfo")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetMinInfo(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("maxinfo")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetMaxInfo(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("showdirty")) == 0 ) {
			pManager->SetShowUpdateRect(_tcsicmp(pstrValue, _T("true")) == 0);
		} 
		else if( _tcsicmp(pstrName, _T("opacity")) == 0 || _tcsicmp(pstrName, _T("alpha")) == 0 ) {
			pManager->SetOpacity(_ttoi(pstrValue));
		} 
		else if( _tcscmp(pstrName, _T("layeredopacity")) == 0 ) {
			pManager->SetLayeredOpacity(_ttoi(pstrValue));
		} 
		else if( _tcscmp(pstrName, _T("layered")) == 0 || _tcscmp(pstrName, _T("bktrans")) == 0) {
			pManager->SetLayered(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("layeredimage")) == 0 ) {
			pManager->SetLayered(true);
			pManager->SetLayeredImage(pstrValue);
		} 
		else if( _tcscmp(pstrName, _T("noactivate")) == 0 ) {
			pManager->SetNoActivate(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcsicmp(pstrName, _T("disabledfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultDisabledColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("defaultfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultFontColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("linkfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultLinkFontColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("linkhoverfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultLinkHoverFontColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("selectedcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultSelectedBkColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("shadowsize")) == 0 ) {
			pManager->GetShadow()->SetSize(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("shadowsharpness")) == 0 ) {
			pManager->GetShadow()->SetSharpness(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("shadowdarkness")) == 0 ) {
			pManager->GetShadow()->SetDarkness(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("shadowposition")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->GetShadow()->SetPosition(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("shadowcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->GetShadow()->SetColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("shadowcorner")) == 0 ) {
			RECT rcCorner = { 0 };
			LPTSTR pstr = NULL;
			rcCorner.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcCorner.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcCorner.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcCorner.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			pManager->GetShadow()->SetShadowCorner(rcCorner);
		}
		else if( _tcsicmp(pstrName, _T("shadowimage")) == 0 ) {
			pManager->GetShadow()->SetImage(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("showshadow")) == 0 ) {
			pManager->GetShadow()->ShowShadow(_tcsicmp(pstrValue, _T("true")) == 0);
		} 
		else if( _tcsicmp(pstrName, _T("gdiplustext")) == 0 ) {
			pManager->SetUseGdiplusText(_tcsicmp(pstrValue, _T("true")) == 0);
		} 
		else if( _tcsicmp(pstrName, _T("textrenderinghint")) == 0 ) {
			pManager->SetGdiplusTextRenderingHint(_ttoi(pstrValue));
		} 
		else if( _tcsicmp(pstrName, _T("tooltiphovertime")) == 0 ) {
			pManager->SetHoverTime(_ttoi(pstrValue));
		} 
	}
}


CControlUI* CUIBuilder::_Parse(pugi::xml_node root,  CControlUI* pParent, CPaintManagerUI* pManager)
{
	IContainerUI* pContainer = NULL;
	CControlUI* pReturn = NULL;
	for( xml_node node = root.first_child() ; node; node = node.next_sibling() ) 
	{
		if(node.type() != node_element)	
			continue;

		CString pstrClass = XML2T(node.name());
		if( _tcsicmp(pstrClass, _T("Image")) == 0 || _tcsicmp(pstrClass, _T("Font")) == 0 \
			|| _tcsicmp(pstrClass, _T("Default")) == 0 || _tcsicmp(pstrClass, _T("Style")) == 0 ) continue;

		CControlUI* pControl = NULL;
		if (_tcsicmp(pstrClass, _T("Import")) == 0) continue;
		if( _tcsicmp(pstrClass, _T("Include")) == 0 ) {			//����Include����ʹ��ԭ����CDialogBuilder
			int count = node.attribute(XTEXT("count")).as_int(1);
			for ( int i = 0; i < count; i++ ) {
				CDialogBuilder builder;
				if( m_pstrtype != NULL ) { // ʹ����Դdll������Դ�ж�ȡ
					WORD id = node.attribute(XTEXT("source")).as_int(); 
					pControl = builder.Create((UINT)id, m_pstrtype, m_pCallback, pManager, pParent);
				}
				else {
					CString szValue = XML2T(node.attribute(XTEXT("source")).value());
					pControl = builder.Create((LPCTSTR)szValue, (UINT)0, m_pCallback, pManager, pParent);
				}
			}

			if(pControl)
			{
				pControl->SetTag((UINT_PTR)node.internal_object());
				node.set_tag((UINT_PTR)pControl);
			}
			continue;
		}
		else {
			//pControl = CreateControl(node);	//�ŵ�����ȥ�����ȴӻص�����

			// �����
			if( pControl == NULL ) {
				CStdPtrArray* pPlugins = CPaintManagerUI::GetPlugins();
				LPCREATECONTROL lpCreateControl = NULL;
				for( int i = 0; i < pPlugins->GetSize(); ++i ) {
					lpCreateControl = (LPCREATECONTROL)pPlugins->GetAt(i);
					if( lpCreateControl != NULL ) {
						pControl = lpCreateControl(pstrClass);
						if( pControl != NULL ) break;
					}
				}
			}
			// �ص�����
			if( pControl == NULL && m_pCallback != NULL ) {
				pControl = m_pCallback->CreateControl(pstrClass);
			}

			if(pControl == NULL)
			{
				pControl = CreateControl(node);	//���ȴӻص�����
			}

		}

		if( pControl == NULL ) {
#ifdef _DEBUG
			DUITRACE(_T("δ֪�ؼ�:%s"), pstrClass);
			continue;
#else
			continue;
#endif
		}
	
		pControl->SetTag((UINT_PTR)node.internal_object());
		node.set_tag((UINT_PTR)pControl);

		// Add children
		if( node.first_child() ) {
			_Parse(node, pControl, pManager);
		}
		// Attach to parent
		// ��ΪĳЩ���Ժ͸�������أ�����selected��������Add��������
		CTreeViewUI* pTreeView = NULL;
		if( pParent != NULL && pControl != NULL ) {
			CTreeNodeUI* pParentTreeNode = static_cast<CTreeNodeUI*>(pParent->GetInterface(_T("TreeNode")));
			CTreeNodeUI* pTreeNode = static_cast<CTreeNodeUI*>(pControl->GetInterface(_T("TreeNode")));
			pTreeView = static_cast<CTreeViewUI*>(pParent->GetInterface(_T("TreeView")));
			// TreeNode�ӽڵ�
			if(pTreeNode != NULL) {
				if(pParentTreeNode) {
					pTreeView = pParentTreeNode->GetTreeView();
					if(!pParentTreeNode->Add(pTreeNode)) {
						delete pTreeNode;
						pTreeNode = NULL;
						continue;
					}
				}
				else {
					if(pTreeView != NULL) {
						if(!pTreeView->Add(pTreeNode)) {
							delete pTreeNode;
							pTreeNode = NULL;
							continue;
						}
					}
				}
			}
			// TreeNode�ӿؼ�
			else if(pParentTreeNode != NULL) {
				pParentTreeNode->GetTreeNodeHoriznotal()->Add(pControl);
			}
			// ��ͨ�ؼ�
			else {
				if( pContainer == NULL ) pContainer = static_cast<IContainerUI*>(pParent->GetInterface(_T("IContainer")));
				//ASSERT(pContainer);
				if( pContainer == NULL ) return NULL;
				if( !pContainer->Add(pControl) ) 
				{
					delete pControl;
					pControl = NULL;
					continue;
				}
			}
		}
		if( pControl == NULL ) continue;

		// Init default attributes
		if( pManager ) {
			if(pTreeView != NULL) {
				pControl->SetManager(pManager, pTreeView, true);
			}
			else {
				pControl->SetManager(pManager, NULL, false);
			}
			LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
			if( pDefaultAttributes ) {
				pControl->ApplyAttributeList(pDefaultAttributes);
			}
		}
		// Process attributes
		for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
		{
			const char *pValue = attr.value();
			if(pValue && *pValue != '\0')
				pControl->SetAttribute(XML2T(attr.name()), XML2T(attr.value()));
		}

		if( pManager ) {
			if(pTreeView == NULL) {
				pControl->SetManager(NULL, NULL, false);
			}
		}
		// Return first item
		if( pReturn == NULL ) pReturn = pControl;
	}
	return pReturn;
}

CControlUI* CUIBuilder::CreateControl(LPCTSTR pstrClass)
{
	CControlUI *pControl;

// 	if(CompareString(pstrClass, _T("Menu")))
// 	{
// 		pControl = new CMenuExUI;
// 		return pControl;
// 	}
// 
// 	if(CompareString(pstrClass, _T("MenuElement")))
// 	{
// 		pControl = new CMenuElementExUI;
// 		return pControl;
// 	}

	CDuiString strClass;
	strClass.Format(_T("C%sUI"), pstrClass);
	pControl = dynamic_cast<CControlUI*>(CControlFactory::GetInstance()->CreateControl(strClass));

	return pControl;
}

CControlUI* CUIBuilder::CreateControl(xml_node node)
{
	CControlUI *pControl = NULL;
	pControl = CreateControl(XML2T(node.name()));
	if(!pControl)
	{
		xml_attribute attr = node.attribute(XTEXT("custombasedfrom"));
		if(attr)
		{
			pControl = CreateControl(XML2T(attr.value()));
		}
// 		else //����ʶ�Ŀؼ���ֱ�Ӵ�CContainner����, ����������Ŷ
// 		{
// 			pControl = CreateControl(DUI_CTR_CONTROL);
// 		}
	}

	return pControl;
}

void CUIBuilder::DeleteControl(CControlUI *pControl)
{
	delete pControl;
}
//////////////////////////////////////////////////////////////////////////
