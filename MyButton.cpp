
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
	gameover = FALSE;
	mine = FALSE;
	status = STATUS_DEFAULT;
}

MyButton::~MyButton(){
	
}

BEGIN_MESSAGE_MAP(MyButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()

afx_msg void MyButton::UpdateWindow(){
	CButton::UpdateWindow();
	performRedraw();
}

/*
afx_msg void MyButton::RedrawWindow(){
	CButton::RedrawWindow();
}
*/

afx_msg void MyButton::OnMouseMove(UINT nFlags, CPoint point){
	//mark();
}

afx_msg void MyButton::OnLButtonDown(UINT nFlags, CPoint point){
	if (gameover){
		return;
	}
	if (status == STATUS_DEFAULT){
		CButton::OnLButtonDown(nFlags,point);
		lDown = TRUE;
	}
}

afx_msg void MyButton::OnLButtonUp(UINT nFlags, CPoint point){
	CButton::OnLButtonUp(nFlags,point);
	if (POINT_VALID(&point)) {
		manager->sweep(x, y);
	}
}

afx_msg void MyButton::OnRButtonDown(UINT nFlags, CPoint point){
	mark();
}

afx_msg void MyButton::OnMButtonDown(UINT nFlags, CPoint point){
	//TODO to be continued
}

afx_msg void MyButton::OnMButtonUp(UINT nFlags, CPoint point){
	//switchToBoom();
	//TODO to be continued
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
		}
		drawStatus();
	}
}

void MyButton::mark(){
	switch (status){
	case STATUS_DEFAULT:
		switchToMark();
		break;
	case STATUS_MARK:
		switchToDefault();
		break;
	default:
		break;
	}
}


void MyButton::detect(){
	switchToDetect();
}

void MyButton::reset(){
	switchToDefault();
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
	status = STATUS_DEFAULT;
	//if (gameover){
		performRedraw();
	//}
	mine = FALSE;
	gameover = FALSE;
	//DrawBitMap(NULL);
}


//switch status with icon change

void MyButton::switchToMark(){
	if (!gameover){
		//SetIcon(LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		//SetBitmap(LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BOOM)));
		status = STATUS_MARK;
		drawStatus();
	}
}

void MyButton::switchToClear(){
	if (!gameover){
		status = STATUS_CLEAR;
		drawStatus();
		if (count == 0){
			manager->sweep(x,y,count);
		}
	}

}

void MyButton::switchToDefault(){
	if (!gameover){
		status = STATUS_DEFAULT;
		CButton::RedrawWindow();
	}

}

void MyButton::switchToBoom(){
	if (!gameover){
		status = STATUS_BOOM;
		DrawBitMap(IDB_BOOM, FALSE);
	}
}

void MyButton::switchToDetect(){
	if (!gameover){
		//SetBitmap(LoadBitmap(IDB_BITMAP1));
	}
}


//private functions

void MyButton::DrawBitMap(int resId,BOOL and){
	HBITMAP hbitmap = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(resId));
	CBitmap hbmp;
	hbmp.Attach(hbitmap);
	BITMAP bitmap;
	hbmp.GetBitmap(&bitmap);
	CDC dcMem;
	//创建兼容DC
	dcMem.CreateCompatibleDC(GetDC());
	CBitmap *pOldBitmap=(CBitmap*)dcMem.SelectObject(hbmp);
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


	//dcMem.SelectObject(pOldBitmap);
	//SetBitmap(hbitmap);
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
			CButton::RedrawWindow();
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
	case STATUS_DEFAULT:
		CButton::RedrawWindow();
		break;
	}
}