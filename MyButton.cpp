
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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()


afx_msg void MyButton::OnLButtonDown(UINT nHitTest, CPoint point){
	lDown = TRUE;
	CButton::OnLButtonDown(nHitTest,point);
}

afx_msg void MyButton::OnLButtonUp(UINT nHitTest, CPoint point){
	CButton::OnLButtonUp(nHitTest,point);
}

afx_msg void MyButton::OnRButtonDown(UINT nHitTest, CPoint point){
	mark();
}

afx_msg void MyButton::OnMButtonDown(UINT nHitTest, CPoint point){
	//TODO to be continued
}

afx_msg void MyButton::OnMButtonUp(UINT nHitTest, CPoint point){
	//TODO to be continued
}

/*
//interface MineBlock
virtual Status getStatus()=0;
virtual Status sweep()=0;
virtual void mark()=0;
virtual void reset()=0;
virtual void detect()=0;
virtual BOOL isMine()=0;
virtual void setMine()=0;
virtual void gameOver()=0;
*/

Status MyButton::getStatus(){
	return status;
}

Status MyButton::sweep(int count){
	if (status == STATUS_DEFAULT){
		if (mine){
			switchToBoom();
		}else{
			switchToClear(count);
		}
	}
	return status;
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
void MyButton::gameOver(){
	gameover = TRUE;
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

/*
//switch status with icon change
virtual void switchToMark();
virtual void switchToClear();
virtual void switchToDefault();
virtual void switchToBoom();
virtual void switchToDetect();
*/

void MyButton::switchToMark(){
	if (!gameover){
		DrawBitMap(IDB_MARK);
		//SetIcon(LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		//SetBitmap(LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BOOM)));
	}
}

void MyButton::switchToClear(int count){
	if (!gameover){
		DrawBitMap(IDB_NUM_2);
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
