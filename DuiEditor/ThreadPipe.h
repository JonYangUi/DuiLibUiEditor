#pragma once


extern HANDLE hChildStdoutRd; 
extern HANDLE hChildStdoutWr; 
extern HANDLE g_hSaveStdout;

// CThreadPipe
extern DWORD g_nThreadPipeID;
extern HANDLE g_hThreadPipeHandle;

class CThreadPipe : public CWinThread
{
	DECLARE_DYNCREATE(CThreadPipe)

public:
	CThreadPipe();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CThreadPipe();

public:
	static void CreatePipe();
	static void ClosePipe();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


