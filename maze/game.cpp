#include "game.h"
#include "sgfHelper.h"

bool Game::init() {
	title = "迷宫";
	w = 900;
	h = 800;
	FPS = 30;
	
	mousePos = { -1 };
	PreMousePos = { -1 };

	menuArea = { w - 100, 0, 100, h };

	displayArea = { 0, 0, w - 100, h };
	backGround = new ItemWithPic();
	*backGround->rect = { 0, 0, 1200, 1200 };
	player = new Player(0, 0);
	*player->rect = { 0, 0, 20, 20 };

	view = new View(displayArea.w, displayArea.h, backGround->rect->w, backGround->rect->h);
	
	
	return true;
}

Game::~Game() {
	delete view;
	delete player;
	delete backGround;
	unloadResource();
	destroyBuffer();
}

bool Game::loadResource() {

	//创建缓冲区
	if(!createBuffer()) return false;
	//加载图片
	SDL_Surface* Surf = IMG_Load("res\\image\\background.png");
	backGround->texture = SDL_CreateTextureFromSurface(bufferRenderer, Surf);

	Surf = IMG_Load("res\\image\\player.png");
	player->texture = SDL_CreateTextureFromSurface(bufferRenderer, Surf);

	SDL_FreeSurface(Surf);


	//创建默认字体

	pDebugFont = TTF_OpenFont("res\\fonts\\courbd.ttf", 10);
	if (!pDebugFont) {
		SDL_Log("Could not open font: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool Game::unloadResource()
{
	TTF_CloseFont(pDebugFont);
	return false;
}


void Game::processEvent(SDL_Event* evt){
	

	if (evt->type == SDL_MOUSEMOTION) {
		//记录鼠标位置
		mousePos.x = evt->motion.x;
		mousePos.y = evt->motion.y;
		//鼠标拖拽控制画面移动
		if (evt->motion.state == SDL_PRESSED) {
			if (PreMousePos.x > 0 && PreMousePos.y > 0) {
				view->moveX(mousePos.x - PreMousePos.x);
				view->moveY(mousePos.y - PreMousePos.y);
			}
			PreMousePos = mousePos;
		}
		else if (evt->motion.state == SDL_RELEASED) 
			PreMousePos = mousePos;
		
	}
	else if (evt->type == SDL_KEYDOWN) {
		//控制画面移动
		if (evt->key.keysym.sym == SDLK_UP && view->rect->y > 0) 
			view->moveY(-10);
		else if (evt->key.keysym.sym == SDLK_DOWN && view->rect->y < backGround->rect->h - view->rect->h)
			view->moveY(10);
		else if (evt->key.keysym.sym == SDLK_RIGHT && view->rect->x < backGround->rect->w - view->rect->w)
			view->moveX(10);
		else if (evt->key.keysym.sym == SDLK_LEFT && view->rect->x > 0)
			view->moveX(-10);

		//控制人物移动
		else if (evt->key.keysym.sym == SDLK_w) player->moveUp();
		else if (evt->key.keysym.sym == SDLK_a) player->moveLeft();
		else if (evt->key.keysym.sym == SDLK_s) player->moveDown();
		else if (evt->key.keysym.sym == SDLK_d) player->moveRight();

	}
	//控制画面缩放
	else if (evt->type == SDL_MOUSEWHEEL) {

		if (view->rect->w > backGround->rect->w)
			return;

		view->zoom(evt->wheel.y * 0.05f);

		if (evt->wheel.y < 0) {
			view->moveX(mousePos.x * 0.05f);
			view->moveY(mousePos.y * 0.05f);
		}
		else {
			view->moveX(-mousePos.x * 0.05f);
			view->moveY(-mousePos.y * 0.05f);
		}
	}
	
}

void Game::update(){

}

void Game::render(SDL_Window*, SDL_Renderer* renderer){
	//把图片绘制到内存缓冲区
	backGround->render(bufferRenderer);
	//SDL_RenderCopy(bufferRenderer, backGround->texture, 0, 0);//背景
	player->render(bufferRenderer);//小人

	

	//创建缓冲区纹理
	SDL_Texture* bufferTex = SDL_CreateTextureFromSurface(renderer,bufferSurf);
	view->transform(displayArea.w, displayArea.h, backGround->rect->w, backGround->rect->w);

	//缓冲区映射到窗口
	SDL_RenderCopy(renderer, bufferTex, view->rect, &displayArea);
	SDL_DestroyTexture(bufferTex);

	//画小地图
	renderMiniMap(renderer);
}



bool Game::createBuffer()
{
	//创建内存缓冲区
	bufferSurf = SDL_CreateRGBSurfaceWithFormat(0, backGround->rect->w, backGround->rect->h,
		32, SDL_PIXELFORMAT_RGBA32);
	if (!bufferSurf) {
		SDL_Log("创建内存缓冲区失败");
		return false;
	}
	//创建渲染器
	bufferRenderer = SDL_CreateSoftwareRenderer(bufferSurf);
	if (!bufferRenderer) {
		SDL_Log("创建内存缓冲区渲染器失败");
		return false;
	}
	return true;
}


void Game::destroyBuffer()
{
	if (bufferRenderer) SDL_DestroyRenderer(bufferRenderer);
	if (bufferSurf) SDL_FreeSurface(bufferSurf);
}

void Game::renderMiniMap(SDL_Renderer* renderer)
{
	//小地图
	SDL_SetRenderDrawColor(renderer, 80, 137, 56, 100);
	const SDL_FRect rectBigRec = { menuArea.x, 0, 100, 100 };
	SDL_RenderFillRectF(renderer, &rectBigRec);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	const SDL_FRect rectSmallRec = {
		rectBigRec.x + (float)view->rect->x * rectBigRec.w / backGround->rect->w,
		rectBigRec.y + (float)view->rect->y * rectBigRec.h / backGround->rect->h,
		(float)view->rect->w / backGround->rect->w * rectBigRec.w,
		(float)view->rect->h / backGround->rect->h * rectBigRec.h };
	SDL_RenderDrawRectF(renderer, &rectSmallRec);
	SDL_SetRenderDrawColor(renderer, 254, 183, 98, 255);
	const SDL_FRect rectPlayer = { 
		rectBigRec.x + (float)player->rect->x * rectBigRec.w / backGround->rect->w,
		rectBigRec.y + (float)player->rect->y * rectBigRec.h / backGround->rect->h, 3, 3 };
	SDL_RenderDrawRectF(renderer, &rectPlayer);

#ifdef _DEBUG //渲染调试信息
	//计算文字大小
	char text[256] = { 0 };
	sprintf_s(text, 255, "(%d,%d,%d,%d)", view->rect->x, view->rect->y, view->rect->w, view->rect->h);
	SDL_Rect dst = { rectBigRec.x, rectBigRec.y, 100, 21 };
	int ret = TTF_SizeText(pDebugFont, text, &(dst.w), &(dst.h));
	if (ret) {
		SDL_Log("get text size failed: %s\n", TTF_GetError());
	}
	//渲染文字
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surf = TTF_RenderText_Blended(pDebugFont, text, color);
	if (surf) {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (texture) {
			SDL_RenderCopy(renderer, texture, NULL, &dst);
			SDL_DestroyTexture(texture);
		}
		SDL_FreeSurface(surf);
	}
#endif // DEBUG
}
