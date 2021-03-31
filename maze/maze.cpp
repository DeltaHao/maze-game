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
	for (auto i = 0; i < MAX_MAZE_LEN; i++)
		for (auto j = 0; j < MAX_MAZE_LEN; j++)
			matrix[i][j] = true;
}

Maze::~Maze(){
	delete rect;
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

