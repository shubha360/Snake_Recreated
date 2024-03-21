#pragma once

#include <Evolve/ShapeRenderer.h>

#include <random>

#include "Grid.h"

class Fruit {
public:	
	static const int FRUIT_SIZE;

	Fruit();
	~Fruit();

	bool init(Grid* grid);

	void changeFruitPosition();

	void draw(Evolve::ShapeRenderer& renderer);

private:
	Grid* grid_ = nullptr;

	glm::ivec2 positionInGrid_ = glm::ivec2(0, 0);
	Evolve::ColorRgba color_ { 0, 0, 100, 255 };

	static std::random_device seed_;
	static std::mt19937 randomEngine_;
	
	std::uniform_int_distribution<int> getRandomX_;
	std::uniform_int_distribution<int> getRandomY_;

	/*std::uniform_int_distribution<int> getRandomRed_;
	std::uniform_int_distribution<int> getRandomYGreen_;*/
};