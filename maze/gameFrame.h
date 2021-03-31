#pragma once

#include"game.h"


class GameFrame {
public:
	Game* game;//��Ϸ
	SDL_Window* pWin;//����ָ��
	SDL_Renderer* pRenderer;//��Ⱦ��ָ�� 
	TTF_Font* pDefFont;//��Ϸ��Դ


	bool init();
	void run();
	void renderFPS(float);
	~GameFrame();
};