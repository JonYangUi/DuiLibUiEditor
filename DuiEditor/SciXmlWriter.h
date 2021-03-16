#pragma once


class CSciUndoBlock //�����������ϲ�Ϊһ��undo����
{
public:
	CSciUndoBlock(CSciWnd *pSciWnd) : m_pSciWnd(pSciWnd)
	{
		m_pSciWnd->sci_BeginUndoAction();
		//InsertMsg(_T("Begin Undo"));
	}
	~CSciUndoBlock()
	{
		m_pSciWnd->sci_EndUndoAction();
		//InsertMsg(_T("End Undo"));
	}
	CSciWnd *m_pSciWnd;
};

class CSciXmlWriter
{
public:
	CSciXmlWriter(CSciWnd *pSciWnd);
	~CSciXmlWriter(void);

	void InitWriter(int beginPos, int beginDepth = 0, BOOL bRootBeginNeedEndLine = FALSE, BOOL bRootEndNeedEndLine = TRUE);
	const char *GetOutputBuffer();
	int GetOutputLength();
	void print(xml_node node);
	int GetStartPos() { return m_nBeginPos; }
	int GetCurPos() { return m_nBeginPos + m_strTempBuffer.GetLength(); }
	int GetOffset() { return m_strTempBuffer.GetLength(); }

protected:
	void write(const char *text);
	void write(char text);
	void writeEndLine();
	void flush();
	void node_output(xml_node root, xml_node node, int &pos, int &depth);
	void node_output_attribute(xml_attribute attr);
	void node_output_all_attributes(xml_node node);
	void node_output_close(xml_node node);
	void text_output(const char * text);
	void text_output(CStringA &strRet, const char * text);
	void text_output_indent(int depth);

private:
	CSciWnd *m_pSciWnd;
	int m_nBeginPos;
	int m_nBeginDepth;

	CStringA m_strTempBuffer;
	BOOL m_bRootBeginNeedEndLine;	//root��ʼ�Ƿ���
	BOOL m_bRootEndNeedEndLine;		//root�����Ƿ���
protected:
	int  GetPosEndLinePosition(int pos); //��ȡpos��β��λ��
	bool IsEndline(int pos);	//�ж��Ƿ���ĩβ
	bool IsWhiteSpace(char ch);
	bool IsEndDoc(char ch);
public:
	//��ƽ������������ԣ���ʱ����XML
	void AddAttribute(xml_node node, xml_attribute attr);
	//��ƽ������޸����ԣ���ʱ����XML
	void ModifyAttribute(xml_node node, xml_attribute attr);
	//��ƽ�����ɾ�����ԣ���ʱ����XML
	void DeleteAttribute(xml_node node, xml_attribute attr);

	//��ƽ��������ӿؼ�����ʱ����XML
	void AddNode(xml_node node);
	//��ƽ�����ɾ���ؼ�����ʱ����XML
	void DeleteNode(xml_node node);

	//ʹnode���µ�pos������ݼ�
	void AdjustElseNodePos(xml_node node, int pos, int offset);
	//ʹnode��attr���µ�pos������ݼ�
	void AdjustElseAttributesPos(xml_node node, xml_attribute attr, int pos, int offset);
};

