#pragma once

#include "Config.h"
#include "MineBlock.h"
#include "GameManager.h"

class MyButton : public CButton,public MineBlock {
	DECLARE_DYNAMIC(MyButton)

public:
	MyButton(GameManager *manager);
	virtual ~MyButton();
	virtual void init(GameManager *manager);
	virtual Status getStatus();
	virtual void performSweep();
	virtual void mark();
	virtual void reset();
	virtual void detect();
	virtual BOOL isMine();
	virtual void setMine();
	virtual void setCount(int count);
	virtual void performGameOver();
	virtual void performRestartGame(int x, int y);


	virtual afx_msg void UpdateWindow();
	virtual afx_msg void RedrawWindow();

protected:
	virtual void switchToMark();
	virtual void switchToClear();
	virtual void switchToDefault();
	virtual void switchToBoom();
	virtual void switchToDetect();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	void DrawBitMap(int resId,BOOL and);
	void performRedraw();
	BOOL lDown;
	BOOL mDown;
	BOOL tracking;
};


