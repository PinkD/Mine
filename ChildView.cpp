
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
	manager = new GameManager(testButtons, this);
	for (int i = 0;i < COUNT_PER_LINE;i++){
		testButtons->Add(new CArray<MyButton*>());
	}
	for (int i = 0;i < COUNT_PER_LINE;i++){
		for (int j = 0;j < COUNT_PER_LINE;j++){
			testButtons->GetAt(i)->Add(new MyButton(manager));
		}
	}
	init = FALSE;
}

CChildView::~CChildView(){
	delete testButtons;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_RESTART_GAME, &CChildView::OnRestartGameSelected)
	ON_UPDATE_COMMAND_UI(ID_GAME_CHEATMODE, &CChildView::OnUpdateCheatModeState)
	ON_COMMAND(ID_GAME_CHEATMODE, &CChildView::OnCheatModeChanged)
	ON_COMMAND(ID_GAME_AUTOMARK, &CChildView::OnAutoMarkStateChanged)
	ON_UPDATE_COMMAND_UI(ID_GAME_AUTOMARK, &CChildView::OnAutoMarkStateUpdate)
END_MESSAGE_MAP()


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	manager->restartGame();
	return TRUE;
}

void CChildView::OnPaint() {
	CPaintDC dc(this);
	CRect rect1;
	GetClientRect(rect1);
	CPen pen(PS_SOLID, MINE_WIDTH, 0xDDDDDD);
	CPen *old = dc.SelectObject(&pen);
	dc.MoveTo(rect1.right / 2, 0);
	dc.LineTo(rect1.right / 2, TOOLBAR_HEIGHT);
	dc.SelectObject(old);
	CRect rect(0,0 + TOOLBAR_HEIGHT, MINE_WIDTH, MINE_WIDTH + TOOLBAR_HEIGHT);
	for (int i = 0;i < COUNT_PER_LINE;i++){
		rect.left = 0;
		rect.right = MINE_WIDTH;			
		for (int j = 0;j < COUNT_PER_LINE;j++){
			int id = i*10+j;

			if (init){
				testButtons->GetAt(i)->GetAt(j)->UpdateWindow();
			}else{
				testButtons->GetAt(i)->GetAt(j)->Create(NULL,WS_CHILD | WS_VISIBLE | BS_BITMAP, rect, this, id);
			}
			rect.left+=MINE_WIDTH;
			rect.right+=MINE_WIDTH;
		}
		rect.top += MINE_WIDTH;
		rect.bottom += MINE_WIDTH;
	}
	SetTimer(TIMER_ID,1000,NULL);
	updateDrawTimer();
	updateDrawCount(testButtons->GetSize());
	init = TRUE;
}


void CChildView::OnRestartGameSelected() {
	manager->restartGame();
}

void CChildView::OnTimer(UINT_PTR nIDEvent){
	if (nIDEvent == TIMER_ID) {
		updateDrawTimer();
	}
}

void CChildView::updateDrawTimer(){
	if (manager->isGameOver()) {
		return;
	}
	CClientDC dc(this);
	CRect rect;
	GetClientRect(rect);
	rect.right = (rect.right - MINE_WIDTH) / 2;
	rect.bottom = TOOLBAR_HEIGHT;
	dc.DrawText(_T("   0   "), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	CString s;
	s.Format(_T("%d"), manager->getAndPlusOneSecond());
	dc.DrawText(s, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CChildView::updateDrawCount(int count){
	if (!IsWindow(*this) || manager->isGameOver()) {
		return;
	}
	CClientDC dc(this);
	CRect rect;
	GetClientRect(rect);
	rect.left = (rect.right + MINE_WIDTH) / 2;
	rect.bottom = TOOLBAR_HEIGHT;
	dc.DrawText(_T("   0   "), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	CString s;
	s.Format(_T("%d"), count);
	dc.DrawText(s, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}


void CChildView::OnUpdateCheatModeState(CCmdUI *pCmdUI) {
	pCmdUI->SetCheck(manager->isCheatModeEnabled());
}

void CChildView::OnCheatModeChanged() {
	manager->switchCheatMode();
}

void CChildView::OnAutoMarkStateUpdate(CCmdUI *pCmdUI) {
	pCmdUI->SetCheck(manager->isAutoMarkEnabled());
	pCmdUI->Enable(!manager->isCheatModeEnabled());
}

void CChildView::OnAutoMarkStateChanged() {
	manager->switchAutoMark();
}
