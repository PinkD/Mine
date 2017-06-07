#include "StdAfx.h"
#include "afxdialogex.h"
#include "GameManager.h"


GameManager::GameManager(CArray<CArray<MyButton*>*> *mineBlocks, UpdateDrawCountCallBack *callback)
	:mineBlocks(mineBlocks),callback(callback){
		autoMark = FALSE;
		cheatMode = FALSE;
		gameover = FALSE;
}


GameManager::~GameManager(void) {
}

BOOL GameManager::isWin(){
	int markCount = 0;
	int defaultCount = 0;
	for (int i = 0;i < mineBlocks->GetSize();i++) {
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			Status status = mineBlocks->GetAt(i)->GetAt(j)->getStatus();
			if (status == STATUS_MARK) {
				markCount++;
			} else if (status == STATUS_DEFAULT) {
				defaultCount++;
			}
		}
	}
	if (defaultCount + markCount == mineBlocks->GetSize()){
		return TRUE;
	}else{
		return FALSE;
	}
}

void GameManager::win(){
	if (gameover) {
		return;
	}
	gameOver();
	MessageBox(NULL, _T("You Win"), _T("Win"), NULL);
}

void GameManager::lose(){
	gameOver();
	MessageBox(NULL, _T("You Lose"), _T("Lose"), MB_ICONEXCLAMATION);
}

BOOL GameManager::isGameOver(){
	return gameover;
}

void GameManager::gameOver(){
	gameover = TRUE;
	for (int i = 0;i < mineBlocks->GetSize();i++) {
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			mineBlocks->GetAt(i)->GetAt(j)->performGameOver();
		}
	}

}

void GameManager::restartGame(){
	gameover = FALSE;
	int *mine = new int[mineBlocks->GetSize()];
	srand(unsigned(time(0)));
	for (int i = 0;i < mineBlocks->GetSize();i++) {//create mine
		RANDOM_BETWEEN(0, (mineBlocks->GetSize() - 1) * (mineBlocks->GetSize() - 1), mineBlocks->GetSize(), mine);
	}
	for (int i = 0;i < mineBlocks->GetSize();i++) {//set mine
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			mineBlocks->GetAt(i)->GetAt(j)->performRestartGame(i,j);
			for (int k = 0;k < COUNT_PER_LINE;k++) {
				if (mine[k] == POINT_TO_POSITION(i,j)) {
					mineBlocks->GetAt(i)->GetAt(j)->setMine();
					break;
				}
			}
		}
	}
	for (int i = 0;i < mineBlocks->GetSize();i++) {//set count
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			if (mineBlocks->GetAt(i)->GetAt(j)->isMine()){
				mineBlocks->GetAt(i)->GetAt(j)->setCount(9);
			}else{
				mineBlocks->GetAt(i)->GetAt(j)->setCount(count(i, j));
			}
		}
	}
	delete[] mine;
	mineCount = mineBlocks->GetSize();
	updateDrawCount();
	resetSecond();
}

void GameManager::sweep(int x, int y){
	try{
		mineBlocks->GetAt(x)->GetAt(y)->performSweep();
		if (autoMark) {
			performAutoMark();
			//autoMarkBlock(x, y, mineBlocks->GetAt(x)->GetAt(y)->count);
		}
		if (isWin()){
			win();
		}
	}catch (Status status){
		if (status == STATUS_BOOM) {
			lose();
		}
	}
}

void GameManager::sweep(int x, int y, int count){//recursion able
	int mineCountAround = 0;

	BOOL left = (x != 0);
	BOOL right = (x != mineBlocks->GetSize() - 1);
	BOOL top = (y != 0);
	BOOL bottom = (y != mineBlocks->GetAt(x)->GetSize() - 1);
	if (left && mineBlocks->GetAt(x - 1)->GetAt(y)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (right && mineBlocks->GetAt(x + 1)->GetAt(y)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (top && mineBlocks->GetAt(x)->GetAt(y -1)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (bottom && mineBlocks->GetAt(x)->GetAt(y + 1)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (left && top && mineBlocks->GetAt(x - 1)->GetAt(y - 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}
	if (right && top && mineBlocks->GetAt(x + 1)->GetAt(y - 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}
	if (left && bottom && mineBlocks->GetAt(x - 1)->GetAt(y + 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}
	if (right && bottom && mineBlocks->GetAt(x + 1)->GetAt(y + 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}
	if (mineCountAround != count){
		return;
	}

	try{
		if (left && !gameover){
			mineBlocks->GetAt(x - 1)->GetAt(y)->performSweep();
		}
		if (right && !gameover){
			mineBlocks->GetAt(x + 1)->GetAt(y)->performSweep();
		}
		if (top && !gameover){
			mineBlocks->GetAt(x)->GetAt(y -1)->performSweep();
		}
		if (bottom && !gameover){
			mineBlocks->GetAt(x)->GetAt(y + 1)->performSweep();
		}
		if (left && top && !gameover) {
			mineBlocks->GetAt(x - 1)->GetAt(y - 1)->performSweep();
		}
		if (right && top && !gameover) {
			mineBlocks->GetAt(x + 1)->GetAt(y - 1)->performSweep();
		}
		if (left && bottom && !gameover) {
			mineBlocks->GetAt(x - 1)->GetAt(y + 1)->performSweep();
		}
		if (right && bottom && !gameover) {
			mineBlocks->GetAt(x + 1)->GetAt(y + 1)->performSweep();
		}
		if (autoMark) {
			performAutoMark();
			//autoMarkBlock(x, y, count);
		}
		if (isWin()){
			win();
		}
	}catch (Status status){
		if (status == STATUS_BOOM) {
			lose();
		}
	}
}

void GameManager::performAutoMark(){
	int changed = 0;
	for (int i = 0;i < mineBlocks->GetSize();i++) {
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			if (mineBlocks->GetAt(i)->GetAt(j)->getStatus() == STATUS_CLEAR) {
				if (autoMarkBlock(i, j, mineBlocks->GetAt(i)->GetAt(j)->count)) {
					changed++;
				}
			}
		}
	}
	if (changed) {
		performAutoMark();
		if (cheatMode) {
			mapSweep();
		}
	}

}

void GameManager::mapSweep(){
	for (int i = 0;i < mineBlocks->GetSize();i++) {
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			if (mineBlocks->GetAt(i)->GetAt(j)->getStatus() == STATUS_CLEAR) {
				sweep(i, j, mineBlocks->GetAt(i)->GetAt(j)->count);
			}
		}
	}
}

BOOL GameManager::autoMarkBlock(int x, int y, int count){
	if (mineBlocks->GetAt(x)->GetAt(y)->getStatus() != STATUS_CLEAR) {
		return FALSE;
	}
	BOOL changed = FALSE;
	int defaultCount = 0;
	int mineCountAround = 0;
	BOOL left = (x != 0);
	BOOL right = (x != mineBlocks->GetSize() - 1);
	BOOL top = (y != 0);
	BOOL bottom = (y != mineBlocks->GetAt(x)->GetSize() - 1);
	if (left && !gameover && mineBlocks->GetAt(x - 1)->GetAt(y)->getStatus() == STATUS_DEFAULT){
		defaultCount++;
	}
	if (right && !gameover && mineBlocks->GetAt(x + 1)->GetAt(y)->getStatus() == STATUS_DEFAULT){
		defaultCount++;
	}
	if (top && !gameover && mineBlocks->GetAt(x)->GetAt(y -1)->getStatus() == STATUS_DEFAULT){
		defaultCount++;
	}
	if (bottom && !gameover && mineBlocks->GetAt(x)->GetAt(y + 1)->getStatus() == STATUS_DEFAULT){
		defaultCount++;
	}
	if (left && top && !gameover && mineBlocks->GetAt(x - 1)->GetAt(y - 1)->getStatus() == STATUS_DEFAULT) {
		defaultCount++;
	}
	if (right && top && !gameover && mineBlocks->GetAt(x + 1)->GetAt(y - 1)->getStatus() == STATUS_DEFAULT) {
		defaultCount++;
	}
	if (left && bottom && !gameover && mineBlocks->GetAt(x - 1)->GetAt(y + 1)->getStatus() == STATUS_DEFAULT) {
		defaultCount++;
	}
	if (right && bottom && !gameover && mineBlocks->GetAt(x + 1)->GetAt(y + 1)->getStatus() == STATUS_DEFAULT) {
		defaultCount++;
	}

	if (left && mineBlocks->GetAt(x - 1)->GetAt(y)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (right && mineBlocks->GetAt(x + 1)->GetAt(y)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (top && mineBlocks->GetAt(x)->GetAt(y -1)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (bottom && mineBlocks->GetAt(x)->GetAt(y + 1)->getStatus() == STATUS_MARK){
		mineCountAround++;
	}
	if (left && top && mineBlocks->GetAt(x - 1)->GetAt(y - 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}
	if (right && top && mineBlocks->GetAt(x + 1)->GetAt(y - 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}
	if (left && bottom && mineBlocks->GetAt(x - 1)->GetAt(y + 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}
	if (right && bottom && mineBlocks->GetAt(x + 1)->GetAt(y + 1)->getStatus() == STATUS_MARK) {
		mineCountAround++;
	}

	if ((defaultCount + mineCountAround) == count && count > 0){
		if (left && !gameover && mineBlocks->GetAt(x - 1)->GetAt(y)->getStatus() == STATUS_DEFAULT){
			mineBlocks->GetAt(x - 1)->GetAt(y)->mark();
			changed = TRUE;
		}
		if (right && !gameover && mineBlocks->GetAt(x + 1)->GetAt(y)->getStatus() == STATUS_DEFAULT){
			mineBlocks->GetAt(x + 1)->GetAt(y)->mark();
			changed = TRUE;
		}
		if (top && !gameover&& mineBlocks->GetAt(x)->GetAt(y -1)->getStatus() == STATUS_DEFAULT){
			mineBlocks->GetAt(x)->GetAt(y -1)->mark();
			changed = TRUE;
		}
		if (bottom && !gameover&& mineBlocks->GetAt(x)->GetAt(y + 1)->getStatus() == STATUS_DEFAULT){
			mineBlocks->GetAt(x)->GetAt(y + 1)->mark();
			changed = TRUE;
		}
		if (left && top && !gameover&& mineBlocks->GetAt(x - 1)->GetAt(y - 1)->getStatus() == STATUS_DEFAULT) {
			mineBlocks->GetAt(x - 1)->GetAt(y - 1)->mark();
			changed = TRUE;
		}
		if (right && top && !gameover&& mineBlocks->GetAt(x + 1)->GetAt(y - 1)->getStatus() == STATUS_DEFAULT) {
			mineBlocks->GetAt(x + 1)->GetAt(y - 1)->mark();
			changed = TRUE;
		}
		if (left && bottom && !gameover&& mineBlocks->GetAt(x - 1)->GetAt(y + 1)->getStatus() == STATUS_DEFAULT) {
			mineBlocks->GetAt(x - 1)->GetAt(y + 1)->mark();
			changed = TRUE;
		}
		if (right && bottom && !gameover&& mineBlocks->GetAt(x + 1)->GetAt(y + 1)->getStatus() == STATUS_DEFAULT) {
			mineBlocks->GetAt(x + 1)->GetAt(y + 1)->mark();
			changed = TRUE;
		}
	}
	return changed;
}


int GameManager::count(int x, int y){
	int count = 0;
	BOOL left = x != 0;
	BOOL right = x != mineBlocks->GetSize() - 1;
	BOOL top = y != 0;
	BOOL bottom = y != mineBlocks->GetAt(x)->GetSize() - 1;
	if (left){
		if(mineBlocks->GetAt(x - 1)->GetAt(y)->isMine()){
			count++;
		}
	}
	if (right){
		if(mineBlocks->GetAt(x + 1)->GetAt(y)->isMine()){
			count++;
		}
	}
	if (top){
		if(mineBlocks->GetAt(x)->GetAt(y -1)->isMine()){
			count++;
		}
	}
	if (bottom){
		if(mineBlocks->GetAt(x)->GetAt(y + 1)->isMine()){
			count++;
		}
	}
	if (left && top) {
		if(mineBlocks->GetAt(x - 1)->GetAt(y - 1)->isMine()){
			count++;
		}
	}
	if (right && top) {
		if(mineBlocks->GetAt(x + 1)->GetAt(y - 1)->isMine()){
			count++;
		}
	}
	if (left && bottom) {
		if(mineBlocks->GetAt(x - 1)->GetAt(y + 1)->isMine()){
			count++;
		}
	}
	if (right && bottom) {
		if(mineBlocks->GetAt(x + 1)->GetAt(y + 1)->isMine()){
			count++;
		}
	}
	return count;
}


int GameManager::getAndPlusOneSecond() {
	return second++;
}
void GameManager::resetSecond() {
	second = 0;
}

void GameManager::mineCountAdd(){
	mineCount++;
	updateDrawCount();
}

void GameManager::mineCountSub(){
	mineCount--;
	updateDrawCount();
	if (mineCount == 0 && isWin()) {
		win();
	}
}

void GameManager::updateDrawCount() {
	callback->updateDrawCount(mineCount);
}

void GameManager::switchCheatMode(){
	cheatMode = !cheatMode;
}

BOOL GameManager::isCheatModeEnabled(){
		return cheatMode;
}

void GameManager::switchAutoMark(){
	autoMark = !autoMark;
}

BOOL GameManager::isAutoMarkEnabled(){
	return autoMark;
}