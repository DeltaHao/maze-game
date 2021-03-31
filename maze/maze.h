#pragma once
#include"interface.h"
#include"itemWithPic.h"

# define MAX_MAZE_LEN 120
# define MAX_ENTRANCE_NUM 5
# define MAX_EXIT_NUM 5
class Maze : public ItemWithPic {
public:
	int w;
	int h;
	POINT entrance[MAX_ENTRANCE_NUM];
	POINT exit[MAX_ENTRANCE_NUM];
	bool matrix[MAX_MAZE_LEN][MAX_MAZE_LEN];
	
	Maze(int w, int h, ItemWithPic* bg);

	~Maze();
	void create();
	void draw(SDL_Renderer*);
	void clear();
	virtual void render(SDL_Renderer* renderer);
};