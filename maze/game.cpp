#include "game.h"
#include "sgfHelper.h"

bool Game::init() {
	title = "�Թ�";
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

	//����������
	if(!createBuffer()) return false;
	//����ͼƬ
	SDL_Surface* Surf = IMG_Load("res\\image\\background.png");
	backGround->texture = SDL_CreateTextureFromSurface(bufferRenderer, Surf);

	Surf = IMG_Load("res\\image\\player.png");
	player->texture = SDL_CreateTextureFromSurface(bufferRenderer, Surf);

	SDL_FreeSurface(Surf);


	//����Ĭ������

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
		//��¼���λ��
		mousePos.x = evt->motion.x;
		mousePos.y = evt->motion.y;
		//�����ק���ƻ����ƶ�
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
		//���ƻ����ƶ�
		if (evt->key.keysym.sym == SDLK_UP && view->rect->y > 0) 
			view->moveY(-10);
		else if (evt->key.keysym.sym == SDLK_DOWN && view->rect->y < backGround->rect->h - view->rect->h)
			view->moveY(10);
		else if (evt->key.keysym.sym == SDLK_RIGHT && view->rect->x < backGround->rect->w - view->rect->w)
			view->moveX(10);
		else if (evt->key.keysym.sym == SDLK_LEFT && view->rect->x > 0)
			view->moveX(-10);

		//���������ƶ�
		else if (evt->key.keysym.sym == SDLK_w) player->moveUp();
		else if (evt->key.keysym.sym == SDLK_a) player->moveLeft();
		else if (evt->key.keysym.sym == SDLK_s) player->moveDown();
		else if (evt->key.keysym.sym == SDLK_d) player->moveRight();

	}
	//���ƻ�������
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
	//��ͼƬ���Ƶ��ڴ滺����
	backGround->render(bufferRenderer);
	//SDL_RenderCopy(bufferRenderer, backGround->texture, 0, 0);//����
	player->render(bufferRenderer);//С��

	

	//��������������
	SDL_Texture* bufferTex = SDL_CreateTextureFromSurface(renderer,bufferSurf);
	view->transform(displayArea.w, displayArea.h, backGround->rect->w, backGround->rect->w);

	//������ӳ�䵽����
	SDL_RenderCopy(renderer, bufferTex, view->rect, &displayArea);
	SDL_DestroyTexture(bufferTex);

	//��С��ͼ
	renderMiniMap(renderer);
}



bool Game::createBuffer()
{
	//�����ڴ滺����
	bufferSurf = SDL_CreateRGBSurfaceWithFormat(0, backGround->rect->w, backGround->rect->h,
		32, SDL_PIXELFORMAT_RGBA32);
	if (!bufferSurf) {
		SDL_Log("�����ڴ滺����ʧ��");
		return false;
	}
	//������Ⱦ��
	bufferRenderer = SDL_CreateSoftwareRenderer(bufferSurf);
	if (!bufferRenderer) {
		SDL_Log("�����ڴ滺������Ⱦ��ʧ��");
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
	//С��ͼ
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

#ifdef _DEBUG //��Ⱦ������Ϣ
	//�������ִ�С
	char text[256] = { 0 };
	sprintf_s(text, 255, "(%d,%d,%d,%d)", view->rect->x, view->rect->y, view->rect->w, view->rect->h);
	SDL_Rect dst = { rectBigRec.x, rectBigRec.y, 100, 21 };
	int ret = TTF_SizeText(pDebugFont, text, &(dst.w), &(dst.h));
	if (ret) {
		SDL_Log("get text size failed: %s\n", TTF_GetError());
	}
	//��Ⱦ����
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
