
#include "stdafx.h"
#include "LearnMFC.h"
#include "MyButton.h"
#include "Resource.h"


IMPLEMENT_DYNAMIC(MyButton, CButton)

void MyButton::init(GameManager *manager){
	this->manager = manager;
}

MyButton::MyButton(GameManager *manager) {
	init(manager);
}

MyButton::~MyButton(){
	
}

BEGIN_MESSAGE_MAP(MyButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

afx_msg void MyButton::UpdateWindow(){
	CButton::UpdateWindow();
	performRedraw();
}

afx_msg void MyButton::OnLButtonDown(UINT nFlags, CPoint point){
	if (gameover){
		return;
	}
	if (status == STATUS_DEFAULT){
		lDown = TRUE;
		CButton::OnLButtonDown(nFlags,point);
	}
}

afx_msg void MyButton::OnLButtonUp(UINT nFlags, CPoint point){
	if (status != STATUS_DEFAULT){
		return;
	}
	CButton::OnLButtonUp(nFlags, point);
	lDown = FALSE;
	if (POINT_VALID(&point)) {
		manager->sweep(x, y);
 	}
}

afx_msg void MyButton::OnRButtonDown(UINT nFlags, CPoint point){
	mark();
}

afx_msg void MyButton::OnMButtonUp(UINT nFlags, CPoint point){
	if (status != STATUS_CLEAR){
		return;
	}
	if (POINT_VALID(&point)) {
		detect();
	}
}


//interface MineBlock

Status MyButton::getStatus(){
	return status;
}

void MyButton::performSweep(){
	if (status == STATUS_DEFAULT){
		if (mine){
			switchToBoom();
			throw status;
		}else{
			switchToClear();
			if (count == 0){
				manager->sweep(x,y,count);
			}
		}
	}
}

void MyButton::mark(){
	switch (status){
	case STATUS_DEFAULT:
		switchToMark();
		manager->mineCountSub();
		break;
	case STATUS_MARK:
		switchToDefault();
		manager->mineCountAdd();
		break;
	default:
		break;
	}
}

void MyButton::reset(){
	switchToDefault();
}

void MyButton::detect(){
	manager->sweep(x, y, count);
}

BOOL MyButton::isMine(){
	return mine;
}

void MyButton::setMine(){
	mine = TRUE;
}

void MyButton::setCount(int count){
	this->count = count;
}


void MyButton::performGameOver(){
	if (status == STATUS_DEFAULT) {
		switchToClear();
	}
	gameover = TRUE;
}

void MyButton::performRestartGame(int x, int y){
	this->x = x;
	this->y = y;
	redraw = FALSE;
	status = STATUS_DEFAULT;
	mine = FALSE;
	gameover = FALSE;
	performRedraw();
}


//switch status with icon change

void MyButton::switchToMark(){
	if (!gameover){
		if (status == STATUS_MARK){
			return;
		}
		status = STATUS_MARK;
		drawStatus();
	}
}

void MyButton::switchToClear(){
	if (!gameover){
		if (status == STATUS_CLEAR){
			return;
		}
		status = STATUS_CLEAR;
		drawStatus();
	}

}

void MyButton::switchToDefault(){
	if (!gameover){
		if (status == STATUS_DEFAULT){
			return;
		}
		status = STATUS_DEFAULT;
		RedrawWindow();
	}

}

void MyButton::switchToBoom(){
	if (!gameover){
		if (status == STATUS_BOOM){
			return;
		}
		status = STATUS_BOOM;
		DrawBitMap(IDB_BOOM, FALSE);
	}
}

//private functions

void MyButton::DrawBitMap(int resId,BOOL and){
	CBitmap hbmp;
	hbmp.LoadBitmap(resId);
	BITMAP bitmap;
	hbmp.GetBitmap(&bitmap);
	CDC dcMem;
	//创建兼容DC
	dcMem.CreateCompatibleDC(GetDC());
	//CreateCompatibleBitmap
	CBitmap *pOldBitmap = (CBitmap*)dcMem.SelectObject(hbmp);
	CRect rect;
	//获取Static控件的大小范围
	GetClientRect(rect);
	//显示位图
	GetDC()->StretchBlt(0, 0,													// destination coordinates
		rect.Width() - MINE_WIDTH / 10, rect.Height() - MINE_WIDTH / 10,		// destination dimensions  
		&dcMem,																	// source DC
		0, 0,																	// source coordinates
		bitmap.bmWidth, bitmap.bmHeight,										// source dimensions
		and ? SRCAND : SRCCOPY);												// raster operation

	dcMem.SelectObject(&pOldBitmap);
	dcMem.DeleteDC();
}

void MyButton::performRedraw(){
	switch (status){
	case STATUS_BOOM:
	case STATUS_CLEAR:
	case STATUS_MARK:
		drawStatus();
		break;
	case STATUS_DEFAULT:
		if (IsWindow(m_hWnd)) {
			RedrawWindow();
		}
		break;
	}
}
void MyButton::drawStatus() {
	switch (status){
	case STATUS_BOOM:
		DrawBitMap(IDB_BOOM, TRUE);
		break;
	case STATUS_CLEAR:
		DrawBitMap(NUM_BITMAP[count], FALSE);
		break;
	case STATUS_MARK:
		DrawBitMap(IDB_MARK, TRUE);
		break;
	}
}

void MyButton::OnPaint(){
	CButton::OnPaint();
	drawStatus();
}
