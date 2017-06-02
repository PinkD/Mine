
#pragma once

#include "MyButton.h"

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();

protected:
	CArray<CArray<MyButton*>*> *testButtons;
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	GameManager *manager;
	BOOL init;
public:
	afx_msg void OnRestartGameSelected();
};

