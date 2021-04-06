#include"interface.h"
#include"view.h"
#include"itemWithPic.h"
#include"maze.h"

#define DISPLAY_WIDTH 800
#define MENU_WIDTH 150


class Game: public GameInterface{
public:
	
	SDL_Surface* bufferSurf;					//缓冲区表面
	SDL_Renderer* bufferRenderer;				//缓冲区渲染器
	TTF_Font* debugFont;
	TTF_Font* infoFont;
	Mix_Music* music;
	Mix_Chunk* levelUp;
	Mix_Chunk* alert;
	int frames;									//经过的帧数
	bool isClearance = false;					//是否通关
	char info[32];

	POINT mousePos;								//当前鼠标位置
	POINT PreMousePos;							//之前鼠标位置
	
	SDL_Rect menuArea;							//菜单区域
	ItemWithPic* menuBG;						//菜单背景
	StatusBar* statusBar;						//菜单状态栏
	std::vector<ItemWithPic*> buttons;						//“下一关”按钮

	SDL_Rect displayArea;						//显示区域
	ItemWithPic* backGround;					//背景
	Player* player;								//小人
	Maze* maze;									//迷宫
	View* view;									//视图

	virtual bool init();							//初始化游戏
	virtual bool loadResource(SDL_Renderer* );		//加载游戏资源	
	virtual bool unloadResource();					//释放游戏资源
	virtual void processEvent(SDL_Event*);			//处理游戏事件
	virtual void update();							//更新游戏数据
	virtual void render(SDL_Window*, SDL_Renderer*);//渲染游戏画面
	virtual ~Game();

	bool createBuffer();//创建内存缓冲区
	void destroyBuffer();//销毁内存缓冲区
	void renderMiniMap(SDL_Renderer*);//画小地图
	bool refreshLevel();//刷新关卡
	void showInformation(char* text, SDL_Renderer* renderer);
	void levelUpCheck();
};