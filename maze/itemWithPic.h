#pragma once
#include"interface.h"

#define BLOCK_WIDTH 25
#define BACKGROUND_WIDTH 1500
#define BACKGROUND_HEIGHT 1500
class ItemWithPic {
public:
	SDL_Texture* texture;
	SDL_Rect* rect;
	SDL_Rect* rectInTexture;
	ItemWithPic(float x, float y, int w, int h) {
		texture = NULL;
		rect = new SDL_Rect;
		*rect = { (int)x,(int)y ,w, h };
		rectInTexture = new SDL_Rect;
	}
	~ItemWithPic() {
		if (texture) {
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
		delete rect;
		delete rectInTexture;
	}
	virtual void render(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, texture, NULL, rect);
	}
};


class BackGround: public ItemWithPic {
public:
	BackGround(float x, float y, int w, int h):ItemWithPic(x, y, w, h) {};
	virtual void render(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, texture, rect, rect);
	}
};

class StatusBar: public ItemWithPic {
public:
	int steps;
	int seconds;
	StatusBar(float x, float y, int w, int h) :ItemWithPic(x, y, w, h) {
		steps = 0;
		seconds = 0;
	};
	//重载
	virtual void render(SDL_Renderer* renderer, TTF_Font* font);

};

class Player:public ItemWithPic {
public:
	int x;
	int y; //在矩阵中的位置

	Player(float x, float y, int w, int h): ItemWithPic(x, y, w, h){
		this->x = 0;
		this->y = 0;
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

