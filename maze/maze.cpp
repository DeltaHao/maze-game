#include"maze.h"
#include<stack>


void Maze::clear(){
	for (auto i = 0; i < w; i++)
		for (auto j = 0; j < h; j++)
			matrix[i][j] = false;
	entrance.clear();
	exit.clear();
}

void Maze::render(SDL_Renderer* renderer){
	for (auto i = 0; i < w; i++) {
		for (auto j = 0; j < h; j++) {
			if (matrix[i][j]) {
				SDL_Rect recBlock = {
					rect->x + i * BLOCK_WIDTH,
					rect->y + j * BLOCK_WIDTH,
					BLOCK_WIDTH,
					BLOCK_WIDTH
				};
				SDL_RenderCopy(renderer, textures[index], 0, &recBlock);
			}
		}
	}	
}

POINT Maze::createGate(int count){
	//随机生成入出口

	POINT ret = { 0 };//DFS所需的起点
	for (int i = 0; i < count; i++) {
		POINT etce, ext;
		int tmp = randEx(0, 4);
		switch (tmp)
		{
		case 0:
			etce = { rand() % (w / 2) * 2 + 1, 0 };
			ext = { rand() % (w / 2) * 2 + 1, h - 1 };
			ret = { etce.x, 1 };
			break;
		case 1:
			etce = { rand() % (w / 2) * 2 + 1, h - 1 };
			ext = { rand() % (w / 2) * 2 + 1, 0 };
			ret = { etce.x, h - 2 };
			break;
		case 2:
			etce = { 0, rand() % (h / 2) * 2 + 1 };
			ext = { w - 1, rand() % (h / 2) * 2 + 1 };
			ret = { 1, etce.y };
			break;
		case 3:
			etce = { w - 1, rand() % (h / 2) * 2 + 1 };
			ext = { 0, rand() % (h / 2) * 2 + 1 };
			ret = { w - 2, etce.y };
			break;
		}
		entrance.push_back(etce);
		exit.push_back(ext);
		matrix[etce.x][etce.y]  = false;
		matrix[ext.x][ext.y] = false;
	}
	return ret;
}

bool Maze::create(int W, int H, int strategy, int pathNum, ItemWithPic* bg)
{
	
	if (W <= 0 || H <= 0 || W >= MAX_MAZE_LEN || H >= MAX_MAZE_LEN) return false;
	clear();
	w = W;
	h = H;
	*rect = {
		(int)((bg->rect->w - w * BLOCK_WIDTH) * 0.5f),
		(int)((bg->rect->h - h * BLOCK_WIDTH) * 0.5f),
		w * BLOCK_WIDTH, 
		w * BLOCK_WIDTH
	};
	
	if (strategy == CREATE_STRATEGY_DEFAULT) {//乱画的
		for (auto i = 0; i < w; i++) {
			matrix[i][0] = true;
			matrix[i][h - 1] = true;
		}
		for (auto j = 0; j < h; j++) {
			matrix[0][j] = true;
			matrix[w - 1][j] = true;
		}
		for (int i = 0; i < 60; i++) {
			matrix[rand() % (w - 1)][rand() % (h - 1)] = true;
		}
		createGate(1);
	}
	else if (strategy == CREATE_STRATEGY_DFS) {//深度优先生成法
		//将宽高变为奇数
		if (!(w % 2)) w++;
		if (!(h % 2)) h++;
		
		//生成初始图案，即除了四壁之外，每隔一格有一个空地；
		for (auto i = 0; i < w; i++) {
			for (auto j = 0; j < h; j++) {
					matrix[i][j] = true;
			}
		}
		for (auto i = 1; i < w-1 ; i++) {
			for (auto j = 1; j < h-1; j++) {
				if(i%2 && j%2)
					matrix[i][j] = false;
			}
		}
		POINT startPoint = createGate(1);//生成出入口
		//DFS
		bool visited[MAX_MAZE_LEN][MAX_MAZE_LEN];
		for (auto i = 0; i < w; i++) 
			for (auto j = 0; j < h; j++) 
				visited[i][j] = false;
			
		std::stack<POINT> stk;
		stk.push(startPoint);//入口入栈
		visited[startPoint.x][startPoint.y] = true;
		//通过遍历目前的空地，然后按照某种方法连通两个空地 即把两者之间的墙变成空地
		while (!stk.empty()) {
			//出栈；
			POINT newPos = { 0 };
			POINT currentPos = stk.top();
			stk.pop();
			//随机选择一个方向进行探索
			for (int i = 0; i < 4; i++) {
				
				int tmp = randEx(0, 4);
				switch (tmp)
				{
				case 0:
					newPos = { currentPos.x + 2, currentPos.y };
					break;
				case 1:
					newPos = { currentPos.x - 2, currentPos.y };
					break;
				case 2:
					newPos = { currentPos.x , currentPos.y + 2 };
					break;
				case 3:
					newPos = { currentPos.x, currentPos.y - 2 };
					break;
				}
				//坐标没有越界且未被访问过
				if (isInArea(newPos.x, newPos.y)) {
					if (!visited[newPos.x][newPos.y]) {
						matrix[(currentPos.x + newPos.x) / 2][(currentPos.y + newPos.y) / 2] = false;
						stk.push(newPos);
						visited[newPos.x][newPos.y] = true;
					}
					else if (rand() < pathNum - 1) {// 此处通过将访问过的节点再次打通来实现为设置迷宫多解
						matrix[(currentPos.x + newPos.x) / 2][(currentPos.y + newPos.y) / 2] = false;
					}
				}
			}		
		}
 	}
	return true;
}
