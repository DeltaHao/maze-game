#pragma once
#include"SDL.h"
class View {
private:
	float scaleFactor ;
	float moveFactorX ;
	float moveFactorY ;//��ͼ�任����
public:
	SDL_Rect* rect ; //��ͼ����
	View(int, int, int, int);
	~View();
	void moveX(float);
	void moveY(float);
	void zoom(float);
	void transform(int, int, int, int);
};