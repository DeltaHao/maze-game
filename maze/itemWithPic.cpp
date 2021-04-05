#include"itemWithPic.h"

void StatusBar::render(SDL_Renderer* renderer, TTF_Font* font) {
	SDL_RenderCopy(renderer, texture, NULL, rect);

	//ÏÔÊ¾ÎÄ×Ö
	char text1[256] = { 0 };
	char text2[256] = { 0 };
	sprintf_s(text1, 255, "STEP:%d", steps);
	sprintf_s(text2, 255, "TIME:%d", seconds);
	SDL_Rect dst1 = { rect->x + 25, rect->y + 40, rect->w, 21 };
	SDL_Rect dst2 = { rect->x + 25, rect->y + 90, rect->w, 21 };
	TTF_SizeText(font, text1, &(dst1.w), &(dst1.h));
	TTF_SizeText(font, text2, &(dst2.w), &(dst2.h));
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surf = TTF_RenderText_Blended(font, text1, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture) {
		SDL_RenderCopy(renderer, texture, NULL, &dst1);
		SDL_DestroyTexture(texture);
	}

	surf = TTF_RenderText_Blended(font, text2, color);
	texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture) {
		SDL_RenderCopy(renderer, texture, NULL, &dst2);
		SDL_DestroyTexture(texture);
	}
	SDL_FreeSurface(surf);
}