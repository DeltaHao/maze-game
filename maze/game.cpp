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
	statusBar = new StatusBar(menuArea.x, menuArea.y , menuArea.w, menuArea.w * 3/2);
	for (int i = 0; i < 4; i++) {
		ItemWithPic* button = new ItemWithPic(
			menuArea.x,
			statusBar->rect->y + statusBar->rect->h + 40 + i*100,
			menuArea.w,
			60
		);
		buttons.push_back(button);
	}
	

	displayArea = { 0, 0, w - MENU_WIDTH, h };
	stringInput = new StringInput(10, h - 100, displayArea.w - 20, 30);
	backGround = new BackGround(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	maze = new Maze();
	player = new Player();

	view = new View(displayArea.w, displayArea.h, backGround->rect->w, backGround->rect->h);

	refreshLevel();

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
	SDL_Surface* surf;
	SDL_Texture* texture;

	surf = IMG_Load("res\\image\\background0.png");
	texture = SDL_CreateTextureFromSurface(bufferRenderer, surf);
	backGround->textures.push_back(texture);
	surf = IMG_Load("res\\image\\background1.png");
	texture = SDL_CreateTextureFromSurface(bufferRenderer, surf);
	backGround->textures.push_back(texture);
	surf = IMG_Load("res\\image\\background2.png");
	texture = SDL_CreateTextureFromSurface(bufferRenderer, surf);
	backGround->textures.push_back(texture);


	surf = IMG_Load("res\\image\\player.png");
	texture = SDL_CreateTextureFromSurface(bufferRenderer, surf);
	player->textures.push_back(texture);

	surf = IMG_Load("res\\image\\block.png");
	texture = SDL_CreateTextureFromSurface(bufferRenderer, surf);
	maze->textures.push_back(texture);

	surf = IMG_Load("res\\image\\memuBG0.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	menuBG->textures.push_back(texture);

	surf = IMG_Load("res\\image\\statusBar.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	statusBar->textures.push_back(texture);

	surf = IMG_Load("res\\image\\button0.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	buttons[0]->textures.push_back(texture);
	surf = IMG_Load("res\\image\\button0-hovered.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	buttons[0]->textures.push_back(texture);

	surf = IMG_Load("res\\image\\button1.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	buttons[1]->textures.push_back(texture);
	surf = IMG_Load("res\\image\\button1-hovered.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	buttons[1]->textures.push_back(texture);
	surf = IMG_Load("res\\image\\button1-disabled.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	buttons[1]->textures.push_back(texture);
	surf = IMG_Load("res\\image\\button1-disabled-hovered.png");
	texture = SDL_CreateTextureFromSurface(winRenderer, surf);
	buttons[1]->textures.push_back(texture);



	SDL_FreeSurface(surf);
	
	
	
	//字体
	debugFont = TTF_OpenFont("res\\fonts\\courbd.ttf", 13);
	infoFont = TTF_OpenFont("res\\fonts\\SHOWG.TTF", 18);
	statusBar->font = TTF_OpenFont("res\\fonts\\courbd.ttf", 24);
	stringInput->font = TTF_OpenFont("res\\fonts\\courbd.ttf", 25);

	//声音
	music = Mix_LoadMUS("res\\music\\music.mp3");
	Mix_PlayMusic(music, -1);

	alert = Mix_LoadWAV("res\\music\\alert.mp3");
	levelUp = Mix_LoadWAV("res\\music\\levelUp.mp3");
	
	

	return true;
}

bool Game::unloadResource()
{
	Mix_FreeChunk(levelUp);
	Mix_FreeChunk(alert);
	Mix_FreeMusic(music);
	TTF_CloseFont(infoFont);
	TTF_CloseFont(debugFont);
	return true;
}


void Game::processEvent(SDL_Event* evt){
	
	
	if (evt->type == SDL_MOUSEMOTION) {
		//记录鼠标位置
		mousePos.x = evt->motion.x;
		mousePos.y = evt->motion.y;
		//鼠标拖拽控制画面移动
		if (isInArea(mousePos, displayArea)) {
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
		if (isInArea(mousePos, *buttons[0]->rect) && isClearance)
			buttons[0]->index = 1;
		else 
			buttons[0]->index = 0;

		if (isInArea(mousePos, *buttons[1]->rect))
			if (Mix_PausedMusic() == 1) //如果音乐被暂停了
				buttons[1]->index = 3;
			else //如果音乐正在播放
				buttons[1]->index = 1;		
		else
			if (Mix_PausedMusic() == 1) //如果音乐被暂停了
				buttons[1]->index = 2;
			else //如果音乐正在播放
				buttons[1]->index = 0;
	}
	else if (evt->type == SDL_MOUSEBUTTONDOWN) {
		if (isInArea(mousePos, *buttons[0]->rect)) {
			if (isClearance) {
				statusBar->level++;
				if (!refreshLevel()) {
					//TODO:游戏结束;
				}
			}
			else {
				Mix_PlayChannel(-1, alert, 0);
				sprintf_s(info, 32, "GET TO THE EXIT!");
			}
		}
		else if (isInArea(mousePos, *buttons[1]->rect)) {
			//如果音乐被暂停了
			if (Mix_PausedMusic() == 1){
				//恢复播放
				Mix_ResumeMusic();
				buttons[1]->index = 0;
			}
			//如果音乐正在播放
			else{
				//暂停音乐
				Mix_PauseMusic();
				buttons[1]->index = 2;
			}
		}
	}
	else if (evt->type == SDL_KEYDOWN) {
		if (evt->key.keysym.sym == SDLK_BACKQUOTE) {
			stringInput->show = !stringInput->show;
		}
		if (stringInput->show) {
			handleSpell(stringInput->handleInput(evt));
			
			//TODO::其他秘籍
			return;
		}
		//控制人物移动
		if (evt->key.keysym.sym == SDLK_w || evt->key.keysym.sym == SDLK_UP) {
			if (player->x < 0 || player->y <= 0 || //防越界
				!maze->matrix[player->x][player->y - 1]) {
				player->moveUp();
				statusBar->steps++;
				levelUpCheck();
			}	
		} 
		else if (evt->key.keysym.sym == SDLK_a || evt->key.keysym.sym == SDLK_LEFT) {
			if (player->x <= 0 || player->y < 0 ||
				!maze->matrix[player->x - 1][player->y]) {
				player->moveLeft();
				statusBar->steps++;
				levelUpCheck();
			}	
		} 
		else if (evt->key.keysym.sym == SDLK_s || evt->key.keysym.sym == SDLK_DOWN) {
			if (player->x < 0 || player->y < -1 ||
				!maze->matrix[player->x][player->y + 1]) {
				player->moveDown();
				statusBar->steps++;
				levelUpCheck();
			}
				
		} 
		else if (evt->key.keysym.sym == SDLK_d || evt->key.keysym.sym == SDLK_RIGHT) {
			if (player->x < -1 || player->y < 0 ||
				!maze->matrix[player->x + 1][player->y]) {
				player->moveRight();
				statusBar->steps++;
				levelUpCheck();
			}	
		}
	}
	
	else if (evt->type == SDL_MOUSEWHEEL) {
		//控制画面缩放

		if (isInArea(mousePos, displayArea)) {
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
	stringInput->rect->w = displayArea.w - 20;
	menuArea = { displayArea.w, 0, MENU_WIDTH, h };
	menuBG->rect->x = menuArea.x;
	statusBar->rect->x = menuArea.x;
	for(auto i :buttons)
		i->rect->x = menuArea.x;
	

	SDL_RenderCopy(renderer, bufferTex, view->rect, &displayArea);
	SDL_DestroyTexture(bufferTex);

	
	
	menuBG->render(renderer);//菜单背景
	statusBar->render(renderer);
	for (auto i : buttons)
		i->render(renderer);
	renderMiniMap(renderer);//画小地图
	if(stringInput->show)
		stringInput->render(renderer);
	
	showInformation(info, renderer);	
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
		(menuArea.w) * backGround->rect->h / backGround->rect->w 
	};

	if(backGround->index == 0)
		SDL_SetRenderDrawColor(renderer, 140, 95, 62, 255);
	else if (backGround->index == 1)
		SDL_SetRenderDrawColor(renderer, 207, 210, 203, 255);
	else if (backGround->index == 2)
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
	SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
	SDL_RenderDrawRectF(renderer, &rectSmallRec);

#ifdef _DEBUG //渲染调试信息
	//计算文字大小
	
	char text[256] = { 0 };
	sprintf_s(text, 255, "(%d,%d,%d,%d)", view->rect->x, view->rect->y, view->rect->w, view->rect->h);
	SDL_Rect dst = { rectBigRec.x, rectBigRec.y, menuArea.w, 21 };
	int ret = TTF_SizeText(debugFont, text, &(dst.w), &(dst.h));
	if (ret) {
		SDL_Log("get text size failed: %s\n", TTF_GetError());
	}
	//渲染文字
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surf = TTF_RenderText_Blended(debugFont, text, color);
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

bool Game::refreshLevel() {
	statusBar->steps = 0;
	statusBar->seconds = 0;
	frames = 0;
	isClearance = false;
	sprintf_s(info, 32, "");
	int W = 10 + randEx(0, 3) + statusBar->level * 3;
	int H = 10 + randEx(0, W/3) + statusBar->level * 3;
	int C = randEx(0, 5);
	if(!maze->create(W, H, CREATE_STRATEGY_DFS, C, backGround))
		return false;
	player->adjustPos(maze);
	backGround->index = randEx(0, 3);

	return true;
}

void Game::showInformation(char* text, SDL_Renderer* renderer){
	//显示文字
	SDL_Rect dst1 = { menuArea.x, buttons[0]->rect->y + 70, menuArea.w, 21 };
	TTF_SizeText(infoFont, text, &(dst1.w), &(dst1.h));

	SDL_Color color = { 255, 50, 50, 255 };
	SDL_Surface* surf = TTF_RenderText_Blended(infoFont, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture) {
		SDL_RenderCopy(renderer, texture, NULL, &dst1);
		SDL_DestroyTexture(texture);
	}
	SDL_FreeSurface(surf);
}

void Game::handleSpell(std::string spell){
	if (spell == "rebuild") {
		refreshLevel();
	}
}

void Game::levelUpCheck()
{
	for (auto i : maze->exit)
		if (player->x == i.x && player->y == i.y) {
			isClearance = true;
			sprintf_s(info, 32, "      LEVEL UP!!!");
			Mix_PlayChannel(-1, levelUp, 0);
		}
}
