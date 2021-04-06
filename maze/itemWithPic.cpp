#include"itemWithPic.h"

void StatusBar::render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, textures[index], NULL, rect);

	//ÏÔÊ¾ÎÄ×Ö
	
	char text1[32] = { 0 };
	char text2[32] = { 0 };
	char text3[32] = { 0 };
	sprintf_s(text1, 32, "STEP:%d", steps);
	sprintf_s(text2, 32, "TIME:%d", seconds);
	sprintf_s(text3, 32, "LEVEL:%d", level+1);
	SDL_Rect dst1 = { rect->x + 25, rect->y + 70, rect->w, 21 };
	SDL_Rect dst2 = { rect->x + 25, rect->y + 110, rect->w, 21 };
	SDL_Rect dst3 = { rect->x + 25, rect->y + 150, rect->w, 21 };
	TTF_SizeText(font, text1, &(dst1.w), &(dst1.h));
	TTF_SizeText(font, text2, &(dst2.w), &(dst2.h));
	TTF_SizeText(font, text2, &(dst3.w), &(dst3.h));
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

	surf = TTF_RenderText_Blended(font, text3, color);
	texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture) {
		SDL_RenderCopy(renderer, texture, NULL, &dst3);
		SDL_DestroyTexture(texture);
	}
	SDL_FreeSurface(surf);
}