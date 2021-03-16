// DlgTemplateSave.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgTemplateSave.h"
#include "afxdialogex.h"

#include "DuiEditorDoc.h"
// CDlgTemplateSave �Ի���

IMPLEMENT_DYNAMIC(CDlgTemplateSave, CDialogEx)

CDlgTemplateSave::CDlgTemplateSave(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTemplateSave::IDD, pParent)
{

	m_strTemplateName = _T("");
}

CDlgTemplateSave::~CDlgTemplateSave()
{
}

void CDlgTemplateSave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_staPicture);
	DDX_Text(pDX, IDC_EDIT1, m_strTemplateName);
}


BEGIN_MESSAGE_MAP(CDlgTemplateSave, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgTemplateSave::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgTemplateSave ��Ϣ�������


BOOL CDlgTemplateSave::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTemplateSave::OnBnClickedOk()
{
	if(!UpdateData(TRUE))	return;

	//�����ļ���
	CFileFind finder;
	BOOL bFind;
	CString FileName;
	bFind = finder.FindFile(g_strAppPath + _T("template\\*.*"));
	while(bFind)
	{
		bFind = finder.FindNextFile();
		if(!finder.IsDots())	//�ǲ���Ŀ¼(.   ..)
		{
			if(finder.IsDirectory())	//���ļ���
			{
				if(FileName == m_strTemplateName)
				{
					AfxMessageBox(_T("�Ѿ��Ѵ�����ͬ���ֵ�ģ��"));
					return;
				}
			}
		}
	}
	finder.Close();	

	CreateDirectory(g_strAppPath + _T("template\\"), NULL);

	//����Ŀ¼
	CString strPath = g_strAppPath + _T("template\\") + m_strTemplateName;
	CreateDirectory(strPath, NULL);

	//�����ͼ
	m_staPicture.GetImage()->Save(strPath+_T("\\skin.jpg"), Gdiplus::ImageFormatJPEG);

	//����Ĭ������
	xml_node root = m_pDoc->m_doc.root().child(XTEXT("Window"));
	m_pDoc->FilterDefaultValue(root);

	//����һ������, Ȼ�󱣴濽��
	xml_document doc;
	doc.root().append_copy(root);
	bool bSave = doc.save_file(strPath+_T("\\skin.xml"), PUGIXML_TEXT("\t"), format_default, encoding_utf8);
	if(!bSave)	return;


	CDialogEx::OnOK();
}
