#include "StdAfx.h"
#include "afxdialogex.h"
#include "GameManager.h"


GameManager::GameManager(CArray<CArray<MyButton*>*> *mineBlocks):mineBlocks(mineBlocks){
}


GameManager::~GameManager(void) {
}

BOOL GameManager::isWin(){
	for (int i = 0;i < mineBlocks->GetSize();i++) {
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			Status status = mineBlocks->GetAt(i)->GetAt(j)->getStatus();
			if (status != STATUS_CLEAR && status != STATUS_MARK) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

void GameManager::win(){
	gameOver();
	MessageBox(NULL, _T("You Win"), _T("Win"), NULL);
	//TODO Dialog
}

void GameManager::gameOver(){
	for (int i = 0;i < mineBlocks->GetSize();i++) {
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			mineBlocks->GetAt(i)->GetAt(j)->performGameOver();
		}
	}

}

void GameManager::restartGame(){
	int *mine = new int[mineBlocks->GetSize()];
	int tmp;
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
}

void GameManager::sweep(int x, int y){//recursion able
	try{
		mineBlocks->GetAt(x)->GetAt(y)->performSweep();
		if (isWin()){
			win();
		}
	}catch (Status status){
		if (status == STATUS_BOOM) {
			gameOver();
		}
	}
}

void GameManager::sweep(int x, int y, int count){//recursion able
	BOOL left = x != 0;
	BOOL right = x != mineBlocks->GetSize() - 1;
	BOOL top = y != 0;
	BOOL bottom = y != mineBlocks->GetAt(x)->GetSize() - 1;
	try{
		if (left){
			mineBlocks->GetAt(x - 1)->GetAt(y)->performSweep();
		}
		if (right){
			mineBlocks->GetAt(x + 1)->GetAt(y)->performSweep();
		}
		if (top){
			mineBlocks->GetAt(x)->GetAt(y -1)->performSweep();
		}
		if (bottom){
			mineBlocks->GetAt(x)->GetAt(y + 1)->performSweep();
		}
		if (left && top) {
			mineBlocks->GetAt(x - 1)->GetAt(y - 1)->performSweep();
		}
		if (right && top) {
			mineBlocks->GetAt(x + 1)->GetAt(y - 1)->performSweep();
		}
		if (left && bottom) {
			mineBlocks->GetAt(x - 1)->GetAt(y + 1)->performSweep();
		}
		if (right && bottom) {
			mineBlocks->GetAt(x + 1)->GetAt(y + 1)->performSweep();
		}
	}catch (Status status){
		if (status == STATUS_BOOM) {
			gameOver();
		}
	}
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

