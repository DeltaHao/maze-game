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
	Maze():ItemWithPic() {
		w = 0;
		h = 0;
		clear();
	}
	void clear();
	virtual void render(SDL_Renderer* renderer);
	POINT createGate(int);
	bool create(int w, int h, int strategy, int pathNum, ItemWithPic* bg);
	bool isInArea(int x, int y) {
		return x > 0 && x < w - 1 && y > 0 && y < h;
	}
};

class Player :public ItemWithPic {
public:
	int x;
	int y; //在矩阵中的位置

	Player() : ItemWithPic() {
		x = 0;
		y = 0;
	}
	void adjustPos(Maze* maze) {
		*rect = {
			maze->rect->x + maze->entrance[maze->entrance.size() - 1].x * BLOCK_WIDTH,
			maze->rect->y + maze->entrance[maze->entrance.size() - 1].y * BLOCK_WIDTH,
			BLOCK_WIDTH,
			BLOCK_WIDTH
		};
		x = maze->entrance[maze->entrance.size() - 1].x;
		y = maze->entrance[maze->entrance.size() - 1].y;
	}
	void moveLeft() {
		x -= 1;
		rect->x -= rect->w;
	}
	void moveRight() {
		x += 1;
		rect->x += rect->w;
	}
	void moveUp() {
		y -= 1;
		rect->y -= rect->h;
	}
	void moveDown() {
		y += 1;
		rect->y += rect->h;
	}
};

