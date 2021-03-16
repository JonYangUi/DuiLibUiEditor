#include "StdAfx.h"
#include "FormSciEditor.h"
#include "colors.h"

static const char *minus_xpm[] = { 
	"     9     9       16            1", 
	"` c #8c96ac", 
	". c #c4d0da", 
	"# c #daecf4", 
	"a c #ccdeec", 
	"b c #eceef4", 
	"c c #e0e5eb", 
	"d c #a7b7c7", 
	"e c #e4ecf0", 
	"f c #d0d8e2", 
	"g c #b7c5d4", 
	"h c #fafdfc", 
	"i c #b4becc", 
	"j c #d1e6f1", 
	"k c #e4f2fb", 
	"l c #ecf6fc", 
	"m c #d4dfe7", 
	"hbc.i.cbh", 
	"bffeheffb", 
	"mfllkllfm", 
	"gjkkkkkji", 
	"da`````jd", 
	"ga#j##jai", 
	"f.k##k#.a", 
	"#..jkj..#", 
	"hemgdgc#h"
};

static const char *plus_xpm[] = { 
	"     9     9       16            1", 
	"` c #242e44", 
	". c #8ea0b5", 
	"# c #b7d5e4", 
	"a c #dcf2fc", 
	"b c #a2b8c8", 
	"c c #ccd2dc", 
	"d c #b8c6d4", 
	"e c #f4f4f4", 
	"f c #accadc", 
	"g c #798fa4", 
	"h c #a4b0c0", 
	"i c #cde5f0", 
	"j c #bcdeec", 
	"k c #ecf1f6", 
	"l c #acbccc", 
	"m c #fcfefc", 
	"mech.hcem", 
	"eldikille", 
	"dlaa`akld", 
	".#ii`ii#.", 
	"g#`````fg", 
	".fjj`jjf.", 
	"lbji`ijbd", 
	"khb#idlhk", 
	"mkd.ghdkm"
}; 


#define VIEW_MARGIN_LINENUMBER 0
#define VIEW_MARGIN_BREAK	1
#define VIEW_MARGIN_FOLD	2

#define BREAK_TYPEN	0

CFormSciEditor::CFormSciEditor(void)
{
}


CFormSciEditor::~CFormSciEditor(void)
{
}

void CFormSciEditor::InitWindow()
{
	m_pSci = (CSciEditUI *)FindControl(_T("sciedit_1"));

	
	InitXml(); //ʹ��XML�ʷ�������
#ifdef _DEBUG
	//���뵱ǰ�Ĳ����ļ�
	CFile file;
	if(file.Open(GetManager()->GetResourcePath() + GetMainWnd()->GetSkinFile(), CFile::modeRead))
	{
		UINT buflen = (UINT)file.GetLength();

		char *pBuffer = new char[buflen + 1];
		file.Read((void *)pBuffer, buflen);
		pBuffer[buflen] = 0;
		m_pSci->sci_SetText(pBuffer);
		m_pSci->sci_SetSavePoint();

		delete []pBuffer;
		file.Close();
	}	
#else 
	//Release�汾����Դ�������ļ�, 
	//������ļ���UTF8���룬����ʹ��CSciEdit::SetText(LPCTSTR pstrText)
	LPBYTE pByte = NULL;
	int size = CRenderEngine::LoadImage2Memory(GetMainWnd()->GetSkinFile().GetData(), 0, pByte);
	CStringA str((const char *)pByte, size);
	m_pSci->sci_SetText(str);
	CRenderEngine::FreeMemory(pByte);
#endif
	
}

void CFormSciEditor::InitXml()
{
	m_pSci->sci_SetLexer(SCLEX_XML);		//�趨�ʷ�������
	m_pSci->sci_SetCodePage(SC_CP_UTF8);	//����

	m_pSci->SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier New");//(LPARAM)"Courier New");//"΢���ź�");
	m_pSci->sci_StyleSetSize(STYLE_DEFAULT, 14);//14);
	m_pSci->sci_StyleSetFore(STYLE_DEFAULT,RGB(0,0,0));

	m_pSci->sci_StyleSetBack(STYLE_DEFAULT, RGB(255,255,255));
	m_pSci->sci_StyleSetBack(STYLE_LINENUMBER, RGB(255,255,255));
	m_pSci->sci_StyleSetBack(STYLE_INDENTGUIDE, RGB(255,255,255));

	for (int i = SCE_H_DEFAULT; i <= SCE_HPHP_OPERATOR; i++)
	{
		m_pSci->sci_StyleSetBack(i,	RGB(255,255,255));
	}

	//����ѡ���ı�����ɫ
	m_pSci->sci_SetSelBack(STYLE_DEFAULT, RGB(0xA0,0xCA,0xF0));//RGB(0xA0,0xCA,0xF0));

	m_pSci->sci_SetExtraDescent(1);
	m_pSci->sci_SetExtraAscent(1);

	//XML
	COLORREF crfComment = RGB(0,150,0);
	m_pSci->sci_StyleSetFore(SCE_H_COMMENT,				crfComment);
	m_pSci->sci_StyleSetFore(SCE_H_ATTRIBUTEUNKNOWN,	crfComment);
	//�ַ���˫����δ���ʱ
	m_pSci->sci_StyleSetFore(SCE_H_DOUBLESTRING,	RGB(128,0,255));
	m_pSci->sci_StyleSetFore(SCE_H_SINGLESTRING,	RGB(128,0,255));
	//��ǩδ���ʱ��ɫ
	m_pSci->sci_StyleSetFore(SCE_H_TAGUNKNOWN,		RGB(255,0,255));
	//��ǩ���ʱ��ɫ
	m_pSci->sci_StyleSetFore(SCE_H_TAG,				RGB(0,0,255));
	m_pSci->sci_StyleSetFore(SCE_H_TAGEND,			RGB(0,0,255));
	//������ɫ
	m_pSci->sci_StyleSetFore(SCE_H_ATTRIBUTE,		RGB(255,0,0));
	m_pSci->sci_StyleSetBold(SCE_H_ATTRIBUTE,		true);

	//////////////////////////////////////////////////////////////////////////
	//�к�
	m_pSci->sci_SetMarginTypeN(VIEW_MARGIN_LINENUMBER, SC_MARGIN_NUMBER);
	int lineNumber = m_pSci->sci_GetLineCount();
	//int charWidth = sci_TextWidth(STYLE_LINENUMBER, _T("9"));
	int charWidth = m_pSci->SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, LPARAM("9"));
	CString tempLine;
	tempLine.Format(_T("%d"), lineNumber);
	m_pSci->sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);

	//�ϵ�
	m_pSci->sci_SetMarginTypeN(VIEW_MARGIN_BREAK, SC_MARGIN_SYMBOL);
	m_pSci->sci_SetMarginWidthN(VIEW_MARGIN_BREAK, 10);
	m_pSci->sci_SetMarginMaskN(VIEW_MARGIN_BREAK, 1);
	//sci_MarkerEnableHighlight(TRUE);
	m_pSci->sci_MarkerSetFore(BREAK_TYPEN, RGB(255,0,0));
	m_pSci->sci_MarkerSetBack(BREAK_TYPEN, RGB(255,0,0));

	//�����۵�
	m_pSci->sci_SetMarginTypeN(VIEW_MARGIN_FOLD, SC_MARGIN_SYMBOL);	//ҳ������
	m_pSci->sci_SetMarginMaskN(VIEW_MARGIN_FOLD, SC_MASK_FOLDERS);	//ҳ������
	m_pSci->sci_SetMarginWidthN(VIEW_MARGIN_FOLD,20);				//ҳ�߿��
	m_pSci->sci_SetMarginSensitiveN(VIEW_MARGIN_FOLD, TRUE);		//ҳ����Ӧ�����

	m_pSci->sci_SetProperty("lexer.xml.allow.scripts", "1");
	m_pSci->sci_SetProperty("fold", "1");
	m_pSci->sci_SetProperty("fold.html", "1");
	m_pSci->sci_SetProperty("fold.compact", "0");
	m_pSci->sci_SetProperty("fold.hypertext.comment", "1");

	// �۵���ǩ��ʽ      
	m_pSci->SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PIXMAP);  
	m_pSci->SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_PIXMAP);  
	m_pSci->SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND,  SC_MARK_PIXMAP);  
	m_pSci->SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_PIXMAP);  
	m_pSci->SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);  
	m_pSci->SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);  
	m_pSci->SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE); 

	m_pSci->SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDER, (sptr_t)plus_xpm);  
	m_pSci->SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPEN, (sptr_t)minus_xpm);  
	m_pSci->SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEREND, (sptr_t)plus_xpm);  
	m_pSci->SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPENMID, (sptr_t)minus_xpm); 

	// �۵���ǩ��ɫ       
	m_pSci->sci_MarkerSetFore(SC_MARKNUM_FOLDEROPEN,	RGB(0,0,255));
	m_pSci->sci_MarkerSetFore(SC_MARKNUM_FOLDER,		RGB(0,0,255));
	m_pSci->SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0xa0a0a0);  
	m_pSci->SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0xa0a0a0);  
	m_pSci->SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0xa0a0a0);  


	//����۵������۵��е����滭һ������  }  
	m_pSci->sci_SetFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED); 

	m_pSci->sci_SetTabWidth(4);	//�Ʊ�����
	m_pSci->sci_SetIndentationGuides(SC_IV_LOOKBOTH);	//������ʾ��
	m_pSci->sci_SetIndent(4);	//������ʾ�ߵ��ַ����
	m_pSci->sci_SetUseTabs(TRUE);

	//����ƥ�����ɫ
	m_pSci->sci_StyleSetBack(STYLE_BRACELIGHT, RGB(0,255,0));
	//����δƥ���
	m_pSci->sci_StyleSetBack(STYLE_BRACEBAD, RGB(255,0,0));

	//��ǰ�и�����ʾ
	m_pSci->sci_SetCaretLineVisible(TRUE);
	m_pSci->sci_SetCaretLineBack(RGB(215,215,247));

	//�Զ��������������
	m_pSci->sci_SetScrollWidthTracking(TRUE);

	//������ʽ
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE, red);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_SMART, liteGreen);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_INC, blue);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_TAGMATCH, purple);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_TAGATTR, yellow);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT1, cyan);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT2, orange);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT3, yellow);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT4, purple);
	m_pSci->sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT5, darkGreen);

	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_SMART, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_INC, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_TAGMATCH, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_TAGATTR, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT1, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT2, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT3, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT4, INDIC_ROUNDBOX);
	m_pSci->execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT5, INDIC_ROUNDBOX);	

	int aplpa = 80;
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_SMART, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_INC, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGMATCH, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGATTR, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT1, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT2, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT3, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT4, aplpa);
	m_pSci->execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT5, aplpa);	

	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_SMART, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_INC, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_TAGMATCH, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_TAGATTR, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT1, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT2, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT3, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT4, true);
	m_pSci->execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT5, true);

	//��ݼ�������ʾ����
	m_pSci->execute(SCI_CLEARCMDKEY, (WPARAM)('F'+(SCMOD_CTRL<<16)), SCI_NULL);
	m_pSci->execute(SCI_CLEARCMDKEY, (WPARAM)('H'+(SCMOD_CTRL<<16)), SCI_NULL);
	m_pSci->execute(SCI_CLEARCMDKEY, (WPARAM)('Z'+((SCMOD_CTRL|SCMOD_SHIFT)<<16)), SCI_NULL);
	m_pSci->execute(SCI_CLEARCMDKEY, (WPARAM)('Y'+(SCMOD_CTRL<<16)), SCI_NULL);
	m_pSci->execute(SCI_CLEARCMDKEY, (WPARAM)SCK_ESCAPE, SCI_NULL);
}

void CFormSciEditor::OnNotifySciNotify(TNotifyUI &msg)
{
	if(IsControl(msg, _T("sciedit_1")))
	{
		SCNotification *pMsg = (SCNotification*)msg.lParam;
		switch (pMsg->nmhdr.code)
		{
		case SCN_STYLENEEDED:
			break;
		case SCN_CHARADDED:
			break;
		case SCN_SAVEPOINTREACHED:
			break;
		case SCN_SAVEPOINTLEFT: //�ļ����޸�
			break;
		case SCN_MODIFYATTEMPTRO:
			break;
		case SCN_KEY:
			break;
		case SCN_DOUBLECLICK:
			break;
		case SCN_UPDATEUI:
			break;
		case SCN_MODIFIED:
			{
				if(pMsg->linesAdded!=0 &&
					m_pSci->sci_GetMarginWidthN(VIEW_MARGIN_LINENUMBER)!=0) //�Զ������кŵĿ��
				{
					int lineNumber = m_pSci->sci_GetLineCount();
					int charWidth = m_pSci->sci_TextWidth(STYLE_LINENUMBER, "9");
					CString tempLine;
					tempLine.Format(_T("%d"), lineNumber);
					m_pSci->sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);
				}
			}
			break;
		case SCN_MACRORECORD:
			break;
		case SCN_MARGINCLICK:
			{
				int line = m_pSci->sci_LineFromPosition(pMsg->position);
				m_pSci->sci_ToggleFold(line);
			}
			break;
		case SCN_NEEDSHOWN:
			break;
		case SCN_PAINTED:
			break;
		case SCN_USERLISTSELECTION:
			break;
		case SCN_URIDROPPED:
			break;
		case SCN_DWELLSTART:
			break;
		case SCN_DWELLEND:
			break;
		case SCN_ZOOM:
			break;
		case SCN_HOTSPOTCLICK:
			break;
		case SCN_HOTSPOTDOUBLECLICK:
			break;
		case SCN_HOTSPOTRELEASECLICK:
			break;
		case SCN_INDICATORCLICK:
			break;
		case SCN_INDICATORRELEASE:
			break;
		case SCN_CALLTIPCLICK:
			break;
		case SCN_AUTOCSELECTION:
			break;
		case SCN_AUTOCCANCELLED:
			break;
		case SCN_AUTOCCHARDELETED:
			break;
		case SCI_SETMODEVENTMASK:
			break;
		case SCI_GETMODEVENTMASK:
			break;
		case SCI_SETMOUSEDWELLTIME:
			break;
		case SCI_GETMOUSEDWELLTIME:
			break;
		case SCI_SETIDENTIFIER:
			break;
		case SCI_GETIDENTIFIER:
			break;
		case SCEN_CHANGE:
			break;
		case SCEN_SETFOCUS:
			break;
		case SCEN_KILLFOCUS:
			break;
		}
	}
}