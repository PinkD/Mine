#pragma once

#include "resource.h"

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

const int NUM_BITMAP[10] = {IDB_NUM_0, IDB_NUM_1, IDB_NUM_2, IDB_NUM_3, IDB_NUM_4, IDB_NUM_5, IDB_NUM_6, IDB_NUM_7, IDB_NUM_8, IDB_MINE};

static int POINT_TO_POSITION(int x, int y) {
	return y * COUNT_PER_LINE + x;
}

static int RANDOM(int start, int end) {
	return start + (end - start) * rand() / (RAND_MAX + 1);
}

static BOOL POINT_VALID(CPoint *point){
	return point->x > 0 && point->y > 0 && point->x < MINE_WIDTH && point->y < MINE_WIDTH;
}
