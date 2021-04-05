#pragma once
#include"interface.h"
#include<vector>
#define BLOCK_WIDTH 25
#define BACKGROUND_WIDTH 1500
#define BACKGROUND_HEIGHT 1500
class ItemWithPic {
public:
	std::vector<SDL_Texture*> textures;
	SDL_Rect* rect;
	int index;
	ItemWithPic() {
		index = 0;
		rect = new SDL_Rect;
	}
	ItemWithPic(float x, float y, int w, int h) {
		index = 0;
		rect = new SDL_Rect;
		*rect = { (int)x,(int)y ,w, h };
	}
	~ItemWithPic() {
		for (auto i : textures) {
			SDL_DestroyTexture(i);
		}
		delete rect;
	}
	virtual void render(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, textures[index], NULL, rect);
	}
};


class BackGround: public ItemWithPic {
public:
	BackGround(float x, float y, int w, int h):ItemWithPic(x, y, w, h) {};
	virtual void render(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, textures[index], rect, rect);
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

