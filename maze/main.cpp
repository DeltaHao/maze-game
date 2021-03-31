#include <Windows.h>
#include "gameFrame.h"


int main(int argc, char* argv[])
{
	GameFrame* gameFrame = new GameFrame();
	if (gameFrame->init()) {
		gameFrame->run();
	}	
	delete gameFrame;
	return 0;
}