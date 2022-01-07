// DlgCreateDuiDocument.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgCreateDuiDocument.h"
#include "afxdialogex.h"


// CDlgCreateDuiDocument �Ի���

IMPLEMENT_DYNAMIC(CDlgCreateDuiDocument, CDialogEx)

CDlgCreateDuiDocument::CDlgCreateDuiDocument(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateDuiDocument::IDD, pParent)
{
	m_pRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
	m_pRender->Init(NULL, NULL);

	m_uiImage = MakeRefPtr<UIImage>(UIGlobal::CreateImage());
}

CDlgCreateDuiDocument::~CDlgCreateDuiDocument()
{
}

void CDlgCreateDuiDocument::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_staPicture);
}


BEGIN_MESSAGE_MAP(CDlgCreateDuiDocument, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCreateDuiDocument::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgCreateDuiDocument::OnSelchangeList1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgCreateDuiDocument ��Ϣ�������


BOOL CDlgCreateDuiDocument::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
				FileName = finder.GetFileName();
				m_listBox.AddString(FileName);
			}
		}
	}
	finder.Close();	

	m_listBox.SelectString(-1, _T("����ҳ"));

	//m_listBox.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgCreateDuiDocument::OnBnClickedOk()
{
	m_listBox.GetText(m_listBox.GetCurSel(), m_strModuleName);
	CDialogEx::OnOK();
}


void CDlgCreateDuiDocument::OnSelchangeList1()
{
	int nSel = m_listBox.GetCurSel();
	if(nSel >= 0)
	{
		CString strFolder;
		m_listBox.GetText(nSel, strFolder);

		CRect rcClient;
		m_staPicture.GetWindowRect(&rcClient);
		ScreenToClient(rcClient);
		m_uiImage->LoadImage(g_strAppPath + _T("template\\") + strFolder + _T("\\skin.jpg"));
		
		CRect rcPaint(0,0,rcClient.Width(), rcClient.Height());
		CRect rc(0,0,m_uiImage->nWidth, m_uiImage->nHeight);
		CRect rcCorner;
		m_pRender->Resize(rc);
		m_pRender->DrawBitmap(m_uiImage->bitmap->GetBitmap(), rc, rc, rc, rcCorner, false);
		//m_pRender->SelectObject(m_uiImage);
		Invalidate();
//		m_staPicture.SetPreviewImage(g_strAppPath + _T("template\\") + strFolder + _T("\\skin.jpg"));
	}
}


void CDlgCreateDuiDocument::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CPaintDC dcPic(&m_staPicture);
	CRect rcClient;
	m_staPicture.GetWindowRect(&rcClient);
	::StretchBlt(dcPic.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(), m_pRender->GetDC(), 0, 0, m_pRender->GetBitmap()->GetWidth(), m_pRender->GetBitmap()->GetHeight(), SRCCOPY);

}
