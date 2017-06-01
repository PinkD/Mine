#pragma once

#include "MyButton.h"

class MyButton;

class GameManager
{
	CArray<CArray<MyButton*>*> *mineBlocks;
public:
	GameManager(CArray<CArray<MyButton*>*> *mineBlocks);
	~GameManager(void);
	void restartGame();
	void gameOver();
	void sweep(int x, int y, int count);
	void sweep(int x, int y);
	int count(int x, int y);
};

