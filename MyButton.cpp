
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
	ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()

/*
afx_msg BOOL MyButton::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){
	switch(message){
	case WM_LBUTTONDOWN:

		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	}
	return CButton::OnWndMsg(message, wParam, lParam, pResult);
}
*/

afx_msg void MyButton::OnNcMouseMove(UINT nFlags, CPoint point){
	//if ((nFlags & WM_LBUTTONDOWN) !=0){
	//	lDown = FALSE;
	//	switchToMark();
	//}
}

afx_msg void MyButton::OnMouseMove(UINT nFlags, CPoint point){
	//switchToClear();
}

afx_msg void MyButton::OnLButtonDown(UINT nFlags, CPoint point){
	lDown = TRUE;
	CButton::OnLButtonDown(nFlags,point);
}

afx_msg void MyButton::OnLButtonUp(UINT nFlags, CPoint point){
	if (lDown){
		switchToClear();
	}else{
		CButton::OnLButtonUp(nFlags,point);
	}
}

afx_msg void MyButton::OnRButtonDown(UINT nFlags, CPoint point){
	mark();
}

afx_msg void MyButton::OnMButtonDown(UINT nFlags, CPoint point){
	//TODO to be continued
}

afx_msg void MyButton::OnMButtonUp(UINT nFlags, CPoint point){
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
	gameover = FALSE;
	//DrawBitMap(NULL);
}

void MyButton::DrawBitMap(int resId){
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
		SRCAND);																// raster operation
	

	//dcMem.SelectObject(pOldBitmap);
	//SetBitmap(hbitmap);
}


//switch status with icon change

void MyButton::switchToMark(){
	if (!gameover){
		DrawBitMap(IDB_MARK);
		//SetIcon(LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		//SetBitmap(LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BOOM)));
	}
}

void MyButton::switchToClear(){
	if (!gameover){
		DrawBitMap(NUM_BITMAP[count]);
		
		//manager->sweep(x,y,count);
	}

}

void MyButton::switchToDefault(){
	if (!gameover){
		//TODO 
	}

}

void MyButton::switchToBoom(){
	if (!gameover){
		DrawBitMap(IDB_BOOM);
	}
}

void MyButton::switchToDetect(){
	if (!gameover){
		//SetBitmap(LoadBitmap(IDB_BITMAP1));
	}
}
