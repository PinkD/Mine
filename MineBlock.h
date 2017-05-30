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
	virtual Status sweep(int count)=0;
	virtual void mark()=0;
	virtual void reset()=0;
	virtual void detect()=0;
	virtual BOOL isMine()=0;
	virtual void setMine()=0;
	virtual void gameOver()=0;
protected:
	virtual void switchToMark()=0;
	virtual void switchToClear(int count)=0;
	virtual void switchToDefault()=0;
	virtual void switchToBoom()=0;
	virtual void switchToDetect()=0;

	GameManager *manager;
	Status status;
	BOOL mine;
	BOOL gameover;
};

