#pragma once
/*
 提供给游戏开发者的接口

 开发者通过实现这个接口来使用游戏框架
*/

#pragma once
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
#include<Windows.h>
#include<iostream>

extern "C" {
#include"SDL.h"
#include"SDL_main.h"
#include"SDL_ttf.h"
#include"SDL_image.h"
#include"SDL_mixer.h"
};

const std::string APP_WIN_NAME = "SGFWin";
const unsigned int MAX_WIN_NAME = 128;
const int DEFAULT_FPS = 60;
const int DEFAULT_WINDOW_WIDTH = 800;
const int DEFAULT_WINDOW_HEIGHT = 600;

class GameInterface {
public:
	std::string  title = APP_WIN_NAME;							//游戏标题
	int w = DEFAULT_WINDOW_WIDTH;
	int h = DEFAULT_WINDOW_HEIGHT;								//窗口大小
	unsigned int FPS = DEFAULT_FPS;								//更新帧率
	bool isResizable = 0;										//窗口大小是否可调节


	virtual ~GameInterface() = default;
	virtual bool init() = 0;									//初始化游戏
	virtual bool loadResource() = 0;							//加载游戏资源
	virtual bool unloadResource() = 0;				//释放游戏资源
	virtual void processEvent(SDL_Event*) = 0;					//处理游戏事件
	virtual void update() = 0;									//更新游戏数据
	virtual void render(SDL_Window*, SDL_Renderer*) = 0;		//渲染游戏画面
};
