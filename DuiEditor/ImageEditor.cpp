// ImageEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditor.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "UIManager.h"

// CImageEditor �Ի���
CImageEditor *g_pEditorImage = NULL;
IMPLEMENT_DYNAMIC(CImageEditor, CDialogEx)

CImageEditor::CImageEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageEditor::IDD, pParent)
{
	m_pFrame = NULL;
	m_pParentGrid = NULL;
}

CImageEditor::~CImageEditor()
{
	
}

void CImageEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImageEditor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImageEditor::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CImageEditor ��Ϣ�������


BOOL CImageEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_pEditorImage = this;

	CRect rectDummy;
	GetClientRect(rectDummy);
	DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	m_pFrame = (CImageEditorFrame* )RUNTIME_CLASS(CImageEditorFrame)->CreateObject();
	m_pFrame->Create(NULL, _T(""), dwStyle, rectDummy, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CImageEditor::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CImageEditor::OnBnClickedCancel()
{

	CDialogEx::OnCancel();
}


void CImageEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(!m_pFrame)	return;
	if(!m_pFrame->GetSafeHwnd())	return;
	m_pFrame->MoveWindow(0, 0, cx, cy);
}


void CImageEditor::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if(m_pFrame && m_pFrame->GetSafeHwnd())
	{
		m_pFrame->DestroyWindow();
	}
	ClearImage();
	g_pEditorImage = NULL;
}

BOOL CImageEditor::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CImageEditor::SetAttributeValue(LPCTSTR szAttribute)
{
	CString strXML = _T("<IMAGE ");
	strXML += szAttribute;
	strXML += _T(" />");
	xml_parse_result ret = m_nodeImage.load(T2XML(strXML));
	if(ret.status != pugi::status_ok)//���ƥ�䲻�ɹ�, ����Ϊֻ���ļ���, ����������
	{
		strXML = _T("<IMAGE />");
		ret = m_nodeImage.load(T2XML(strXML));
		m_nodedata = m_nodeImage.child(XTEXT("IMAGE"));

		g_duiProp.AddAttribute(m_nodedata, _T("file"), szAttribute, NULL);
	}
	else
	{
		m_nodedata = m_nodeImage.child(XTEXT("IMAGE"));
	}

	//����ͼƬ
	CString strSkinDir = g_proj.GetProjectPath(); //GetUIManager()->GetDocument()->GetSkinPath();
	CString strPathName = strSkinDir + XML2T(m_nodedata.attribute(XTEXT("file")).value());
	LoadImageFile(strPathName);

	return;
}


void CImageEditor::SetImageFile(LPCTSTR lpstrPathName)
{
	CString strDocPath = g_proj.GetProjectPath(); //GetUIManager()->GetDocument()->GetSkinPath(); //CPaintManagerUI::GetResourcePath();
	CString strFileName = lpstrPathName;

	//ֻ��ȡ��Ŀ¼���ļ�
	if(strFileName.Find(strDocPath) == 0)
	{
		strFileName = strFileName.Right(strFileName.GetLength() - strDocPath.GetLength());
	}
	else
	{
		::MessageBoxA(NULL, "ֻ��ʹ�õ�ǰĿ¼������Ŀ¼���ļ�", "ERROR", MB_OK);
		return;
	}

	g_duiProp.AddAttribute(g_pEditorImage->m_nodedata, _T("file"), strFileName, NULL);

	LoadImageFile(lpstrPathName);
}

void CImageEditor::LoadImageFile(LPCTSTR lpstrPathName)
{
	m_strSelectedFile = lpstrPathName;
	m_rcImage.SetRect(0,0,0,0);
	ClearImage();

	UIImage *pImageInfo = UIGlobal::CreateImage();

	do 
	{
		CUIFile f;
		if(!f.LoadFile(lpstrPathName))
			break;

		//���Խ���GIF
		CStdPtrArray arrImageInfo;
		if(pImageInfo->LoadGifImageFromMemory(f.GetData(), f.GetSize(), arrImageInfo))
		{
			for (int i=0; i<arrImageInfo.GetSize(); i++)
			{
				UIImage *pImageInfo2 = (UIImage *)arrImageInfo.GetAt(i);
				if(pImageInfo2->nWidth > m_rcImage.right) m_rcImage.right = pImageInfo2->nWidth; 
				if(pImageInfo2->nHeight > m_rcImage.bottom) m_rcImage.bottom = pImageInfo2->nHeight; 
				m_imageFrames.Add(pImageInfo2);
			}
			break;
		}

		//����������ʽ��ͼ��
		pImageInfo->LoadImageFromMemory(f.GetData(), f.GetSize());
		if(!pImageInfo) break;

		if(m_imageFrames.Add(pImageInfo))
		{
			m_rcImage.SetRect(0, 0, pImageInfo->nWidth, pImageInfo->nHeight);
			pImageInfo = NULL;
			break;
		}

	} while (false);

	if(pImageInfo)	pImageInfo->Release();
}

void CImageEditor::ClearImage()
{
	for (int i=0; i<m_imageFrames.GetSize(); i++)
	{
		UIImage *pImageInfo = (UIImage *)m_imageFrames.GetAt(i);
		pImageInfo->Release();
	}
	m_imageFrames.Empty();
}

CString CImageEditor::GetAttributeValue()
{
	xml_node node = m_nodeImage.child(XTEXT("IMAGE"));

	//����Ĭ������
	g_duiProp.FilterDefaultValue(node, NULL);
	g_duiProp.FilterPosWidthHeight(node, NULL);

	//�ж�dest�Ƿ����ͼ��ԭʼ��С, ������ɾ��dest���Զ���
	xml_attribute attr = node.attribute(XTEXT("dest"));
	if(attr)
	{
		CDuiRect rc(XML2T(attr.value()));
		if(m_rcControl == rc)
			node.remove_attribute(attr);
	}

	//�ж�source�Ƿ����ͼ��ԭʼ��С, ������ɾ��source���Զ���
	xml_attribute attr2 = node.attribute(XTEXT("source"));
	if(attr2)
	{
		CDuiRect rc(XML2T(attr2.value()));
		if(m_rcImage == rc)
			node.remove_attribute(attr2);
	}

	CString strImage, temp;
	//�ж��Ƿ�ֻ���ļ�������
	BOOL bCustomDefine = FALSE;
	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		if(CompareString(attr.name(), _T("file")))
		{
			continue;
		}
		bCustomDefine = TRUE;
	}

	if(bCustomDefine)
	{
		for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
		{
			temp.Format(_T("%s='%s' "), XML2T(attr.name()), XML2T(attr.value()));
			strImage += temp;
		}
	}
	else
	{
		strImage = XML2T(node.attribute(XTEXT("file")).value());
	}

	return strImage;
}

LRESULT CImageEditor::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(message == WM_KICKIDLE)
	{
		if(m_pFrame && m_pFrame->GetSafeHwnd())
		{
			m_pFrame->SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
		}
	}

	return CDialogEx::DefWindowProc(message, wParam, lParam);
}


BOOL CImageEditor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		if(m_pFrame && m_pFrame->GetSafeHwnd())
		{
			CWnd *pWnd = m_pFrame->GetFocus();
			if(pWnd)
				pWnd->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

/*
#define strtoll _strtoi64
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "nanosvg/src/stb_image_write.h"
#define NANOSVG_IMPLEMENTATION
#include "nanosvg/src/nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvg/src/nanosvgrast.h"

BOOL CImageEditor::svg_2_cximage(CxImage &cximg, LPCTSTR strPathName)
{
	NSVGimage *image = nsvgParseFromFile(LST2A(strPathName), "px", 96.0f);
	if(image == NULL) return FALSE;

	int w = (int)image->width;
	int h = (int)image->height;
	if(w==0 || h==0) return FALSE;

	NSVGrasterizer *rast = nsvgCreateRasterizer();
	if (rast == NULL) 
	{
		nsvgDelete(image);
		return FALSE;
	}

	unsigned char* img = (unsigned char*)malloc(w*h*4);
	if (img == NULL) 
	{
		nsvgDeleteRasterizer(rast);
		nsvgDelete(image);
		return FALSE;
	}

	nsvgRasterize(rast, image, 0,0,1, img, w, h, w*4);

	//stbi_write_png("c:\\svg.png", w, h, 4, img, w*4);

	int outlen = 0;
	BYTE *out = stbi_write_png_to_mem(img, w*4, w, h, 4, &outlen);

	cximg.DestroyFrames(); cximg.Destroy();
	bool bDecode = cximg.Decode(out, outlen, CXIMAGE_FORMAT_PNG);
	//cximg->Save(_T("c:\\svg2.png"), CXIMAGE_FORMAT_PNG);

	free(out);
	free(img);
	nsvgDeleteRasterizer(rast);
	nsvgDelete(image);
	return bDecode;
}

*/