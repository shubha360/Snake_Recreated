#include "../include/Snake.h"

const int Snake::BODY_SIZE = Grid::CELL_SIZE;
const float Snake::SPEED = 5.0f;

Snake::Snake() {}

Snake::~Snake() {
	Evolve::ImageLoader::DeleteTexture(bodyTexture_);
}

bool Snake::init(Grid* grid, Food* food, Jackpot* jackpot) {

	//snake_.reserve(SNAKE_RESERVE_SIZE);

	grid_ = grid;
	food_ = food;
	jackpot_ = jackpot;


	/*createNewPart(SnakePart::HEAD, glm::ivec2(40, 10), SnakeDirection::RIGHT);

	for (int i = 39; i > 0; i--) {
		createNewPart(SnakePart::BODY, glm::ivec2(i, 10), SnakeDirection::RIGHT);
	}

	createNewPart(SnakePart::TAIL, glm::ivec2(0, 10), SnakeDirection::RIGHT);*/

	createNewPart(SnakePart::HEAD, Evolve::Position2D { 12, 10 }, SnakeDirection::RIGHT);
	createNewPart(SnakePart::BODY, Evolve::Position2D { 11, 10 }, SnakeDirection::RIGHT);
	createNewPart(SnakePart::TAIL, Evolve::Position2D { 10, 10 }, SnakeDirection::RIGHT);

	Evolve::ImageLoader::LoadTextureFromImage("resources/images/body.png", bodyTexture_, 4);
	Evolve::ImageLoader::BufferTextureData(bodyTexture_);
	Evolve::ImageLoader::FreeTexture(bodyTexture_);

	return true;
}

void Snake::restart() {

	for (auto& part : snake_) {
		
		grid_->clearCell(part.currentPositionInGrid_.Y, part.currentPositionInGrid_.X);
	}
	
	// the body parts don't move once the snake eats itself
	// that's why no body part covers this cell but it remains occupied in the grid
	grid_->clearCell(snake_[0].previousPositionInGrid_.Y, snake_[0].previousPositionInGrid_.X);

	snake_.clear();
	//snake_.reserve(SNAKE_RESERVE_SIZE);

	createNewPart(SnakePart::HEAD, Evolve::Position2D { 12, 10 }, SnakeDirection::RIGHT);
	createNewPart(SnakePart::BODY, Evolve::Position2D { 11, 10 }, SnakeDirection::RIGHT);
	createNewPart(SnakePart::TAIL, Evolve::Position2D { 10, 10 }, SnakeDirection::RIGHT);
}

// returns true if snake changed position in grid, otherwise false, 
// don't have to check for anything if the snake didn't change it's position in grid
// pointHolder gets the point for rotations and loops,
// pointHolder is -1 on death
bool Snake::move(float deltaTime, int level, int& pointHolder) {

	pointHolder = 0;
	bool changedPosition = false;

	for (int i = 0; i < snake_.size(); i++) {

		auto& current = snake_[i];

		bool positionChangedInGrid = false;

		switch (current.direction_) {

		case SnakeDirection::RIGHT:
			current.currentPositionInWorld_.X += (int)((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.X >= current.nextPositionInWorld_.X) {
				
				positionChangedInGrid = true;
				
				current.currentPositionInWorld_.X = current.nextPositionInWorld_.X;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

					
				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					if (current.type_ == SnakePart::HEAD) {
						pointHolder += POINT_LOOP;
					}

					current.currentPositionInWorld_.X = current.currentPositionInGrid_.X * BODY_SIZE;
				}
				
				current.nextPositionInGrid_.X = current.currentPositionInGrid_.X + 1;
				current.nextPositionInWorld_.X = current.nextPositionInGrid_.X * BODY_SIZE;
			}

			break;

		case SnakeDirection::LEFT:
			current.currentPositionInWorld_.X -= (int) ((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.X <= current.nextPositionInWorld_.X) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.X = current.nextPositionInWorld_.X;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					if (current.type_ == SnakePart::HEAD) {
						pointHolder += POINT_LOOP;
					}

					current.currentPositionInWorld_.X = current.currentPositionInGrid_.X * BODY_SIZE;
				}

				current.nextPositionInGrid_.X = current.currentPositionInGrid_.X - 1;
				current.nextPositionInWorld_.X = current.nextPositionInGrid_.X * BODY_SIZE;
			}
			break;

		case SnakeDirection::UP:
			current.currentPositionInWorld_.Y += (int) ((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.Y >= current.nextPositionInWorld_.Y) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.Y = current.nextPositionInWorld_.Y;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					if (current.type_ == SnakePart::HEAD) {
						pointHolder += POINT_LOOP;
					}

					current.currentPositionInWorld_.Y = current.currentPositionInGrid_.Y * BODY_SIZE;
				}

				current.nextPositionInGrid_.Y = current.currentPositionInGrid_.Y + 1;
				current.nextPositionInWorld_.Y = current.nextPositionInGrid_.Y * BODY_SIZE;
			}
			break;

		case SnakeDirection::DOWN:
			current.currentPositionInWorld_.Y -= (int) ((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.Y <= current.nextPositionInWorld_.Y) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.Y = current.nextPositionInWorld_.Y;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					if (current.type_ == SnakePart::HEAD) {
						pointHolder += POINT_LOOP;
					}

					current.currentPositionInWorld_.Y = current.currentPositionInGrid_.Y * BODY_SIZE;
				}

				current.nextPositionInGrid_.Y = current.currentPositionInGrid_.Y - 1;
				current.nextPositionInWorld_.Y = current.nextPositionInGrid_.Y * BODY_SIZE;
			}
			break;
		}		

		if (positionChangedInGrid) {

			bool newTailAdded = false;
			changedPosition = true;

			current.nextPositionInGridOffset_ = current.nextPositionInGrid_;

			SnakeBodyPart::OffsetPosition(current.nextPositionInGridOffset_,
				grid_->getNumRows(), grid_->getNumColumns());

			if (current.type_ == SnakePart::TAIL) {

				grid_->clearCell(current.previousPositionInGrid_.Y, current.previousPositionInGrid_.X);

				if (snake_.size() - 1 > i) { // have to add new part if true

					auto& newTail = snake_[(size_t) i + 1];

					if (current.previousPositionInGrid_.isEqualTo(newTail.currentPositionInGrid_)) {

						current.type_ = SnakePart::BODY;

						newTail.type_ = SnakePart::TAIL;
						newTail.direction_ = current.direction_;

						SnakeBodyPart::SetNextPosition(newTail, grid_->getNumRows(), grid_->getNumColumns());

						grid_->addSnakeCell(newTail.currentPositionInGrid_.Y, newTail.currentPositionInGrid_.X);
						
						newTailAdded = true;
					}
				}
			}

			// ROTATION
			auto& currentRotation = current.rotations_[current.currentRotationIndex_];

			if (currentRotation.rotateDirection_ != SnakeDirection::NONE &&
				current.currentPositionInGrid_.isEqualTo(currentRotation.rotatePositionInGrid_)) {

				current.direction_ = currentRotation.rotateDirection_;
				currentRotation.rotateDirection_ = SnakeDirection::NONE;

				current.currentRotationIndex_ = (current.currentRotationIndex_ + 1) % SnakeBodyPart::MAX_ROTATIONS;

				SnakeBodyPart::SetNextPosition(current, grid_->getNumRows(), grid_->getNumColumns());

				if (current.type_ == SnakePart::HEAD) {
					pointHolder += POINT_ROTAION;
				}

				if (current.type_ != SnakePart::TAIL) {
					auto& next = snake_[(size_t) i + 1];
						
					next.rotations_[next.newRotationIndex_].rotateDirection_ = current.direction_;
					next.rotations_[next.newRotationIndex_].rotatePositionInGrid_ = current.currentPositionInGrid_;

					next.newRotationIndex_ = (next.newRotationIndex_ + 1) % SnakeBodyPart::MAX_ROTATIONS;
				}
			}


			if (current.type_ == SnakePart::HEAD) {
				
				bool ateSelf = false;

				// ate itself
				if (grid_->isSnakeCell(current.currentPositionInGrid_.Y, current.currentPositionInGrid_.X)) {
					ateSelf = true;
				}
				
				// ate fruit
				else if (grid_->isFruitCell(current.currentPositionInGrid_.Y, current.currentPositionInGrid_.X)) {

					createNewPart(
						SnakePart::NONE,
						Evolve::Position2D { current.currentPositionInGrid_.X, current.currentPositionInGrid_.Y },
						SnakeDirection::NONE
					);

					food_->consumed();
				} 
				
				// ate jackpot
				else if (grid_->isJackpotCell(current.currentPositionInGrid_.Y, current.currentPositionInGrid_.X)) {
					
					createNewPart(
						SnakePart::NONE,
						Evolve::Position2D { current.currentPositionInGrid_.X, current.currentPositionInGrid_.Y },
						SnakeDirection::NONE
					);

					jackpot_->consumed();
					jackpotConsumed_ = true;
				}

				// it is possible that the vector of body parts was reallocated
				/*current = snake_[i];*/

				grid_->addSnakeCell(current.currentPositionInGrid_.Y, current.currentPositionInGrid_.X);

				if (ateSelf) {
					pointHolder = -1;
				}
			}

			// the new tail shouldn't move in this frame
			if (newTailAdded) {
				break;
			}
		}
	}
	return changedPosition;
}

void Snake::changeDirection(const SnakeDirection newDirection) {

	auto& head = snake_[0];

	switch (newDirection) {

	case SnakeDirection::RIGHT:
	case SnakeDirection::LEFT:

		if (head.direction_ == SnakeDirection::UP || head.direction_ == SnakeDirection::DOWN) {

			head.rotations_[head.newRotationIndex_] = SnakeBodyPart::Rotation { head.nextPositionInGrid_, newDirection };

			SnakeBodyPart::OffsetPosition(head.rotations_[head.newRotationIndex_].rotatePositionInGrid_, 
				grid_->getNumRows(), grid_->getNumColumns());

			head.newRotationIndex_ = (head.newRotationIndex_ + 1) % SnakeBodyPart::MAX_ROTATIONS;
		}
		break;	

	case SnakeDirection::UP:
	case SnakeDirection::DOWN:

		if (head.direction_ == SnakeDirection::RIGHT || head.direction_ == SnakeDirection::LEFT) {

			head.rotations_[head.newRotationIndex_] = SnakeBodyPart::Rotation{ head.nextPositionInGrid_, newDirection };

			SnakeBodyPart::OffsetPosition(head.rotations_[head.newRotationIndex_].rotatePositionInGrid_,
				grid_->getNumRows(), grid_->getNumColumns());

			head.newRotationIndex_ = (head.newRotationIndex_ + 1) % SnakeBodyPart::MAX_ROTATIONS;
		}
		break;
	}
}

void Snake::draw(Evolve::TextureRenderer& renderer) {
	
	static Evolve::ColorRgba HEAD_COLOR {  88, 185, 142, 255 };
	static Evolve::ColorRgba BODY_COLOR { 105, 210, 164, 255 };

	static const Evolve::UvDimension uv { 0.0f, 0.0f, 1.0f, 1.0f };

	// jackpot consumption graphics
	static int ALPHA_CHANGE = 10;
	static int jackpotAlphaGoingDown = true;
	static int currentBodyAlpha = BODY_COLOR.Alpha;

	static int opaqueCount = 0;
	static const int MAX_OPAQUES = 3;

	if (jackpotConsumed_) {
		if (jackpotAlphaGoingDown) {
			currentBodyAlpha -= ALPHA_CHANGE;

			if (currentBodyAlpha < 0) {
				currentBodyAlpha = 0;
				jackpotAlphaGoingDown = false;
			}
		}
		else {
			currentBodyAlpha += ALPHA_CHANGE;

			if (currentBodyAlpha > 255) {
				currentBodyAlpha = 255;
				jackpotAlphaGoingDown = true;

				opaqueCount++;

				if (opaqueCount == MAX_OPAQUES) {
					jackpotConsumed_ = false;
					opaqueCount = 0;
				}
			}
		}
	
		BODY_COLOR.Alpha = currentBodyAlpha;
	}	

	for (int i = (int) snake_.size() - 1; i >= 0; i--) {

		auto& current = snake_[i];

		Evolve::RectDimension ghostDims;

		switch (current.direction_) {
		case SnakeDirection::RIGHT:
			
			if (current.nextPositionInGrid_.X == grid_->getNumColumns()) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.X - (int) grid_->getNumColumns() * Grid::CELL_SIZE,
					current.currentPositionInWorld_.Y,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;

		case SnakeDirection::LEFT:

			if (current.nextPositionInGrid_.X == -1) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.X + (int) grid_->getNumColumns() * Grid::CELL_SIZE,
					current.currentPositionInWorld_.Y,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;

		case SnakeDirection::UP:

			if (current.nextPositionInGrid_.Y == grid_->getNumRows()) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.X,
					current.currentPositionInWorld_.Y - (int) grid_->getNumRows() * Grid::CELL_SIZE,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;

		case SnakeDirection::DOWN:

			if (current.nextPositionInGrid_.Y == -1) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.X,
					current.currentPositionInWorld_.Y + (int) grid_->getNumRows() * Grid::CELL_SIZE,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;
		}

		Evolve::RectDimension dims(
			Evolve::Origin::BOTTOM_LEFT,
			current.currentPositionInWorld_.X,
			current.currentPositionInWorld_.Y,
			BODY_SIZE,
			BODY_SIZE
		);

		// main drawing
		if (current.type_ == SnakePart::HEAD) {
			renderer.draw(ghostDims, uv, bodyTexture_.id, HEAD_COLOR);
			renderer.draw(dims, uv, bodyTexture_.id, HEAD_COLOR);
		}
		else {

			// have to print rotation cell
			if (current.rotations_[current.currentRotationIndex_].rotateDirection_ != SnakeDirection::NONE &&
				current.nextPositionInGridOffset_.isEqualTo
				(current.rotations_[current.currentRotationIndex_].rotatePositionInGrid_)) {
				
				Evolve::RectDimension rotationDims(
					Evolve::Origin::BOTTOM_LEFT,
					current.nextPositionInGridOffset_.X * BODY_SIZE,
					current.nextPositionInGridOffset_.Y * BODY_SIZE,
					BODY_SIZE,
					BODY_SIZE
				);

				renderer.draw(rotationDims, uv, bodyTexture_.id, BODY_COLOR);
			}
			renderer.draw(ghostDims, uv, bodyTexture_.id, BODY_COLOR);
			renderer.draw(dims, uv, bodyTexture_.id, BODY_COLOR);
		}
	}
}

void Snake::createNewPart(const SnakePart type, const Evolve::Position2D& positionInGrid, const SnakeDirection direction) {
	
	snake_.emplace_back(type, positionInGrid, direction, grid_->getNumRows(), grid_->getNumColumns());

	grid_->addSnakeCell(positionInGrid.Y, positionInGrid.X);
}

SnakeBodyPart::SnakeBodyPart(const SnakePart type, const Evolve::Position2D& positionInGrid, const SnakeDirection direction,
	const size_t numRows, const size_t numColumns) {

	type_ = type;
	direction_ = direction;

	currentPositionInGrid_ = positionInGrid;

	currentPositionInWorld_.X = currentPositionInGrid_.X * Snake::BODY_SIZE;
	currentPositionInWorld_.Y = currentPositionInGrid_.Y * Snake::BODY_SIZE;

	SetNextPosition(*this, numRows, numColumns);
}

bool SnakeBodyPart::OffsetPosition(Evolve::Position2D& position, size_t numRows, size_t numColumns) {
	if (position.X == -1) {
		position.X = (int) numColumns - 1;
		return true;
	}
	else if (position.X == numColumns) {
		position.X = 0;
		return true;
	}
	else if (position.Y == -1) {
		position.Y = (int) numRows - 1;
		return true;
	}
	else if (position.Y == numRows) {
		position.Y = 0;
		return true;
	}
	return false;
}

void SnakeBodyPart::SetNextPosition(SnakeBodyPart& part, size_t numRows, size_t numColumns) {
	
	switch (part.direction_) {

	case SnakeDirection::RIGHT:
		part.nextPositionInGrid_.X = part.currentPositionInGrid_.X + 1;
		part.nextPositionInGrid_.Y = part.currentPositionInGrid_.Y;
		break;

	case SnakeDirection::LEFT:
		part.nextPositionInGrid_.X = part.currentPositionInGrid_.X - 1;
		part.nextPositionInGrid_.Y = part.currentPositionInGrid_.Y;
		break;

	case SnakeDirection::UP:
		part.nextPositionInGrid_.X = part.currentPositionInGrid_.X;
		part.nextPositionInGrid_.Y = part.currentPositionInGrid_.Y + 1;
		break;

	case SnakeDirection::DOWN:
		part.nextPositionInGrid_.X = part.currentPositionInGrid_.X;
		part.nextPositionInGrid_.Y = part.currentPositionInGrid_.Y - 1;
		break;
	}

	part.nextPositionInWorld_.X = part.nextPositionInGrid_.X * Snake::BODY_SIZE;
	part.nextPositionInWorld_.Y = part.nextPositionInGrid_.Y * Snake::BODY_SIZE;

	part.nextPositionInGridOffset_ = part.nextPositionInGrid_;
	OffsetPosition(part.nextPositionInGridOffset_, numRows, numColumns);
}