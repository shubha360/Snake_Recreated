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

class SnakeBodyPart {
public:
	struct Rotation {
		glm::ivec2 rotatePosition_ = glm::ivec2(0);
		SnakeDirection rotateDirection_ = SnakeDirection::NONE;
	};

	SnakeBodyPart(const SnakePart type, const glm::ivec2& position, const SnakeDirection direction);

	SnakePart type_;
	glm::ivec2 position_;
	SnakeDirection direction_;

	std::vector<Rotation> rotations_;
	int currentRotation_ = 0, newRotation_ = 0;

	bool verticalLoopFlag_ = true, horizontalLoopFlag_ = true;
};

class Snake {
public:
	static const int BODY_SIZE;
	static const int SPEED;

	Snake();
	~Snake();

	bool init(const int windowWidth, const int windowHeight);

	void move();	

	void changeDirection(const SnakeDirection newDirection);

	void addNewPart();

	void printSnake(Evolve::ShapeRenderer& renderer);

	inline SnakeBodyPart getSnakeHead() const { return snake_[0]; }
	
	inline std::vector<SnakeBodyPart>& getWholeSnake() { return snake_; }

private:
	
	int windowWidth_ = 0, windowHeight_ = 0;
	std::vector<SnakeBodyPart> snake_;
};