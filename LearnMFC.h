
// LearnMFC.h : LearnMFC Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CLearnMFCApp:
// �йش����ʵ�֣������ LearnMFC.cpp
//

class CLearnMFCApp : public CWinApp
{
public:
	CLearnMFCApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLearnMFCApp theApp;
