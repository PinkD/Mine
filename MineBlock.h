#pragma once

#include "Config.h"
#include "GameManager.h"

class GameManager;

class MineBlock
{
public:
	MineBlock(){};
	virtual ~MineBlock(){};
	virtual void init(GameManager *manager)=0;
	virtual Status getStatus()=0;
	virtual void performSweep()=0;
	virtual void mark()=0;
	virtual void reset()=0;
	virtual void detect()=0;
	virtual BOOL isMine()=0;
	virtual void setMine()=0;
	virtual void setCount(int count)=0;
	virtual void performGameOver()=0;
	virtual void performRestartGame(int x, int y)=0;
	int x;
	int y;
	int count;

protected:
	virtual void switchToMark()=0;
	virtual void switchToClear()=0;
	virtual void switchToDefault()=0;
	virtual void switchToBoom()=0;

	GameManager *manager;
	Status status;
	BOOL mine;
	BOOL gameover;
	BOOL redraw;
};

