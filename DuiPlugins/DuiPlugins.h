// DuiPlugins.h : DuiPlugins DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDuiPluginsApp
// �йش���ʵ�ֵ���Ϣ������� DuiPlugins.cpp
//

class CDuiPluginsApp : public CWinApp
{
public:
	CDuiPluginsApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
