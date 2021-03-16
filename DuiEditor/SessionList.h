#pragma once

//CSessionList, �������ļ�
class CSessionList
{
public:
	CSessionList(void);
	~CSessionList(void);

	void Init();
	BOOL Save();

	//��ȡFile�ڵ�
	xml_node GetSession(LPCTSTR lpszPathName);

	//����������Ҫ���ļ���·��
	CString GetSessionFile(LPCTSTR lpszPathName);

	//����backup����
	CString GetSessionBackup(LPCTSTR lpszPathName);

	//ɾ��File�ڵ�
	void DeleteSession(LPCTSTR lpszPathName);

	void ProcessSessionFileList();
private:
	xml_document xml;
};

extern CSessionList g_session;