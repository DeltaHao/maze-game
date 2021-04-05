#include "game.h"
#include "sgfHelper.h"

bool Game::init() {
	title = "迷宫";
	w = DISPLAY_WIDTH + MENU_WIDTH;
	h = DISPLAY_WIDTH;
	FPS = 30;
	
	mousePos = { -1 };
	PreMousePos = { -1 };

	menuArea = { w - MENU_WIDTH, 0, MENU_WIDTH, h };
	menuBG = new ItemWithPic(menuArea.x, menuArea.y, menuArea.w, menuArea.h);
	statusBar = new StatusBar(menuArea.x, menuArea.y, menuArea.w, menuArea.w);

	displayArea = { 0, 0, w - MENU_WIDTH, h };
	backGround = new BackGround(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	maze = new Maze(40, 40, backGround);
	maze->create(CREATE_STRATEGY_DFS, 4);

	player = new Player(
		maze->rect->x + maze->entrance[maze->entrance.size() - 1].x * BLOCK_WIDTH,
		maze->rect->y + maze->entrance[maze->entrance.size() - 1].y * BLOCK_WIDTH,
		BLOCK_WIDTH, 
		BLOCK_WIDTH
	);
	player->x = maze->entrance[maze->entrance.size() - 1].x;
	player->y = maze->entrance[maze->entrance.size() - 1].y;

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

bool Game::loadResource(SDL_Renderer* winRenderer) {

	//创建缓冲区
	if(!createBuffer()) return false;
	//加载图片
	SDL_Surface* Surf = IMG_Load("res\\image\\background2.png");
	backGround->texture = SDL_CreateTextureFromSurface(bufferRenderer, Surf);
	Surf = IMG_Load("res\\image\\player.png");
	player->texture = SDL_CreateTextureFromSurface(bufferRenderer, Surf);
	Surf = IMG_Load("res\\image\\block.png");
	maze->texture = SDL_CreateTextureFromSurface(bufferRenderer, Surf);

	Surf = IMG_Load("res\\image\\memuBG0.png");
	menuBG->texture = SDL_CreateTextureFromSurface(winRenderer, Surf);
	Surf = IMG_Load("res\\image\\statusBar.png");
	statusBar->texture = SDL_CreateTextureFromSurface(winRenderer, Surf);

	SDL_FreeSurface(Surf);


	//创建字体
	pDebugFont = TTF_OpenFont("res\\fonts\\courbd.ttf", 13);
	pMemuFont = TTF_OpenFont("res\\fonts\\ariblk.ttf", 20);
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
	else if (evt->type == SDL_MOUSEBUTTONDOWN) {
		SDL_Log("%d,%d", mousePos.x, mousePos.y);
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
		else if (evt->key.keysym.sym == SDLK_w) {
			if (player->x < 0 || player->y <= 0 || //防越界
				!maze->matrix[player->x][player->y - 1]) {
				player->moveUp();
				statusBar->steps++;
			}
				
		} 
		else if (evt->key.keysym.sym == SDLK_a) {
			if (player->x <= 0 || player->y < 0 ||
				!maze->matrix[player->x - 1][player->y]) {
				player->moveLeft();
				statusBar->steps++;
			}
				
		} 
		else if (evt->key.keysym.sym == SDLK_s) {
			if (player->x < 0 || player->y < -1 ||
				!maze->matrix[player->x][player->y + 1]) {
				player->moveDown();
				statusBar->steps++;
			}

				
		} 
		else if (evt->key.keysym.sym == SDLK_d) {
			if (player->x < -1 || player->y < 0 ||
				!maze->matrix[player->x + 1][player->y]) {
				player->moveRight();
				statusBar->steps++;
			}

				
		}

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

	else if (evt->type == SDL_DISPLAYEVENT) {
		int a = 0;
	}
}

void Game::update(){
	frames++;
	if (frames % FPS == 0) 
		statusBar->seconds++;
}

void Game::render(SDL_Window*, SDL_Renderer* renderer){
	//把图片绘制到内存缓冲区
	backGround->render(bufferRenderer);//背景
	player->render(bufferRenderer);//小人
	maze->render(bufferRenderer);//迷宫
	

	//创建缓冲区纹理
	SDL_Texture* bufferTex = SDL_CreateTextureFromSurface(renderer,bufferSurf);
	view->transform(displayArea.w, displayArea.h, backGround->rect->w, backGround->rect->h);

	//缓冲区映射到窗口
	displayArea = { 0, 0, w - MENU_WIDTH, h };
	menuArea = { displayArea.w, 0, MENU_WIDTH, h };
	menuBG->rect->x = menuArea.x;
	statusBar->rect->x = menuArea.x;

	SDL_RenderCopy(renderer, bufferTex, view->rect, &displayArea);
	SDL_DestroyTexture(bufferTex);

	
	menuBG->render(renderer);//菜单背景
	statusBar->render(renderer, pMemuFont);
	renderMiniMap(renderer);//画小地图
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
	
	const SDL_FRect rectBigRec = {
		menuArea.x, 
		h - menuArea.w * backGround->rect->h / backGround->rect->w,
		menuArea.w,
		menuArea.w * backGround->rect->h / backGround->rect->w };
	SDL_SetRenderDrawColor(renderer, 156, 133, 46, 255);
	SDL_RenderFillRectF(renderer, &rectBigRec);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRectF(renderer, &rectBigRec);

	float factor = rectBigRec.w / backGround->rect->w;

	if(player){
		const SDL_FRect rectPlayer = {
		rectBigRec.x + (float)player->rect->x * factor,
		rectBigRec.y + (float)player->rect->y * factor,
		BLOCK_WIDTH * factor,
		BLOCK_WIDTH * factor };
		SDL_SetRenderDrawColor(renderer, 254, 183, 98, 255);
		SDL_RenderFillRectF(renderer, &rectPlayer);
	}

	if (maze) {
		SDL_SetRenderDrawColor(renderer, 81, 104, 69, 255);
		for (auto i = 0; i < maze->w; i++) {
			for (auto j = 0; j < maze->h; j++) {
				if (maze->matrix[i][j]) {
					SDL_FRect recBlock = {
						rectBigRec.x + (maze->rect->x + i * BLOCK_WIDTH) * factor,
						rectBigRec.y + (maze->rect->y + j * BLOCK_WIDTH) * factor,
						BLOCK_WIDTH * factor,
						BLOCK_WIDTH * factor
					};
					SDL_RenderFillRectF(renderer, &recBlock);
				}
			}
		}
	}
	const SDL_FRect rectSmallRec = {
		rectBigRec.x + (float)view->rect->x * factor,
		rectBigRec.y + (float)view->rect->y * factor,
		(float)view->rect->w * factor,
		(float)view->rect->h * factor };
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderDrawRectF(renderer, &rectSmallRec);

#ifdef _DEBUG //渲染调试信息
	//计算文字大小
	char text[256] = { 0 };
	sprintf_s(text, 255, "(%d,%d,%d,%d)", view->rect->x, view->rect->y, view->rect->w, view->rect->h);
	SDL_Rect dst = { rectBigRec.x, rectBigRec.y, menuArea.w, 21 };
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
