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
	int tmp;
	srand(unsigned(time(0)));
	for (int i = 0;i < mineBlocks->GetSize();i++) {//create mine
		tmp = RANDOM(0, (mineBlocks->GetSize() - 1) * (mineBlocks->GetSize() - 1));
		mine[i] = tmp;
		//mine[i] = RANDOM(0, mineBlocks->GetSize() * mineBlocks->GetSize());
	}
	for (int i = 0;i < mineBlocks->GetSize();i++) {//set mine
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			mineBlocks->GetAt(i)->GetAt(j)->performRestartGame(i,j);
			for (int k = 0;k < COUNT_PER_LINE;k++) {
				tmp = POINT_TO_POSITION(i,j);
				//if (mine[i] == POINT_TO_POSITION(i,j)) {
				if (mine[k] == tmp) {
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

