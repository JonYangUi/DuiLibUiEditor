#pragma once
#include "Resource.h"
// CSciFind �Ի���

#define FIND_RANGE_CURRENT	0		//���ҵ�ǰ�ĵ�
#define FIND_RANGE_OPENED	1		//���ҵ�ǰ�Ѿ��򿪵��ĵ�
#define FIND_RANGE_PROJECT	2		//����������Ŀ

#define FIND_DIRECT_DOWN	0
#define FIND_DIRECT_UP		1
#define FIND_DIRECT_ALL		2

class CSciFind : public CDialogEx
{
	DECLARE_DYNAMIC(CSciFind)

public:
	CSciFind(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSciFind();

// �Ի�������
	enum { IDD = IDD_SCI_FIND };

	BOOL sciFind(CSciWnd *pSciWnd);
	BOOL sciReplaceCurrent(CSciWnd *pSciWnd);
	int sciReplaceAll(CSciWnd *pSciWnd);

	void SaveText();
	void LoadText();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual void PostNcDestroy();
	afx_msg void OnBtnRepaceCurrent();
	afx_msg void OnBtnReplaceAll();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
};
