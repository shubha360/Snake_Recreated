#pragma once

#include <Evolve/ShapeRenderer.h>

#include <random>

#include "Snake.h"

class Fruit {
public:	
	int FRUIT_SIZE = 36;

	Fruit();
	~Fruit();

	bool init(Snake* snake, const int minX, const int maxX, const int minY, const int maxY);

	void update();

	void changeFruitPosition();

	void draw(Evolve::ShapeRenderer& renderer);

private:
	Snake* snake_ = nullptr;

	glm::ivec2 position_ = glm::ivec2(0, 0);
	Evolve::ColorRgba color_ { 0, 0, 100, 255 };

	static std::random_device seed_;
	static std::mt19937 randomEngine_;
	
	std::uniform_int_distribution<int> getRandomX_;
	std::uniform_int_distribution<int> getRandomY_;

	/*std::uniform_int_distribution<int> getRandomRed_;
	std::uniform_int_distribution<int> getRandomYGreen_;*/

	bool isSnakePartInsideFruit(const SnakeBodyPart& part);
};