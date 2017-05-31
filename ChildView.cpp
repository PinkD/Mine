
#include "stdafx.h"
#include "LearnMFC.h"
#include "ChildView.h"
#include "Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView(){
	testButtons = new CArray<CArray<MyButton*>*>;
	manager = new GameManager(testButtons);
	for (int i = 0;i < COUNT_PER_LINE;i++){
		testButtons->Add(new CArray<MyButton*>());
	}
	for (int i = 0;i < COUNT_PER_LINE;i++){
		for (int j = 0;j < COUNT_PER_LINE;j++){
			testButtons->GetAt(i)->Add(new MyButton(manager));
		}
	}
	manager->restartGame();
}

CChildView::~CChildView(){
	delete testButtons;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	return TRUE;
}

void CChildView::OnPaint() {
	CPaintDC dc(this);
	CRect rect(0,0 + TOOLBAR_HEIGHT, MINE_WIDTH, MINE_WIDTH + TOOLBAR_HEIGHT);
	CString s;
	for (int i = 0;i < COUNT_PER_LINE;i++){
		rect.left = 0;
		rect.right = MINE_WIDTH;			
		for (int j = 0;j < COUNT_PER_LINE;j++){
			int id = i*10+j;
			//s.Format(_T("%d"),i*10+j);
			s.Format(_T("%d"),(i+1)*10+j+1);
			testButtons->GetAt(i)->GetAt(j)->Create(NULL,WS_CHILD | WS_VISIBLE | BS_BITMAP, rect, this, id);
			rect.left+=MINE_WIDTH;
			rect.right+=MINE_WIDTH;
		}
		rect.top += MINE_WIDTH;
		rect.bottom += MINE_WIDTH;
	}
	
	//CRect rect;
	//GetClientRect(rect);
	//dc.DrawText(_T("test"),rect,DT_CENTER);
}
