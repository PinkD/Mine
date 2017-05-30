#include "StdAfx.h"
#include "GameManager.h"


GameManager::GameManager(CArray<CArray<MineBlock*>*> *mineBlocks):mineBlocks(mineBlocks){

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
	//TODO reset button
}

void GameManager::restartGame(){
	for (int i = 0;i < mineBlocks->GetSize();i++) {
		for (int j = 0; j < mineBlocks->GetAt(i)->GetSize();j++) {
			mineBlocks->GetAt(i)->GetAt(j)->performRestartGame();
		}
	}
}

void GameManager::sweep(int x, int y, int count){


}