
#pragma once

#include "MyButton.h"

class CChildView : public CWnd, public UpdateDrawCountCallBack
{
public:
	CChildView();
	virtual ~CChildView();

protected:
	CArray<CArray<MyButton*>*> *testButtons;
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnTimer(UINT_PTR nIDEvent);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	void updateDrawTimer();
	GameManager *manager;
	BOOL init;
public:
	virtual void updateDrawCount(int count);
	afx_msg void OnRestartGameSelected();
	afx_msg void OnUpdateCheatModeState(CCmdUI *pCmdUI);
	afx_msg void OnCheatModeChanged();
	afx_msg void OnAutoMarkStateChanged();
	afx_msg void OnAutoMarkStateUpdate(CCmdUI *pCmdUI);
};

