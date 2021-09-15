#pragma once
class CMyFilePath
{
public:
	CMyFilePath(void);
	~CMyFilePath(void);

	//�������Ŀ¼
	static BOOL CreateMuiltDirectory(LPCTSTR lpszPathName);

	//�淶��������·����ȥ�� ".\" �� "..\"
	static CString Canonicalize(LPCTSTR lpszPathName);

	//��֤·���Ƿ�Ϊ��Ч·���� 
	static BOOL IsValidPath(LPCTSTR lpszPathName);

	//�ļ���Ŀ¼�Ƿ����
	static BOOL IsFileExist(LPCTSTR lpszPathName);

	//ȥ���ļ������õ�Ŀ¼�� ·��ĩβ����"\"
	static CString GetPathName(LPCTSTR lpszPathName);

	//ȥ��Ŀ¼���õ��ļ���
	static CString GetFileName(LPCTSTR lpszPathName);

	//ȥ���ļ�·���е��ļ���չ��
	static CString RemoveFileExt(LPCTSTR lpszPathName);

	//�ָ�·����
	//���� lpszPathFrom = c:/a/b/1.txt, lpszPathTo = c:/a/
	//���ؽ��Ϊ b/1.txt
	static CString SplitPath(LPCTSTR lpszPathName, LPCTSTR lpszPathBase);
};

