// DlgSelectFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgSelectFile.h"
#include "afxdialogex.h"


// CDlgSelectFile �Ի���

IMPLEMENT_DYNAMIC(CDlgSelectFile, CDialogEx)

CDlgSelectFile::CDlgSelectFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSelectFile::IDD, pParent)
{

}

CDlgSelectFile::~CDlgSelectFile()
{
}

void CDlgSelectFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_fileList);
}


BEGIN_MESSAGE_MAP(CDlgSelectFile, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSelectFile::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSelectFile ��Ϣ�������


BOOL CDlgSelectFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strTile;
	GetWindowText(strTile);
	if(m_eWorkType == eWORK_EXPORT_RES)
	{
		SetWindowText(strTile += _T(" ��Res.xml��"));
		m_fileTitle = _T("Res.xml");
	}
	else if(m_eWorkType == eWORK_EXPORT_LANG)
	{
		SetWindowText(strTile += _T(" �����԰���"));
		m_fileTitle = _T("lan_");
	}
	else if(m_eWorkType == eWORK_EXPORT_NAME)
	{
		SetWindowText(strTile += _T(" ��name���ԣ�"));
		m_fileTitle = _T("ui_control_name");
	}


	m_fileList.SetFont(GetFont());
	LoadList(g_proj.GetProjectPath());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgSelectFile::LoadList(LPCTSTR pstrPath)
{
	CDC *pDC = GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	ReleaseDC(pDC);

	CString strPath = pstrPath;
	if(strPath.GetAt(strPath.GetLength()-1) != _T('\\'))
		strPath += _T("\\");
	CFileFind finder;
	BOOL bFind = finder.FindFile(strPath + _T("*.*"));
	while (bFind)
	{
		bFind = finder.FindNextFile();
		if(finder.IsDots()) continue;
		if(finder.IsDirectory())
		{
			LoadList(finder.GetFilePath());
		}
		else 
		{
			if(IsNeedLoadToList(finder))
			{
				int n = m_fileList.AddString(finder.GetFilePath());
				m_fileList.SetItemHeight(n, tm.tmHeight + 8);
				m_fileList.SetCheck(n, TRUE);
			}
			/*
			CString fileName = finder.GetFilePath();
			int nFind = fileName.ReverseFind(_T('.'));
			if(nFind >= 0)
			{
				fileName = fileName.Right(fileName.GetLength() - nFind - 1);
				if(fileName.CompareNoCase(_T("xml")) == 0)
				{
					int n = m_fileList.AddString(finder.GetFilePath());
					m_fileList.SetItemHeight(n, tm.tmHeight + 8);

					if(finder.GetFileTitle() != _T("config"))
						m_fileList.SetCheck(n, TRUE);
				}
			}
			*/
		}
	}
}

BOOL CDlgSelectFile::IsNeedLoadToList(CFileFind &finder)
{
	CString fileName = finder.GetFilePath();
	int nFind = fileName.ReverseFind(_T('.'));
	if(nFind < 0) return FALSE;

	CString fileExtName = fileName.Right(fileName.GetLength() - nFind - 1);

	if(m_eWorkType == eWORK_EXPORT_RES)
	{
		if(fileExtName.CompareNoCase(_T("xml")) == 0)
			return TRUE;
		if((fileExtName.CompareNoCase(_T("jpg")) == 0)  ||
			(fileExtName.CompareNoCase(_T("bmp")) == 0) ||
			(fileExtName.CompareNoCase(_T("png")) == 0) ||
			(fileExtName.CompareNoCase(_T("gif")) == 0) ||
			(fileExtName.CompareNoCase(_T("ico")) == 0) ||
			(fileExtName.CompareNoCase(_T("svg")) == 0) )
			return TRUE;
	}
	else if(m_eWorkType == eWORK_EXPORT_LANG)
	{
		if(fileExtName.CompareNoCase(_T("xml")) == 0)
			return TRUE;
	}
	else if(m_eWorkType == eWORK_EXPORT_NAME)
	{
		if(fileExtName.CompareNoCase(_T("xml")) == 0)
			return TRUE;
	}

	return FALSE;
}

void CDlgSelectFile::OnBnClickedOk()
{
	if(m_eWorkType == eWORK_EXPORT_RES)
	{
		OnExportRes();
	}
	else if(m_eWorkType == eWORK_EXPORT_LANG)
	{
		OnExportLang();
	}
	else if(m_eWorkType == eWORK_EXPORT_NAME)
	{
		OnExportName();
	}

	AfxMessageBox(_T("��������"));	
	CDialogEx::OnOK();
}

void CDlgSelectFile::OnExportRes()
{
	TCHAR BASED_CODE szFilter[] = _T("xml(*.xml)|*.xml||");
	CFileDialog fileDlg(FALSE,_T("xml"), g_proj.GetProjectPath() + m_fileTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	if(fileDlg.DoModal() != IDOK)	return;

	xml_document xmlRes;
	xml_node rootRes = xmlRes.child_auto("Res");

	//��һ��д��Xml�ļ���������ʵ�Ǹ�����Xml�ļ��������
	int rows = m_fileList.GetCount();
	for (int i=0; i<rows; i++)
	{
		CString fileName;
		m_fileList.GetText(i, fileName);

		int nFind = fileName.ReverseFind(_T('.'));
		CString fileExtName;
		if(nFind > 0)
		{
			fileExtName = fileName.Right(fileName.GetLength() - nFind - 1);
		}

		if(fileExtName.CompareNoCase(_T("xml")) == 0)
		{
			int nFind = fileName.Find(g_proj.GetProjectPath());
			if(nFind == 0)
			{
				fileName.Delete(0, g_proj.GetProjectPath().GetLength());
			}

			CString idName = fileName;
			CString pathName = fileName;

			xml_node nodeXml = rootRes.append_child("Xml");
			nodeXml.attribute_auto("id").set_value(T2XML(idName));
			nodeXml.attribute_auto("path").set_value(T2XML(pathName));
		}	
	}

	//�ڶ���д��Image��Դ������ʵ�Ǹ�����ͼƬ�ļ��������
	rows = m_fileList.GetCount();
	for (int i=0; i<rows; i++)
	{
		CString fileName;
		m_fileList.GetText(i, fileName);

		int nFind = fileName.ReverseFind(_T('.'));
		CString fileExtName;
		if(nFind > 0)
		{
			fileExtName = fileName.Right(fileName.GetLength() - nFind - 1);
		}

		if((fileExtName.CompareNoCase(_T("jpg")) == 0)  ||
			(fileExtName.CompareNoCase(_T("bmp")) == 0) ||
			(fileExtName.CompareNoCase(_T("png")) == 0) ||
			(fileExtName.CompareNoCase(_T("gif")) == 0) ||
			(fileExtName.CompareNoCase(_T("ico")) == 0) ||
			(fileExtName.CompareNoCase(_T("svg")) == 0) )
		{
			int nFind = fileName.Find(g_proj.GetProjectPath());
			if(nFind == 0)
			{
				fileName.Delete(0, g_proj.GetProjectPath().GetLength());
			}

			CString idName = fileName;
			CString pathName = fileName;

			xml_node nodeXml = rootRes.append_child("Image");
			nodeXml.attribute_auto("id").set_value(T2XML(idName));
			nodeXml.attribute_auto("value").set_value(T2XML(pathName));
		}	
	}

	xmlRes.save_file(fileDlg.GetPathName());
}

void CDlgSelectFile::OnExportLang()
{
	TCHAR BASED_CODE szFilter[] = _T("xml(*.xml)|*.xml||");
	CFileDialog fileDlg(FALSE,_T("xml"), g_proj.GetProjectPath() + m_fileTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	if(fileDlg.DoModal() != IDOK)	return;

	xml_document xmlLang;
	xml_node rootLang = xmlLang.child_auto("Res");

	int rows = m_fileList.GetCount();
	for (int i=0; i<rows; i++)
	{
		CString fileName;
		m_fileList.GetText(i, fileName);

		xml_document xml;
		xml_parse_result ret = xml.load_file(fileName);
		if(ret.status != pugi::status_ok)
		{
			AfxMessageBox(fileName + _T(" ��ȡ�ļ�ʧ��"));
			continue;
		}
		_buildLangPackage(rootLang, xml.root());
	}

	xmlLang.save_file(fileDlg.GetPathName());
}

void CDlgSelectFile::_buildLangPackage(xml_node rootLang, xml_node nodeSkin)
{
	CString text = XML2T(nodeSkin.attribute(XTEXT("text")).as_string());
	CString tooltip = XML2T(nodeSkin.attribute(XTEXT("tooltip")).as_string());
	CString tipvalue = XML2T(nodeSkin.attribute(XTEXT("tipvalue")).as_string());
	BOOL bResourceText = nodeSkin.attribute(XTEXT("resourcetext")).as_bool();
	if(bResourceText)
	{
		if(!text.IsEmpty())
		{
			//�Ѿ����ڵ��ı���Ҫ�ظ�д��
			xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", T2XML(text));
			if(!nodeFind)
			{
				xml_node nodeLang = rootLang.append_child("Text");
				nodeLang.attribute_auto("id").set_value(T2XML(text));
				nodeLang.attribute_auto("value").set_value(T2XML(text));
			}		
		}

		if(!tooltip.IsEmpty())
		{
			//�Ѿ����ڵ��ı���Ҫ�ظ�д��
			xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", T2XML(tooltip));
			if(!nodeFind)
			{
				xml_node nodeLang = rootLang.append_child("Text");
				nodeLang.attribute_auto("id").set_value(T2XML(tooltip));
				nodeLang.attribute_auto("value").set_value(T2XML(tooltip));
			}		
		}

		if(!tipvalue.IsEmpty())
		{
			//�Ѿ����ڵ��ı���Ҫ�ظ�д��
			xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", T2XML(tipvalue));
			if(!nodeFind)
			{
				xml_node nodeLang = rootLang.append_child("Text");
				nodeLang.attribute_auto("id").set_value(T2XML(tipvalue));
				nodeLang.attribute_auto("value").set_value(T2XML(tipvalue));
			}		
		}
	}

	for (xml_node node = nodeSkin.first_child(); node; node=node.next_sibling())
	{
		_buildLangPackage(rootLang, node);
	}	
}

void CDlgSelectFile::OnExportName()
{
	TCHAR BASED_CODE szFilter[] = _T("c++ header file|*.h||");
	CFileDialog fileDlg(FALSE,_T("h"), g_proj.GetProjectPath() + m_fileTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	if(fileDlg.DoModal() != IDOK)	return;

	CFile f;
	if(!f.Open(fileDlg.GetPathName(), CFile::modeReadWrite|CFile::modeCreate|CFile::typeBinary))
	{
		AfxMessageBox(fileDlg.GetPathName() + _T(" �����ļ�ʧ��"));
		return;
	}

	CStringA sTitle = "//��Ŀ�е�Name���Զ���\r\n//\r\n\r\n\r\n";
	f.Write(sTitle, sTitle.GetLength());

	m_arrMacroName.RemoveAll();
	int rows = m_fileList.GetCount();
	for (int i=0; i<rows; i++)
	{
		CString fileName;
		m_fileList.GetText(i, fileName);

		xml_document xml;
		xml_parse_result ret = xml.load_file(fileName);
		if(ret.status != pugi::status_ok)
		{
			AfxMessageBox(fileName + _T(" ��ȡ�ļ�ʧ��"));
			continue;
		}
		
		//�����ļ���
		int nFind = fileName.Find(g_proj.GetProjectPath());
		if(nFind == 0)
		{
			fileName.Delete(0, g_proj.GetProjectPath().GetLength()); //תΪ��Ŀ���·��
		}

		CString fileTitleName;
		nFind = fileName.ReverseFind(_T('.'));
		if(nFind > 0)
		{
			fileTitleName = fileName.Left(nFind); //ȥ����չ��
		}

		CString sName = fileName;
		CString sMacroName = fileTitleName;
		sMacroName.Replace(_T('\\'), _T('_')); //·�������б��תΪ�»��ߡ�_��
		sMacroName.MakeUpper();
		sMacroName = _T("UIF_") + sMacroName; //�Ӹ�һ��ǰ׺
		if(m_arrMacroName.Find(sMacroName))
		{
			AfxMessageBox(sMacroName + _T(" �������ظ�")); //���ļ�������������ظ��ġ�
			return;
		}
		_WriteNameMacroFile(f, sMacroName, sName);

		//�����ؼ���
		_buildNameMacroFile(f, xml.root());

		//ÿд��һ���ļ�������һ�����У���������ÿ���ļ�����Ŀؼ�
		//���ؼ��ĺ����ظ�ʱ�����ֵĲ���׼ȷ�ˡ�
		f.Write("\r\n", 2);
	}

	f.Close();
}

void CDlgSelectFile::_buildNameMacroFile(CFile &f, xml_node nodeSkin)
{
	//Font��Ҫ����
	if(CompareString(nodeSkin.name(), "Font"))
		return;

	//Default��Ҫ����
	if(CompareString(nodeSkin.name(), "Default"))
		return;

	xml_attribute attr = nodeSkin.attribute(XTEXT("name"));
	if(attr)
	{
		CString sName = XML2T(attr.as_string());
		if(!sName.IsEmpty())
		{
			CString sMacroName = sName;
			sMacroName.MakeUpper();

			//�Ӹ�һ��ǰ׺
			sMacroName = _T("UIS_") + sMacroName;

			_WriteNameMacroFile(f, sMacroName, sName);
		}
	}

	for (xml_node node = nodeSkin.first_child(); node; node=node.next_sibling())
	{
		_buildNameMacroFile(f, node);
	}	
}


void CDlgSelectFile::_WriteNameMacroFile(CFile &f, CString &sMacroName, CString &sName)
{
	//�Ѿ����ڵĲ�Ҫд��
	if(m_arrMacroName.Find(sMacroName))
		return;
	m_arrMacroName.Add(sMacroName, true);

	CStringA sWrite;
	sWrite.Format("#define %s", LST2A(sMacroName));

	//����һЩ�Ʊ�������ļ��ÿ���
	int tTimes = 12 - sWrite.GetLength()/4;
	if(tTimes <= 0)
		sWrite += " ";
	else
	{
		for(int i=0; i<tTimes; i++) 
			sWrite += "\t";
	}
	sWrite += "_T(\"";
	sWrite += LST2A(sName);
	sWrite += "\")";
	sWrite += "\r\n";
	f.Write(sWrite, sWrite.GetLength());
}