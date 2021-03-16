#include "StdAfx.h"
#include "DuiEditor.h"
#include "SciXmlWriter.h"


CSciXmlWriter::CSciXmlWriter(CSciWnd *pSciWnd) : m_pSciWnd(pSciWnd)
{
	m_nBeginPos = 0;
	m_nBeginDepth = 0;
	m_bRootBeginNeedEndLine = FALSE;
	m_bRootEndNeedEndLine = TRUE;
}

CSciXmlWriter::~CSciXmlWriter(void)
{
}

void CSciXmlWriter::InitWriter(int beginPos, int beginDepth, BOOL bRootBeginNeedEndLine, BOOL bRootEndNeedEndLine)
{
	m_strTempBuffer.Empty();
	m_nBeginPos = beginPos;
	m_nBeginDepth = beginDepth;
	m_bRootBeginNeedEndLine = bRootBeginNeedEndLine;
	m_bRootEndNeedEndLine = bRootEndNeedEndLine;
}

const char *CSciXmlWriter::GetOutputBuffer()
{
	return m_strTempBuffer;
}

int CSciXmlWriter::GetOutputLength()
{
	return m_strTempBuffer.GetLength();
}

void CSciXmlWriter::print(xml_node node)
{
	if(node)
	{
		if(m_bRootBeginNeedEndLine)
		{
			write("\r\n");
		}
		int pos = m_nBeginPos;
		int depth = m_nBeginDepth;
		node_output(node, node, pos, depth);
	}
	flush();
}

void CSciXmlWriter::write(const char *text)
{
	//LSSTRING_CONVERSION;
	//m_strTempBuffer += LST2UTF8(text);
	m_strTempBuffer += text;
}

void CSciXmlWriter::write(char text)
{
	//LSSTRING_CONVERSION;
	//m_strTempBuffer += LST2UTF8(&text);
	m_strTempBuffer += text;
}

void CSciXmlWriter::writeEndLine()
{
	write("\r\n");
}

void CSciXmlWriter::flush()
{
	if(m_pSciWnd && !m_strTempBuffer.IsEmpty())
	{
		//LSSTRING_CONVERSION;
		//InsertMsg(LSUTF82T(m_strTempBuffer));
		m_pSciWnd->sci_InsertText(m_nBeginPos, (const char *)m_strTempBuffer);
		//m_pSciWnd->SendEditor(SCI_APPENDTEXT, m_strTempBuffer.GetLength(), (LPARAM)(const char *)m_strTempBuffer);
		//m_strTempBuffer.Empty();
	}
}

void CSciXmlWriter::node_output(xml_node root, xml_node node, int &pos, int &depth)
{	
	//�ݹ��ӡxml��ʵЧ�ʺ��������ǣ��������ǲ���Ҫ������ӡ�����ĵ���
	xml_node nodeParent = node.parent();

	xml_node_type ndtype = node.type();
	switch (ndtype)
	{
	case node_document:
		for(xml_node nodeChild=node.first_child(); nodeChild; nodeChild=nodeChild.next_sibling())
		{
			node_output(root, nodeChild, pos, depth);
		}
		break;

	case node_pcdata:
		break;

	case node_cdata:
		break;

	case node_comment:
		text_output_indent(depth);
		node.set_open_start_pos(GetCurPos());

		write("<!--");
		write(XML2UTF8(node.value()));
		write("-->");

		node.set_open_end_pos(GetCurPos());
		writeEndLine();
		break;

	case node_pi:
		break;

	case node_declaration:
		write("<?");
		write(XML2UTF8(node.name()));
		node_output_all_attributes(node);
		write("?>");
		writeEndLine();
		break;

	case node_doctype:
		write("<!DOCTYPE");
		if (node.value())
		{
			write(' ');
			write(XML2UTF8(node.value()));
		}
		write('>');
		break;

	case node_element:
		text_output_indent(depth);
		node.set_open_start_pos(GetCurPos());

		//��ӡ<NodeName 
		write("<");
		write(XML2UTF8(node.name()));

		//��ӡ����
		node_output_all_attributes(node);

		//��ӡ ...>
		if(!node.first_child()) write(" />");
		else write(">");

		node.set_open_end_pos(GetCurPos());

		//���������, ˵����Ҫ�հ�, ֱ�ӻ��С� �����ж��ǲ��Ǹ��ڵ㣬�ǲ�����Ҫ����
		if(node.first_child())
			writeEndLine();
		else
		{
			if(node == root)
			{
				if(m_bRootEndNeedEndLine) writeEndLine();
			}
			else 
				writeEndLine();
		}


		depth++;
		for(xml_node nodeChild=node.first_child(); nodeChild; nodeChild=nodeChild.next_sibling())
		{
			node_output(root, nodeChild, pos, depth);
		}
		depth--;

		//��ӡ</NodeName>
		if(node.first_child())
		{
			text_output_indent(depth);
			node_output_close(node);
			if(node == root)
			{
				if(m_bRootEndNeedEndLine) writeEndLine();
			}
			else 
				writeEndLine();
		}

		break;

	default:
		assert(!"Invalid node type");
	}		

}

void CSciXmlWriter::node_output_attribute(xml_attribute attr)
{
	attr.set_name_start_pos(GetCurPos());
	write(XML2UTF8(attr.name()));
	attr.set_name_end_pos(GetCurPos());

	write("=\"");

	attr.set_value_start_pos(GetCurPos());
	text_output(XML2UTF8(attr.value())); 
	attr.set_value_end_pos(GetCurPos());

	write("\"");
}

void CSciXmlWriter::node_output_all_attributes(xml_node node)
{
	const char_t* default_name = PUGIXML_TEXT(":anonymous");

	for (xml_attribute a = node.first_attribute(); a; a = a.next_attribute())
	{
		write(' ');

		write(XML2UTF8(a.name()));
		write("=\"");

		text_output(XML2UTF8(a.value()));

		write('"');
	}
}

void CSciXmlWriter::node_output_close(xml_node node)
{
	node.set_close_start_pos(GetCurPos());

	write("</");
	write(XML2UTF8(node.name()));
	write(">");

	node.set_close_end_pos(GetCurPos());
}

void CSciXmlWriter::text_output(const char * text)
{
	const char * s = text;

	//����Ƿ���ת���ַ�
	BOOL IsEscapeChar = FALSE;
	while (*s)
	{
		switch (*s)
		{
		case 0: break;
		case '&':
			IsEscapeChar = TRUE;
			break;
		case '<':
			IsEscapeChar = TRUE;
			break;
		case '>':
			IsEscapeChar = TRUE;
			break;
		case '"':
			IsEscapeChar = TRUE;
			break;
		default:
			break;
		}

		if(IsEscapeChar)
			break;

		++s;
	}

	if(!IsEscapeChar)
	{
		write(text);
		return;
	}

	s = text;
	while (*s)
	{
		switch (*s)
		{
		case 0: break;
		case '&':
			write("&amp;");
			++s;
			break;
		case '<':
			write("&lt;");
			++s;
			break;
		case '>':
			write("&gt;");
			++s;
			break;
		case '"':
			write("&quot;");
			++s;
			break;
		default: 
			write(*s);
			++s;
			break;
		}
	}
}

void CSciXmlWriter::text_output(CStringA &strRet, const char * text)
{
	strRet.Empty();

	const char * s = text;

	//����Ƿ���ת���ַ�
	BOOL IsEscapeChar = FALSE;
	while (*s)
	{
		switch (*s)
		{
		case 0: break;
		case '&':
			IsEscapeChar = TRUE;
			break;
		case '<':
			IsEscapeChar = TRUE;
			break;
		case '>':
			IsEscapeChar = TRUE;
			break;
		case '"':
			IsEscapeChar = TRUE;
			break;
		default:
			break;
		}

		if(IsEscapeChar)
			break;

		++s;
	}

	if(!IsEscapeChar)
	{
		strRet = text;
		return;
	}

	s = text;
	while (*s)
	{
		switch (*s)
		{
		case 0: break;
		case '&':
			strRet += "&amp;";
			++s;
			break;
		case '<':
			strRet += "&lt;";
			++s;
			break;
		case '>':
			strRet += "&gt;";
			++s;
			break;
		case '"':
			strRet += "&quot;";
			++s;
			break;
		default: 
			strRet += *s;
			++s;
			break;
		}
	}
}

void CSciXmlWriter::text_output_indent(int depth)
{
	for (int i=0; i<depth; i++)
	{
		write('\t');
	}
}

//////////////////////////////////////////////////////////////////////////
int  CSciXmlWriter::GetPosEndLinePosition(int pos)
{
	return m_pSciWnd->sci_GetLineEndPosition(m_pSciWnd->sci_LineFromPosition(pos));
}

bool CSciXmlWriter::IsEndline(int pos)
{
	return m_pSciWnd->sci_GetCharAt(pos == '\r') && m_pSciWnd->sci_GetCharAt(pos+1) == '\n';
}

bool CSciXmlWriter::IsWhiteSpace(char ch)
{
	return ch == '\t' || ch == 0x20;
}

bool CSciXmlWriter::IsEndDoc(char ch)
{
	return ch == 0;
}
//////////////////////////////////////////////////////////////////////////
void CSciXmlWriter::AddNode(xml_node node)
{
	CSciWnd &sci = *m_pSciWnd;

	int pos = -1;
	int depth = 0;
	xml_node nodeParent = node.parent();
	if(nodeParent)
	{
		//�ҵ����׵�depth
		int open_start = nodeParent.get_open_start_pos();
		int linepos = sci.sci_PositionFromLine(sci.sci_LineFromPosition(open_start));
		depth = open_start - linepos; //����Ǹ��׵�++

		//���ڵ�ֻ��һ���ող�����ӽڵ㣬��ô�����ڵ���Ҫ�ĳ����ģʽ <NodeName>.....</NodeName>
		if(nodeParent.first_child() == nodeParent.last_child())
		{
			//��������Ƿ��Ѿ��� </NodeName>
			if(nodeParent.get_close_start_pos() < 0 && nodeParent.get_close_end_pos() < 0)
			{
				int parent_offset = 0;
				int posx = nodeParent.get_open_end_pos();
				//ɾ�� />
				sci.sci_DeleteRange(posx - 2, 2);
				parent_offset -= 2;
				posx -= 2;
			
				InitWriter(posx);
				write('>');
				nodeParent.set_open_end_pos(GetCurPos());
				writeEndLine();
				text_output_indent(depth);
				node_output_close(nodeParent);
				AdjustElseNodePos(nodeParent, GetStartPos(), GetOffset() + parent_offset);
				flush();

				pos = nodeParent.get_open_end_pos();
			}
			else
			{
				pos = nodeParent.get_open_end_pos();
			}
		}
		else if(node.previous_sibling())
		{
			xml_node nodeprev = node.previous_sibling();
			if(nodeprev.get_close_end_pos() >= 0)
				pos = nodeprev.get_close_end_pos();
			else
				pos = nodeprev.get_open_end_pos();
		}
		else
		{
			pos = nodeParent.get_open_end_pos();
		}
	}
	
	//�����ֹ������ϰ�ߣ�����һ��������β����س���Ȼ���������֡�
	//����ǣ���һ�еĻ��з�����������ģ�ԭ���Ļ��з����ܵ�����һ��
	//����������ģʽ����
	//���Һ��Ե�����Ŀո��ٽ��л���   
	//while (IsWhiteSpace(sci.sci_GetCharAt(pos))) pos++;

	InitWriter(pos, depth+1, TRUE, FALSE);
	print(node);

	//���������pos
	AdjustElseNodePos(node, GetStartPos(), GetOffset());
}

void CSciXmlWriter::DeleteNode(xml_node node)
{
	//ɾ����һ�еĻ��з��ţ�����ɾ���еĻ��з���
	CSciWnd &sci = *m_pSciWnd;

	//���ڵ�ֻ����ôһ���ӽڵ㣬ɾ��֮�󣬸��ڵ���Ҫ�ĳ����ģʽ <NodeName />
	xml_node nodeParent = node.parent();
	if(nodeParent && nodeParent.first_child() == nodeParent.last_child())
	{
		int start = nodeParent.get_open_end_pos() -1;
		int end = nodeParent.get_close_end_pos();

		sci.sci_DeleteRange(start, end-start);
		
		InitWriter(start);
		if(nodeParent.first_attribute())
			write("/>");
		else
			write(" />");

		nodeParent.set_open_end_pos(GetCurPos());
		nodeParent.set_close_start_pos(-1);
		nodeParent.set_close_end_pos(-1);
		AdjustElseNodePos(nodeParent, start, start - end + GetOffset());
		flush();
	}
	else
	{
		int start = 0;
		int end = 0;
		if(node.previous_sibling())
		{
			xml_node nodeprev = node.previous_sibling();
			start = nodeprev.get_open_end_pos();
			if(nodeprev.get_close_end_pos() >= 0)
			{
				start = nodeprev.get_close_end_pos();
			}

			end = node.get_open_end_pos();
			if(node.get_close_end_pos() >= 0)
			{
				end = node.get_close_end_pos();
			}

			sci.sci_DeleteRange(start, end-start);
			AdjustElseNodePos(node, start, start - end);
		}
		else
		{		
			if(nodeParent)
			{
				start = nodeParent.get_open_end_pos();
			}
			else
			{
				start = node.get_open_start_pos();
			}
			end = node.get_open_end_pos();
			if(node.get_close_end_pos() >= 0)
			{
				end = node.get_close_end_pos();
			}

			sci.sci_DeleteRange(start, end-start);
			AdjustElseNodePos(node, start, start - end);
		}
		flush();
	}
}

void CSciXmlWriter::AdjustElseNodePos(xml_node node1, int pos, int offset)
{
	//�򵥴ֱ������������ĵ�ȥ����POS, �Ժ���Ż�
	xml_node root = node1.root();
	xml_node node = root;
	do 
	{
		if(node.type() == node_document)
		{
			node = node.first_child();
			continue;
		}
		else
		{
			if(node != node1)
			{
				if(pos < node.get_open_start_pos())
					node.set_open_start_pos(node.get_open_start_pos() + offset);
				if(pos < node.get_open_end_pos())
					node.set_open_end_pos(node.get_open_end_pos() + offset);

				if(pos < node.get_close_start_pos())
					node.set_close_start_pos(node.get_close_start_pos() + offset);
				if(pos < node.get_close_end_pos())
					node.set_close_end_pos(node.get_close_end_pos() + offset);

				for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
				{
					if(pos < attr.get_name_start_pos())
						attr.set_name_start_pos(attr.get_name_start_pos() + offset);
					if(pos < attr.get_name_end_pos())
						attr.set_name_end_pos(attr.get_name_end_pos() + offset);

					if(pos < attr.get_value_start_pos())
						attr.set_value_start_pos(attr.get_value_start_pos() + offset);
					if(pos < attr.get_value_end_pos())
						attr.set_value_end_pos(attr.get_value_end_pos() + offset);
				}
			}

			if(node.first_child())
			{
				node = node.first_child();
				continue;
			}
		}

		while (node != root)
		{
			if(node.next_sibling())
			{
				node = node.next_sibling();
				break;
			}
			node = node.parent();
		}
	} while (node != root);
}

void CSciXmlWriter::AdjustElseAttributesPos(xml_node node, xml_attribute attr1, int pos, int offset)
{
	node.set_open_end_pos(node.get_open_end_pos() + offset);
	if(pos < node.get_close_start_pos())
		node.set_close_start_pos(node.get_close_start_pos() + offset);
	if(pos < node.get_close_end_pos())
		node.set_close_end_pos(node.get_close_end_pos() + offset);

	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		if(attr == attr1)
			continue;

		if(pos < attr.get_name_start_pos())
			attr.set_name_start_pos(attr.get_name_start_pos() + offset);
		if(pos < attr.get_name_end_pos())
			attr.set_name_end_pos(attr.get_name_end_pos() + offset);

		if(pos < attr.get_value_start_pos())
			attr.set_value_start_pos(attr.get_value_start_pos() + offset);
		if(pos < attr.get_value_end_pos())
			attr.set_value_end_pos(attr.get_value_end_pos() + offset);
	}
}

void CSciXmlWriter::AddAttribute(xml_node node, xml_attribute attr)
{
	//�Ѿ����ڵ�attr���޸�
	if(attr.get_name_start_pos() >= 0) 
		return ModifyAttribute(node, attr);

	CSciWnd &sci = *m_pSciWnd;

	//�����ǰһ����attr�������������
	xml_attribute attrprev = attr.previous_attribute();
	if(attrprev)
	{
		InitWriter(attrprev.get_value_end_pos() + 1);
		write(' ');

		//��ʽ������
		node_output_attribute(attr);

		AdjustElseAttributesPos(node, attr, GetStartPos(), GetOffset());
		AdjustElseNodePos(node, GetStartPos(), GetOffset());
		flush();
	}
	else //û��ǰһ��attr����node����ֱ�Ӳ���
	{
		int pos = node.get_open_start_pos();
		pos += strlen(node.name()) +1;

		//��ʽ������
		InitWriter(pos);
		write(' ');
		node_output_attribute(attr);
		AdjustElseAttributesPos(node, attr, GetStartPos(), GetOffset());
		AdjustElseNodePos(node, GetStartPos(), GetOffset());
		flush();
	}	
}

void CSciXmlWriter::ModifyAttribute(xml_node node, xml_attribute attr)
{
	CSciWnd &sci = *m_pSciWnd;
	
	//��ɾ��������Ӻ���������� undo �¼�
// 	int start = attr.get_value_start_pos();
// 	int end = attr.get_value_end_pos();
// 	sci.sci_DeleteRange(start, end-start);
// 
// 	InitWriter(start);
// 	text_output(XML2UTF8(attr.value()));
// 
// 	attr.set_value_end_pos(GetCurPos());
// 	int offset = GetOffset() - (end - start);
// 	if(offset != 0)
// 	{
// 		AdjustElseAttributesPos(node, attr, GetStartPos(), offset);
// 		AdjustElseNodePos(node, GetStartPos(), offset);
// 	}
// 	flush();
	

	//////////////////////////////////////////////////////////////////////////
	//�ĳ�ֱ���滻

	int start = attr.get_value_start_pos();
	int end = attr.get_value_end_pos();

	CStringA strRet;
	text_output(strRet, XML2UTF8(attr.value()));

	sci.sci_SetTargetStart(start);
	sci.sci_SetTargetEnd(end);
	sci.sci_ReplaceTarget(-1, strRet);
	sci.sci_GoToPos(start + strRet.GetLength());

	attr.set_value_end_pos(start + strRet.GetLength());
	int offset = strRet.GetLength() - (end - start);
	if(offset != 0)
	{
		AdjustElseAttributesPos(node, attr, start, offset);
		AdjustElseNodePos(node, start, offset);
	}
}

void CSciXmlWriter::DeleteAttribute(xml_node node, xml_attribute attr)
{
	CSciWnd &sci = *m_pSciWnd;

	if(attr.get_name_start_pos() < 0 || attr.get_value_end_pos() < 0)
		return;

	int start = attr.get_name_start_pos() - 1;
	int end = attr.get_value_end_pos() + 1;
	sci.sci_DeleteRange(start, end-start);

	AdjustElseAttributesPos(node, attr, start, start-end);
	AdjustElseNodePos(node, start, start-end);
	flush();
}
