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
			matrix[i][j] = false;
}

Maze::~Maze(){
	delete rect;
}

void Maze::create()
{
	entrance[0].x = 0; entrance[0].y = 0;
	for (auto i = 1; i < w - 1; i++) {
		matrix[i][0] = true;
		matrix[i][h - 1] = true;
	}
	for (auto j = 1; j < h - 1; j++) {
		matrix[0][j] = true;
		matrix[w - 1][j] = true;
	}
	matrix[3][13] = true;
	matrix[16][5] = true;


	matrix[0][0] = false;
	matrix[0][1] = false;
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

