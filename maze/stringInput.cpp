#include "stringInput.h"

StringInput::StringInput(int x, int y, int w, int h){
	str = "";
	surf = NULL;
	font = NULL;
	rect = new SDL_Rect;
	show = false;
	*rect = { x, y, w, h };
}

StringInput::~StringInput(){
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	delete rect;
}

std::string StringInput::handleInput(SDL_Event* evt){
	std::string temp = str;
	if (evt->key.keysym.sym >= SDLK_a && evt->key.keysym.sym <= SDLK_z ||
		evt->key.keysym.sym >= SDLK_0 && evt->key.keysym.sym <= SDLK_9 ||
		evt->key.keysym.sym == SDLK_SPACE)
	{
		str += evt->key.keysym.sym;
	}
	else if (evt->key.keysym.sym == SDLK_BACKSPACE) {
		if(str.length() > 0)
			str.erase(str.length() - 1);
	}
	else if (evt->key.keysym.sym == SDLK_RETURN) {
		show = false;
		return str;
	}

	if (str != temp) {
		SDL_FreeSurface(surf);
		SDL_Color color = { 255, 255, 255, 255 };
		surf = TTF_RenderText_Solid(font, str.c_str(), color);
	}
	return "";
}

void StringInput::render(SDL_Renderer* renderer){
	//ºÚ¿ò
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	SDL_RenderFillRect(renderer, rect);
	
	//ÎÄ×Ö
	SDL_Rect textRect = { rect->x, rect->y,	12 * str.size(), rect->h };
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture) {
		SDL_RenderCopy(renderer, texture, NULL, &textRect);
		SDL_DestroyTexture(texture);
	}
}
