// DlgBuildLanguagePackageEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgBuildLanguagePackageEx.h"
#include "afxdialogex.h"

#include "UIManager.h"
// CDlgBuildLanguagePackageEx �Ի���

IMPLEMENT_DYNAMIC(CDlgBuildLanguagePackageEx, CDialogEx)

CDlgBuildLanguagePackageEx::CDlgBuildLanguagePackageEx(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBuildLanguagePackageEx::IDD, pParent)
{

}

CDlgBuildLanguagePackageEx::~CDlgBuildLanguagePackageEx()
{
}

void CDlgBuildLanguagePackageEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbLang);
}


BEGIN_MESSAGE_MAP(CDlgBuildLanguagePackageEx, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgBuildLanguagePackageEx::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBuildLanguagePackageEx ��Ϣ�������


BOOL CDlgBuildLanguagePackageEx::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rectDummy;
	if (!sci.Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, rectDummy, this, ID_SCI_WND))
	{
		AfxMessageBox(_T("create SciLexer window error."));
		return -1;      // δ�ܴ���
	}
	sci.InitXML();
	sci.sci_SetMargins(0);
	sci.sci_UsePopup(FALSE);

	CRect rcClient, rc;
	GetClientRect(rcClient);
	GetDlgItem(IDC_STATIC_WND)->GetWindowRect(rc);
	sci.MoveWindow(0,0,rcClient.Width(), rc.Height());

	xml_document xml;
	xml.append_child(node_comment).set_value(T2XML(_T(" ����ӿ����ɵ����԰����������� ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" id���Զ�Ӧ�ڿؼ��ϵ�resourceid ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" value���Զ�Ӧ������ص��ı� ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" resourceid �������ά������, ������������Ϊ��Դid�ؼ��ֽ����ı� ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" Ϊÿ��xml����һ�����԰� ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" ��������Ǵ����԰��ļ���������д�� ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" �����ñ��ݣ� ")));

	xml_node nodeRes = xml.child_auto("Language");
	xml_node nodeText = nodeRes.append_child("Text");
	nodeText.attribute_auto("id").set_value(1);
	nodeText.attribute_auto("value").set_value(T2XML(_T("���w����")));

	nodeText = nodeRes.append_child("Text");
	nodeText.attribute_auto("id").set_value(2);
	nodeText.attribute_auto("value").set_value(T2XML(_T("DuiLib�OӋ��")));

	xml_writer_string w;
	xml.print(w);
	sci.sci_SetText(w.strText.c_str());
	sci.sci_SetReadOnly(TRUE);

	CString strSkinDir = GetUIManager()->GetDocument()->GetSkinPath();
	if(!strSkinDir.IsEmpty())
	{
		strSkinDir += g_cfg.strLangPath;
		strSkinDir += _T("\\");
	}
	CFileFind finder;
	BOOL bFind = finder.FindFile(strSkinDir + _T("*.*"));
	while (bFind)
	{
		bFind = finder.FindNextFile();
		if(finder.IsDots()) continue;
		if(finder.IsDirectory())	//���ļ��У�������ƣ��������ļ�ɾ��
		{
			CString lang = finder.GetFileName();
			m_cbLang.AddString(lang);
		}
	}
	m_cbLang.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgBuildLanguagePackageEx::OnBnClickedOk()
{
	xml_node root = GetUIManager()->GetDocument()->m_doc.child(XTEXT("Window"));
	if(!root)
	{
		root = GetUIManager()->GetDocument()->m_doc.child(XTEXT("Menu"));
		if(!root)
		{
			AfxMessageBox(_T("ֻ�и��ڵ�ΪWindow��Menu�����������԰���"));
			return;
		}
	}

	CString strLang;
	m_cbLang.GetWindowText(strLang);
	if(GetUIManager()->GetDocument()->m_mLangPackage.Find(strLang))
	{
		AfxMessageBox(_T("�Ѿ�������ͬ���Ե����԰���"));
		return;
	}

	xml_document *xml = new xml_document;
	GetUIManager()->GetDocument()->m_mLangPackage.Insert(strLang, xml);
	xml_node rootLang = xml->child_auto("Language");
	_buildLangPackage(rootLang, root);

	CDialogEx::OnOK();
}

void CDlgBuildLanguagePackageEx::_buildLangPackage(xml_node rootLang, xml_node nodeSkin)
{
	//ֻ������3������
	CString text = XML2T(nodeSkin.attribute(XTEXT("text")).as_string());
	CString tooltip = XML2T(nodeSkin.attribute(XTEXT("tooltip")).as_string());
	CString tipvalue = XML2T(nodeSkin.attribute(XTEXT("tipvalue")).as_string());

	int resourceid = nodeSkin.attribute(XTEXT("resourceid")).as_int();
	if(resourceid > 0)
	{
		//�Ѿ����ڵ�id��Ҫ�ظ�д��
		xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", nodeSkin.attribute(XTEXT("resourceid")).value());
		if(!text.IsEmpty() || !tooltip.IsEmpty() || !tipvalue.IsEmpty())
		{
			if(!nodeFind)
			{
				nodeFind = rootLang.append_child("Text");
				nodeFind.attribute_auto("id").set_value(resourceid);
			}
		}

		if(!text.IsEmpty())
		{
			nodeFind.attribute_auto("text").set_value(T2XML(text));
		}
		if(!tooltip.IsEmpty())
		{
			nodeFind.attribute_auto("tooltip").set_value(T2XML(tooltip));
		}
		if(!tipvalue.IsEmpty())
		{
			nodeFind.attribute_auto("tipvalue").set_value(T2XML(tipvalue));
		}
	}

	for (xml_node node = nodeSkin.first_child(); node; node=node.next_sibling())
	{
		_buildLangPackage(rootLang, node);
	}	
}