#include"interface.h"
#include"view.h"
#include"itemWithPic.h"
#include"maze.h"

#define DISPLAY_WIDTH 800
#define MENU_WIDTH 100


class Game: public GameInterface{
public:
	SDL_Surface* bufferSurf;//����������
	SDL_Renderer* bufferRenderer;//��������Ⱦ��
	TTF_Font* pDebugFont; //��������

	POINT mousePos;//��ǰ���λ��
	POINT PreMousePos;//֮ǰ���λ��
	
	SDL_Rect menuArea;//�˵�����

	SDL_Rect displayArea;//��ʾ����
	ItemWithPic* backGround;//����
	Player* player;//С��
	Maze* maze;

	View* view;//��ͼ

	virtual bool init();							//��ʼ����Ϸ
	virtual bool loadResource();					//������Ϸ��Դ	
	virtual bool unloadResource();					//�ͷ���Ϸ��Դ
	virtual void processEvent(SDL_Event*);			//������Ϸ�¼�
	virtual void update();							//������Ϸ����
	virtual void render(SDL_Window*, SDL_Renderer*);//��Ⱦ��Ϸ����
	virtual ~Game();


	bool createBuffer();//�����ڴ滺����
	void destroyBuffer();//�����ڴ滺����
	void renderMiniMap(SDL_Renderer*);//��С��ͼ
};