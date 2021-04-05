#pragma once
#include"interface.h"
#include<vector>
#define BLOCK_WIDTH 25
#define BACKGROUND_WIDTH 1500
#define BACKGROUND_HEIGHT 1500
class ItemWithPic {
public:
	SDL_Texture* texture;
	SDL_Rect* rect;
	ItemWithPic() {
		texture = NULL;
		rect = new SDL_Rect;
	}
	ItemWithPic(float x, float y, int w, int h) {
		texture = NULL;
		rect = new SDL_Rect;
		*rect = { (int)x,(int)y ,w, h };
	}
	~ItemWithPic() {
		if (texture) {
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
		delete rect;
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
	//жиди
	virtual void render(SDL_Renderer* renderer, TTF_Font* font);

};

