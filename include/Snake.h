#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Evolve/ShapeRenderer.h>

#include "Grid.h"

enum class SnakeDirection {
	UP, RIGHT, DOWN, LEFT, NONE
};

enum class SnakePart {
	HEAD, BODY, TAIL
};

class Snake {
public:
	const int BODY_SIZE = 32;
	const int SPEED = 5;

	Snake();
	~Snake();

	bool init(Grid* grid);

	void move();	

	void changeDirection(const SnakeDirection newDirection);

private:
	class SnakeBodyPart {
	public:
		SnakeBodyPart(const SnakePart type, const glm::ivec2& position, const SnakeDirection direction);

		SnakePart type_;
		glm::ivec2 position_;
		SnakeDirection direction_;
	};
	
	Grid* grid_ = nullptr;
	std::vector<SnakeBodyPart> snake_;

	void movePart(SnakeBodyPart& part, const glm::ivec2& newPosition);

	void createNewSnakePart(const SnakePart type, const glm::ivec2& position, const SnakeDirection direction);
};