#pragma once
/*
 �ṩ����Ϸ�����ߵĽӿ�

 ������ͨ��ʵ������ӿ���ʹ����Ϸ���
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
	std::string  title = APP_WIN_NAME;							//��Ϸ����
	int w = DEFAULT_WINDOW_WIDTH;
	int h = DEFAULT_WINDOW_HEIGHT;								//���ڴ�С
	unsigned int FPS = DEFAULT_FPS;								//����֡��
	bool isResizable = 0;										//���ڴ�С�Ƿ�ɵ���


	virtual ~GameInterface() = default;
	virtual bool init() = 0;									//��ʼ����Ϸ
	virtual bool loadResource() = 0;							//������Ϸ��Դ
	virtual bool unloadResource() = 0;				//�ͷ���Ϸ��Դ
	virtual void processEvent(SDL_Event*) = 0;					//������Ϸ�¼�
	virtual void update() = 0;									//������Ϸ����
	virtual void render(SDL_Window*, SDL_Renderer*) = 0;		//��Ⱦ��Ϸ����
};