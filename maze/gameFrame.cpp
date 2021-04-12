#include "gameFrame.h"

bool GameFrame::init()
{
	bool ret;
	//初始化SDL
	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret) {
		SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	//初始化TTF
	ret = TTF_Init();
	if (ret) {
		SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	//初始化 Image
	int flags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		SDL_Log("Failed to init required jpg and png and webp support: %s\n", IMG_GetError());
		return NULL;
	}


	//初始化 mixer
	flags = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS;
	initted = Mix_Init(flags);
	if ((initted & flags) != flags) {
		SDL_Log("Failed to init required flag and mod and mp3 and ogg and mid and opus support: %s\n", Mix_GetError());
		return NULL;
	}


	//打开音频
	ret = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
	if (ret != 0) {
		SDL_Log("Mix_OpenAudio failed: %s\n", Mix_GetError());
		return NULL;
	}

	//初始化游戏
	game = new Game();
	ret = game->init();
	if (!ret) {
		SDL_Log("Unable to initialize game\n");
		return false;
	}

	//创建窗口
	char winName[MAX_WIN_NAME] = { 0 };
	GBKToUTF8(game->title.data(), -1, winName, MAX_WIN_NAME);
	pWin = SDL_CreateWindow(winName,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		game->w, game->h,
		(game->isResizable) ? SDL_WINDOW_RESIZABLE : 0);
	if (!pWin) {
		SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}
	//设置图标
	SDL_Surface* iconSurface = IMG_Load("res\\ico\\icon.ico");
	if (iconSurface) {
		SDL_SetWindowIcon(pWin, iconSurface);
		SDL_FreeSurface(iconSurface);
	}

	//创建渲染器
	pRenderer = SDL_CreateRenderer(pWin, -1, SDL_RENDERER_ACCELERATED);
	if (!pRenderer) {
		SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	//创建默认字体
	
	pDefFont = TTF_OpenFont("res\\fonts\\courbd.ttf", 18);
	if (!pDefFont) {
		SDL_Log("Could not open font: %s\n", SDL_GetError());
		return false;
	}
	


	//加载游戏资源
	ret = game->loadResource(pRenderer);
	if (!ret) {
		SDL_Log("Unable to load resource");
		return false;
	}
	return true;
}

void GameFrame::run()
{
	int fps = DEFAULT_FPS;
	if (game->FPS > 0) {
		fps = game->FPS;
	}

	
	SDL_Event evt;

	Uint64 nFrequency, nPrevCounter, nCurrCounter, nElapsedCounter;
	float elapsed = 0.0f, lag = 0.0f, frameMS = 1000.0f / fps;

	nFrequency = SDL_GetPerformanceFrequency();
	nPrevCounter = SDL_GetPerformanceCounter();

	while (true) {
		if (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT)
				break;
			SDL_GetWindowSize(pWin, &game->w, &game->h);
			game->processEvent(&evt);
		}
		else {
			nCurrCounter = SDL_GetPerformanceCounter();
			nElapsedCounter = nCurrCounter - nPrevCounter;
			nPrevCounter = nCurrCounter;
			//前后两帧的耗时(ms)
			elapsed = (nElapsedCounter * 1000.0f) / nFrequency;
			lag += elapsed;			
			//更新游戏
			while (lag >= frameMS) {
				game->update();
				lag -= frameMS;
			}
			//清除背景
			SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
			SDL_RenderClear(pRenderer);
			//渲染图像
			game->render(pWin, pRenderer);
			//显示渲染帧率	 
			renderFPS(elapsed);
			//翻转缓冲
			SDL_RenderPresent(pRenderer);
		}
	}
	
}

void GameFrame::renderFPS(float elapsed)
{
	static float _totalTime = 0;
	static int _fps = 0;
	static int _fpsCount = 0;

	_totalTime += elapsed;

	if (_totalTime >= 1000.0f)
	{
		_totalTime -= 1000.0f;
		_fps = _fpsCount;

		_fpsCount = 1;
	}
	else
	{
		++_fpsCount;
	}


	//计算文字大小
	char fpsText[256] = { 0 };
	sprintf_s(fpsText, 255, "FPS: %d", _fps);
	SDL_Rect dst = { 0, 0, 100, 21 };
	int ret = TTF_SizeText(pDefFont, fpsText, &(dst.w), &(dst.h));
	if (ret) {
		SDL_Log("get text size failed: %s\n", TTF_GetError());
	}


	//渲染文字
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surf = TTF_RenderText_Blended(pDefFont, fpsText, color);
	if (surf) {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surf);
		if (texture) {
			SDL_RenderCopy(pRenderer, texture, NULL, &dst);
			SDL_DestroyTexture(texture);
		}
		SDL_FreeSurface(surf);
	}
	
}

GameFrame::~GameFrame()
{
	if (pRenderer) {
		SDL_DestroyRenderer(pRenderer);
	}
	if (pWin) {
		SDL_DestroyWindow(pWin);
	}
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
