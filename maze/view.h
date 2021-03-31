#pragma once
#include"SDL.h"
class View {
private:
	float scaleFactor ;
	float moveFactorX ;
	float moveFactorY ;//视图变换因子
public:
	SDL_Rect* rect ; //视图坐标
	View(int, int, int, int);
	~View();
	void moveX(float);
	void moveY(float);
	void zoom(float);
	void transform(int, int, int, int);
};