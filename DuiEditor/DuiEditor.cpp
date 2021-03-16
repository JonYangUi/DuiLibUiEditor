
// DuiEditor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DuiEditor.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "DuiEditorDocTemplate.h"
#include "DuiEditorDoc.h"
#include "DuiEditorView.h"
#include "DuiEditorViewDesign.h"

#include "ChildFrm2.h"
#include "ScriptEditorDoc.h"
#include "ScriptEditorView.h"
#include "ScriptEditorDocTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ע���д���λ�� HKEY_CURRENT_USER\SOFTWARE\DuiEditor  == name
AFX_STATIC_DATA const TCHAR _afxFileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR _afxFileEntry[] = _T("File%d");
AFX_STATIC_DATA const TCHAR _afxPreviewSection[] = _T("Settings");
AFX_STATIC_DATA const TCHAR _afxPreviewEntry[] = _T("PreviewPages");

CDuiProject g_proj;
CString g_strAppPath;
CDuiPropertyFile g_duiProp;
//IScriptEngine *g_pScriptEngine = NULL;
//IScriptHelper *ScriptHelper = NULL;
HWND g_hWndMsg = NULL;

// CDuiEditorApp

BEGIN_MESSAGE_MAP(CDuiEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDuiEditorApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, &CDuiEditorApp::OnUpdateRecentFileMenu)
	ON_COMMAND(ID_FILE_NEW, &CDuiEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CDuiEditorApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW_XML, &CDuiEditorApp::OnFileNewXml)
	ON_COMMAND(ID_FILE_LOAD_TEMPLATE, &CDuiEditorApp::OnFileNewXmlFromTemplate)
	ON_COMMAND(ID_FILE_NEW_AS_SCRIPT, &CDuiEditorApp::OnFileNewAsScript)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW_AS_SCRIPT, &CDuiEditorApp::OnUpdateFileNewAsScript)
END_MESSAGE_MAP()


// CDuiEditorApp ����

CDuiEditorApp::CDuiEditorApp()
{
	m_hModuleScript = NULL;

	m_bHiColorIcons = TRUE;

	m_bIsDDEOpen = FALSE;
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DuiEditor.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDuiEditorApp ����

CDuiEditorApp theApp;


// CDuiEditorApp ��ʼ��

BOOL CDuiEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	AfxInitRichEdit2();

	//_CrtSetBreakAlloc(528);

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	CoInitialize(NULL);
	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	//CShadowUI::Initialize(m_hInstance);
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("DuiEditor"));
	LoadStdProfileSettings(16);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	free((void*)m_pszAppName);
	m_pszAppName=_tcsdup(_T("DuiEditor"));

	//��ȡ����·��
	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	::GetModuleFileName(m_hInstance, tszModule, MAX_PATH);
	g_strAppPath = tszModule;
	int pos = g_strAppPath.ReverseFind(_T('\\'));
	if( pos >= 0 ) g_strAppPath = g_strAppPath.Left(pos + 1);

	if(!g_duiProp.LoadPropertyFile())
	{
		return FALSE;
	}

	g_cfg.LoadConfig(g_strAppPath + _T("DuiEditor.xml"));
	g_session.Init();

#ifndef DUILIB_VERSION_ORIGINAL

	//�����ʽ������չ�ؼ�
#ifdef _DEBUG
	CPaintManagerUI::LoadPlugin(g_strAppPath + _T("DuiPlugins_ud.dll"));
#else
	CPaintManagerUI::LoadPlugin(g_strAppPath + _T("DuiPlugins_u.dll"));
#endif

	//�ѽű�����ӽ���
#ifdef _DEBUG
	m_hModuleScript = ::LoadLibrary(g_strAppPath +_T("DuiScript_ud.dll"));
#else
	m_hModuleScript = ::LoadLibrary(g_strAppPath +_T("DuiScript_u.dll"));
#endif
	if(m_hModuleScript)
	{
		m_funCreateScriptHelper = (CREATE_SCRIPT_HELPER)::GetProcAddress(m_hModuleScript, "CreateScriptHelper");
		m_funDeleteScriptHelper = (DELETE_SCRIPT_HELPER)::GetProcAddress(m_hModuleScript, "DeleteScriptHelper");
	}

	//����ģʽע����չ�ؼ�
	DuiPluginsRegister();

	//����DPIʱ����������ʾͼƬ��
	CPaintManagerUI::SetAdjustDPIRecource(false);

#endif //#ifndef DUILIB_VERSION_ORIGINAL

	
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CDuiEditorDocTemplate(IDR_DuiEditorTYPE,
		RUNTIME_CLASS(CDuiEditorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CDuiEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	//CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CScriptEditorDocTemplate(IDR_DuiScriptTYPE,
		RUNTIME_CLASS(CScriptEditorDoc),
		RUNTIME_CLASS(CChildFrame2), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CScriptEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������/��
	m_pMainWnd->DragAcceptFiles();

	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen || cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand =  CCommandLineInfo::FileNothing;

	//��֪��Ϊʲô������򿪶���ļ���mdi tabs����ˢ�� -_-!

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE)
	{
		m_bIsDDEOpen = TRUE;
		//AfxMessageBox(_T("cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE"));
		//AfxMessageBox(GetCommandLine());
	}

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(SW_MAXIMIZE); //m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CDuiEditorApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
// 	if(m_funDeleteScriptHelper)
// 	{
// 		(*m_funDeleteScriptHelper)(ScriptHelper);
// 		ScriptHelper= NULL;
// 	}

	CPaintManagerUI::Term();
	CoUninitialize();
	AfxOleTerm(FALSE);
	CleanState();
	return CWinAppEx::ExitInstance();
}

CDocTemplate *CDuiEditorApp::GetUIDocTemplate()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CDuiEditorDocTemplate)))
		{
			return pTemplate;
		}
	}
	return NULL;
}

CDocTemplate *CDuiEditorApp::GetScriptDocTemplate()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CScriptEditorDocTemplate)))
		{
			return pTemplate;
		}
	}
	return NULL;
}
// CDuiEditorApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CDuiEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDuiEditorApp �Զ������/���淽��

void CDuiEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CDuiEditorApp::LoadCustomState()
{
}

void CDuiEditorApp::SaveCustomState()
{
	
}

void CDuiEditorApp::LoadStdProfileSettings(UINT nMaxMRU)
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList == NULL);

	BOOL bNoRecentDocs = FALSE;
	GetSysPolicyValue(_AFX_SYSPOLICY_NORECENTDOCHISTORY, &bNoRecentDocs);
	if (nMaxMRU != 0 && !bNoRecentDocs )
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentFileList = new CRecentFileList(0, _afxFileSection, _afxFileEntry,
			nMaxMRU, MAX_PATH);
		m_pRecentFileList->ReadList();
	}
	// 0 by default means not set
	m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
}

void CDuiEditorApp::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
	/*
	CMenu *pMenu = pCmdUI->m_pMenu;
	if(pMenu != NULL)
	{
		int count = pMenu->GetMenuItemCount();
		for (int i=0; i<count; i++)
		{
			CString menustr;
			pMenu->GetMenuString(i, menustr, MF_BYPOSITION);
			{
				if (menustr == "������ļ�")
				{
					pCmdUI->m_pMenu = pMenu->GetSubMenu(i);
					CWinAppEx::OnUpdateRecentFileMenu(pCmdUI);
					pCmdUI->m_pMenu = pMenu;
					return;
				}
			}
		}
	}
	*/
	//InsertMsg(_T("OnUpdateRecentFileMenu"));
	CWinAppEx::OnUpdateRecentFileMenu(pCmdUI);
}



void CDuiEditorApp::OnFileNew()
{
	CWinAppEx::OnFileNew();
}


void CDuiEditorApp::OnFileOpen()
{
	CWinAppEx::OnFileOpen();
}


void CDuiEditorApp::OnFileNewXml()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CDuiEditorDocTemplate)))
		{
			pTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
}

void CDuiEditorApp::OnFileNewXmlFromTemplate()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CDuiEditorDocTemplate)))
		{
			CDuiEditorDocTemplate *pTemp = dynamic_cast<CDuiEditorDocTemplate *>(pTemplate);
			if(pTemp != NULL)
			{
				pTemp->OpenDocumentFile(NULL, TRUE, TRUE, TRUE);
			}
			else
			{
				pTemplate->OpenDocumentFile(NULL);
			}
			return;
		}
	}
}

void CDuiEditorApp::OnFileNewAsScript()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CScriptEditorDocTemplate)))
		{
			pTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
}


void CDuiEditorApp::OnUpdateFileNewAsScript(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(FALSE);
}
