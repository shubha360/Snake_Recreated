#include "../include/MainGame.h"

#include <iostream>

int main(int args, char** argv) {

	MainGame game;
	if (game.init()) {
		game.run();
	}
	return 0;
}