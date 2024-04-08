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
		Evolve::Position2D rotatePositionInGrid_ {};
		SnakeDirection rotateDirection_ = SnakeDirection::NONE;
	};

	SnakeBodyPart();

	SnakeBodyPart(const SnakePart type, const Evolve::Position2D& positionInGrid, const SnakeDirection direction, 
		const size_t numRows, const size_t numCollumns);

	SnakePart type_ = SnakePart::NONE;
	
	// in Evolve::Position2D, x represents the horizontal position and y represents the vertical position

	Evolve::Position2D previousPositionInGrid_ {};

	Evolve::Position2D currentPositionInGrid_ {};
	Evolve::Position2D currentPositionInWorld_ {};

	Evolve::Position2D nextPositionInGrid_ {};
	Evolve::Position2D nextPositionInWorld_ {};

	Evolve::Position2D nextPositionInGridOffset_ {};

	SnakeDirection direction_ = SnakeDirection::NONE;

	static const int MAX_ROTATIONS = 4;
	Rotation rotations_[MAX_ROTATIONS] {};
	int currentRotationIndex_ = 0, newRotationIndex_ = 0;

	static bool OffsetPosition(Evolve::Position2D& position, size_t numRows, size_t numColumns);
	
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

	// returns true if snake changed position in grid, otherwise false, 
	// don't have to check for anything if the snake didn't change it's position in grid
	// pointHolder gets the point for rotations and loops,
	// pointHolder is -1 on death
	bool move(float deltaTime, int level, int& pointHolder);

	void changeDirection(const SnakeDirection newDirection);

	void draw(Evolve::TextureRenderer& renderer);

	inline SnakeBodyPart getHead() const { return snake_[0]; }
	
	inline std::vector<SnakeBodyPart>& getWholeSnake() { return snake_; }

private:
	const int POINT_ROTAION = 1;
	const int POINT_LOOP = 2;

	Evolve::TextureData bodyTexture_;

	Grid* grid_ = nullptr;
	Food* food_ = nullptr;
	Jackpot* jackpot_ = nullptr;

	std::vector<SnakeBodyPart> snake_;

	const int SNAKE_MAX_CAPACITY = 256;
	int totalParts_ = 0;
	bool maxCapacityReached_ = false;

	bool jackpotConsumed_ = false;

	void createNewPart(const SnakePart type, const Evolve::Position2D& positionInGrid, const SnakeDirection direction);
};