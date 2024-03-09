#include "../include/Snake.h"

#include <iostream>

int main(int args, char** argv) {

	Snake snake;
	if (snake.init()) {
		snake.run();
	}
	return 0;
}