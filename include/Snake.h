#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Evolve/ShapeRenderer.h>

#include "Grid.h"
#include "Fruit.h"

enum class SnakeDirection {
	UP, RIGHT, DOWN, LEFT, NONE
};

enum class SnakePart {
	HEAD, BODY, TAIL, NONE
};

class SnakeBodyPart {
public:
	struct Rotation {
		glm::ivec2 rotatePositionInGrid_ = glm::ivec2(0);
		SnakeDirection rotateDirection_ = SnakeDirection::NONE;
	};

	SnakeBodyPart(const SnakePart type, const glm::ivec2& positionInGrid, const SnakeDirection direction);

	SnakePart type_;
	
	glm::ivec2 previousPositionInGrid_ {};

	glm::ivec2 currentPositionInGrid_;
	glm::ivec2 currentPositionInWorld_;

	glm::ivec2 nextPositionInGrid_;
	glm::ivec2 nextPositionInWorld_;

	SnakeDirection direction_;

	std::vector<Rotation> rotations_;
	int currentRotationIndex_ = 0, newRotationIndex_ = 0;

	//bool verticalLoopFlag_ = true, horizontalLoopFlag_ = true;
};

class Snake {
public:
	static const int BODY_SIZE;
	static const float SPEED;

	Snake();
	~Snake();

	bool init(Grid* grid);

	bool move(float deltaTime);	

	void changeDirection(const SnakeDirection newDirection);

	void addNewPart();

	void draw(Evolve::ShapeRenderer& renderer);

	inline SnakeBodyPart getSnakeHead() const { return snake_[0]; }
	
	inline std::vector<SnakeBodyPart>& getWholeSnake() { return snake_; }

private:
	
	Grid* grid_ = nullptr;
	Fruit fruit_;

	std::vector<SnakeBodyPart> snake_;
	
	size_t currentSnakeMaxSize_ = 128;
	size_t currentPartIndex_ = 0;

	void createNewPart(const SnakePart type, const glm::ivec2& positionInGrid, const SnakeDirection direction);
};