#pragma once

#include <vector>
#include <string>

#include <Evolve/ShapeRenderer.h>

class Snake;

class Grid {
public:
	friend class Snake;

	const int CELL_SIZE = 32;

	const char SNAKE_SIGN = 'S';
	const char FRUIT_SIGN = 'F';
	const char JACKPOT_SIGN = 'J';

	Grid();
	~Grid();

	bool init(const int windowWidth, const int windowHeight);

	void printGrid(Evolve::ShapeRenderer& shapeRenderer);

private:
	std::vector<std::string> grid_;
};

