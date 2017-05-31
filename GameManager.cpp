#include "StdAfx.h"
#include "GameManager.h"


GameManager::GameManager(CArray<CArray<MyButton*>*> *mineBlocks):mineBlocks(mineBlocks){
	;
}


GameManager::~GameManager(void)
{
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
	srand(unsigned(time(0)));
	for (int i = 0;i < mineBlocks->GetSize();i++) {//create mine
		mine[i] = RANDOM(0, mineBlocks->GetSize() * mineBlocks->GetSize());
	}
	for (int i = 0;i < mineBlocks->GetSize();i++) {//set mine
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			mineBlocks->GetAt(i)->GetAt(j)->performRestartGame(i,j);
			for (int i = 0;i < COUNT_PER_LINE;i++) {
				if (mine[i] = POINT_TO_POSITION(i,j)) {
					mineBlocks->GetAt(i)->GetAt(j)->setMine();
					break;
				}
			}
		}
	}
	for (int i = 0;i < mineBlocks->GetSize();i++) {//set count
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			mineBlocks->GetAt(i)->GetAt(j)->setCount(0);
		}
	}
	delete[] mine;
}


void GameManager::sweep(int x, int y, int count){//recursion able
	BOOL left = x != 1;
	BOOL right = x != mineBlocks->GetSize() - 1;
	BOOL top = y != 1;
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
		if (left && top) {
			mineBlocks->GetAt(x + 1)->GetAt(y + 1)->performSweep();
		}
	}catch (Status status){
		if (status == STATUS_BOOM) {
			gameOver();
		}
	}
}


