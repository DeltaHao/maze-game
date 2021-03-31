#pragma once

#include"game.h"


class GameFrame {
public:
	Game* game;//游戏
	SDL_Window* pWin;//窗口指针
	SDL_Renderer* pRenderer;//渲染器指针 
	TTF_Font* pDefFont;//游戏资源


	bool init();
	void run();
	void renderFPS(float);
	~GameFrame();
};