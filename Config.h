#pragma once

#define MINE_WIDTH 25
#define TOOLBAR_HEIGHT MINE_WIDTH * 3
#define COUNT_PER_LINE 9
#define LEVEL_1 1
#define LEVEL_2 2
#define LEVEL_3 3
typedef enum {
	STATUS_DEFAULT,
	STATUS_MARK,
	STATUS_CLEAR,
	STATUS_BOOM
}Status;

static int POINT_TO_POSITION(int x, int y){
	return ((y - TOOLBAR_HEIGHT) / MINE_WIDTH) * COUNT_PER_LINE + (x / MINE_WIDTH);
}