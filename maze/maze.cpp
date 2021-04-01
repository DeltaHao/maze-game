#include"maze.h"

Maze::Maze(int w, int h, ItemWithPic* bg):ItemWithPic(
	(bg->rect->w - w * BLOCK_WIDTH) * 0.5f,
	(bg->rect->h - h * BLOCK_WIDTH) * 0.5f, 
	w * BLOCK_WIDTH, w * BLOCK_WIDTH)
{
	this->w = w;
	this->h = h;
	for (auto i : entrance) i = { 0 };
	for (auto i : exit) i = { 0 };
	clear();
}

Maze::~Maze(){
	delete rect;
}

void Maze::clear(){
	for (auto i = 0; i < MAX_MAZE_LEN; i++)
		for (auto j = 0; j < MAX_MAZE_LEN; j++)
			matrix[i][j] = false;
}

void Maze::render(SDL_Renderer* renderer){
	for (auto i = 0; i < w; i++) {
		for (auto j = 0; j < h; j++) {
			if (matrix[i][j]) {
				SDL_Rect recBlock = {
					rect->x + i * BLOCK_WIDTH,
					rect->y + j * BLOCK_WIDTH,
					BLOCK_WIDTH,
					BLOCK_WIDTH
				};
				SDL_RenderCopy(renderer, texture, 0, &recBlock);
			}
		}
	}	
}

void Maze::create(int strategy)
{
	if (w <= 0 || h <= 0) return;
	switch (strategy)
	{
	case CREATE_STRATEGY_DEFAULT://乱画的
		POINT etce = { 0 };
		entrance.push_back(etce);
		for (auto i = 0; i < w; i++) {
			matrix[i][0] = true;
			matrix[i][h - 1] = true;
		}
		for (auto j = 0; j < h; j++) {
			matrix[0][j] = true;
			matrix[w - 1][j] = true;
		}
		for (int i = 0; i < 60; i++) {
			matrix[rand() % (w - 1)][rand() % (h - 1)] = true;
		}
		matrix[0][0] = false;
		matrix[0][1] = false;
		break;

	case CREATE_STRATEGY_DFS:

		break;

	//TODO:其他生成算法
	default:
		break;
	}
	
	
}
