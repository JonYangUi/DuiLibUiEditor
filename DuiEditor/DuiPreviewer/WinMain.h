#pragma once
class CWinMain
{
public:
	CWinMain(HINSTANCE hInstance);
	~CWinMain(void);

	int Run();
	void CenterWindow();
	static LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static CString m_strSelectFile;		// ��ǰѡ����ļ�
private:
	HWND m_hWndButton;
private:
	static HINSTANCE m_hInst;			// ��ǰʵ��

	HWND m_hWnd;
	TCHAR m_szTitle[255];				// �������ı�
	TCHAR m_szWindowClass[255];			// ����������
};

