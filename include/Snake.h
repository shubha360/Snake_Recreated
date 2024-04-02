#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Evolve/ShapeRenderer.h>

#include "Grid.h"
#include "Food.h"
#include "Jackpot.h"

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

	SnakeBodyPart(const SnakePart type, const glm::ivec2& positionInGrid, const SnakeDirection direction, 
		const size_t numRows, const size_t numCollumns);

	SnakePart type_;
	
	// in glm::ivec2, x represents the horizontal position and y represents the vertical position

	glm::ivec2 previousPositionInGrid_ {};

	glm::ivec2 currentPositionInGrid_;
	glm::ivec2 currentPositionInWorld_;

	glm::ivec2 nextPositionInGrid_;
	glm::ivec2 nextPositionInWorld_;

	glm::ivec2 nextPositionInGridOffset_;

	SnakeDirection direction_;

	std::vector<Rotation> rotations_;
	int currentRotationIndex_ = 0, newRotationIndex_ = 0;

	static bool OffsetPosition(glm::ivec2& position, size_t numRows, size_t numColumns);
	
	static void SetNextPosition(SnakeBodyPart& part, size_t numRows, size_t numColumns);
};

class Snake {
public:
	static const int BODY_SIZE;
	static const float SPEED;

	Snake();
	~Snake();

	bool init(Grid* grid, Food* food, Jackpot* jackpot);

	void restart();

	// returns the point for rotations and loops,
	// returns -1 on death
	int move(float deltaTime, int level);

	void changeDirection(const SnakeDirection newDirection);

	void draw(Evolve::ShapeRenderer& renderer);

	inline SnakeBodyPart getHead() const { return snake_[0]; }
	
	inline std::vector<SnakeBodyPart>& getWholeSnake() { return snake_; }

private:
	const int POINT_ROTAION = 1;
	const int POINT_LOOP = 2;

	Grid* grid_ = nullptr;
	Food* food_ = nullptr;
	Jackpot* jackpot_ = nullptr;

	std::vector<SnakeBodyPart> snake_;
	
	const size_t startingSnakeReserveSize_ = 128;
	size_t currentSnakeMaxSize_ = startingSnakeReserveSize_;
	size_t currentPartIndex_ = 0;

	bool jackpotConsumed_ = false;

	void createNewPart(const SnakePart type, const glm::ivec2& positionInGrid, const SnakeDirection direction);
};