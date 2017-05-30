#pragma once

#include "MineBlock.h"

class MineBlock;

class GameManager
{
	CArray<CArray<MineBlock*>*> *mineBlocks;
public:
	GameManager(CArray<CArray<MineBlock*>*> *mineBlocks);
	~GameManager(void);
	void restartGame();
	void gameOver();
	void sweep(int x, int y, int count);
};

