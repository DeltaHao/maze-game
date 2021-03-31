#include"view.h"


View::View(int winW, int winH, int bgW, int bgH)
{
	rect = new SDL_Rect();
	*rect = { (int)((bgW - winW)*0.5f), (int)((bgH - winH) * 0.5f), winW, winH };

	scaleFactor = 1;
	moveFactorX = 0;
	moveFactorY = 0;
}

View::~View() {
	delete rect;
}


void View::moveX(float len)
{
	moveFactorX -= len;
}

void View::moveY(float len)
{
	moveFactorY -= len;
}

void View::zoom(float factor)
{
	scaleFactor -= factor;
}


void View::transform(int winW, int winH, int bgW, int bgH)
{
	*rect = { (int)((bgW - winW) * 0.5f + moveFactorX),
			(int)((bgH - winH) * 0.5f + moveFactorY) ,
			(int)(winW * scaleFactor),(int)(winH * scaleFactor) };
	//±ß½ç¿ØÖÆ
	if (rect->x <= 0) {
		rect->x = 0;
		moveFactorX = (bgW - winW) * -0.5f;
	} 
	if (rect->y <= 0) {
		rect->y = 0;
		moveFactorY = (bgH - winH) * -0.5f;
	} 
	if (rect->x >= bgW - rect->w) {
		rect->x = bgW - rect->w;
		moveFactorX = (bgW - rect->w) - (bgW - winW) * 0.5f;
	} 
	if (rect->y >= bgH - rect->h) {
		rect->y = bgH - rect->h;
		moveFactorY = (bgH - rect->h) - (bgH - winH) * 0.5f;
	} 
	if (rect->w > bgW) {
		rect->x = 0;
		rect->w = bgW; rect->h = bgW;
		moveFactorX = (bgW - winW) * -0.5f;
		//moveFactorY = (bgH - winH) * -0.5f;
		scaleFactor = (float)bgW / winW;
	}
	if (rect->h > bgH) {
		rect->y = 0;
		rect->w = bgH; rect->h = bgH;
		//moveFactorX = (bgW - winW) * -0.5f;
		moveFactorY = (bgH - winH) * -0.5f;
		scaleFactor = (float)bgH / winH;
	}
}

