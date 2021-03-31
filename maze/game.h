#include"interface.h"
#include"view.h"
#include"itemWithPic.h"
#include"maze.h"

#define DISPLAY_WIDTH 800
#define MENU_WIDTH 100


class Game: public GameInterface{
public:
	SDL_Surface* bufferSurf;//缓冲区表面
	SDL_Renderer* bufferRenderer;//缓冲区渲染器
	TTF_Font* pDebugFont; //调试字体

	POINT mousePos;//当前鼠标位置
	POINT PreMousePos;//之前鼠标位置
	
	SDL_Rect menuArea;//菜单区域

	SDL_Rect displayArea;//显示区域
	ItemWithPic* backGround;//背景
	Player* player;//小人
	Maze* maze;

	View* view;//视图

	virtual bool init();							//初始化游戏
	virtual bool loadResource();					//加载游戏资源	
	virtual bool unloadResource();					//释放游戏资源
	virtual void processEvent(SDL_Event*);			//处理游戏事件
	virtual void update();							//更新游戏数据
	virtual void render(SDL_Window*, SDL_Renderer*);//渲染游戏画面
	virtual ~Game();


	bool createBuffer();//创建内存缓冲区
	void destroyBuffer();//销毁内存缓冲区
	void renderMiniMap(SDL_Renderer*);//画小地图
};