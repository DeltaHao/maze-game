#pragma once
#include"SDL.h"
class ItemWithPic {
public:
	SDL_Texture* texture;
	SDL_Rect* rect;
	SDL_Rect* rectInTexture;
	ItemWithPic() {
		texture = NULL;
		rect = new SDL_Rect;
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
	void render(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, texture, 0, rect);
	}
};

class Player:public ItemWithPic {
public:
	int x;
	int y; //在矩阵中的位置
	Player(int x, int y):x(x),y(y) {}
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

