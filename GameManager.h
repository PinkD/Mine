#pragma once

#include "MyButton.h"

class MyButton;
class CChildView;

class GameManager
{
	CArray<CArray<MyButton*>*> *mineBlocks;
public:
	GameManager(CArray<CArray<MyButton*>*> *mineBlocks, UpdateDrawCountCallBack *callback);
	~GameManager(void);
	void restartGame();
	BOOL isGameOver();
	int getAndPlusOneSecond();
	void mineCountAdd();
	void mineCountSub();
	void resetSecond();
	void switchCheatMode();
	BOOL isCheatModeEnabled();
	void switchAutoMark();
	BOOL isAutoMarkEnabled();
	void sweep(int x, int y, int count);
	void sweep(int x, int y);
	int count(int x, int y);
private:
	BOOL isWin();
	void win();
	void lose();
	void gameOver();
	void performAutoMark();
	BOOL autoMarkBlock(int x, int y, int count);
	void mapSweep();
	void updateDrawCount();
	UpdateDrawCountCallBack *callback;
	BOOL gameover;
	BOOL autoMark;
	BOOL cheatMode;
	int mineCount;
	int second;
};

