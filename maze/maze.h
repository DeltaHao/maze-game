#pragma once
#include"interface.h"
#include"itemWithPic.h"
#include<vector>

# define MAX_MAZE_LEN BACKGROUND_WIDTH / BLOCK_WIDTH
# define MAX_ENTRANCE_NUM 5
# define MAX_EXIT_NUM 5

#define CREATE_STRATEGY_DEFAULT 0
#define CREATE_STRATEGY_DFS 1

class Maze : public ItemWithPic {
public:
	int w;
	int h;
	std::vector<POINT> entrance;
	std::vector<POINT> exit;
	bool matrix[MAX_MAZE_LEN][MAX_MAZE_LEN];

	Maze(int w, int h, ItemWithPic* bg);
	~Maze();
	void clear();
	virtual void render(SDL_Renderer* renderer);
	POINT createGate(int);
	void create(int strategy, int pathNum);
	bool isInArea(int x, int y) {
		return x > 0 && x < w - 1 && y > 0 && y < h;
	}
};