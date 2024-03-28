#include "../include/Snake.h"

const int Snake::BODY_SIZE = Grid::CELL_SIZE;
const float Snake::SPEED = 5.0f;

Snake::Snake() {}

Snake::~Snake() {}

bool Snake::init(Grid* grid, Fruit* fruit, Jackpot* jackpot) {

	snake_.reserve(currentSnakeMaxSize_);

	grid_ = grid;
	fruit_ = fruit;
	jackpot_ = jackpot;

	createNewPart(SnakePart::HEAD, glm::ivec2(12, 10), SnakeDirection::RIGHT);
	createNewPart(SnakePart::BODY, glm::ivec2(11, 10), SnakeDirection::RIGHT);
	createNewPart(SnakePart::TAIL, glm::ivec2(10, 10), SnakeDirection::RIGHT);

	return true;
}

void Snake::restart() {

	for (auto& part : snake_) {
		grid_->clearCell(part.currentPositionInGrid_.y, part.currentPositionInGrid_.x);
	}

	snake_.clear();

	currentSnakeMaxSize_ = startingSnakeReserveSize_;
	currentPartIndex_ = 0;
	snake_.reserve(currentSnakeMaxSize_);

	createNewPart(SnakePart::HEAD, glm::ivec2(12, 10), SnakeDirection::RIGHT);
	createNewPart(SnakePart::BODY, glm::ivec2(11, 10), SnakeDirection::RIGHT);
	createNewPart(SnakePart::TAIL, glm::ivec2(10, 10), SnakeDirection::RIGHT);
}

bool Snake::move(float deltaTime, int level) {

	for (int i = 0; i < snake_.size(); i++) {

		auto& current = snake_[i];

		bool positionChangedInGrid = false;

		switch (current.direction_) {

		case SnakeDirection::RIGHT:
			current.currentPositionInWorld_.x += (int)((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.x >= current.nextPositionInWorld_.x) {
				
				positionChangedInGrid = true;
				
				current.currentPositionInWorld_.x = current.nextPositionInWorld_.x;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

					
				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					current.currentPositionInWorld_.x = current.currentPositionInGrid_.x * BODY_SIZE;
				}
				
				current.nextPositionInGrid_.x = current.currentPositionInGrid_.x + 1;
				current.nextPositionInWorld_.x = current.nextPositionInGrid_.x * BODY_SIZE;
			}

			break;

		case SnakeDirection::LEFT:
			current.currentPositionInWorld_.x -= (int) ((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.x <= current.nextPositionInWorld_.x) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.x = current.nextPositionInWorld_.x;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					current.currentPositionInWorld_.x = current.currentPositionInGrid_.x * BODY_SIZE;
				}

				current.nextPositionInGrid_.x = current.currentPositionInGrid_.x - 1;
				current.nextPositionInWorld_.x = current.nextPositionInGrid_.x * BODY_SIZE;
			}

			break;

		case SnakeDirection::UP:
			current.currentPositionInWorld_.y += (int) ((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.y >= current.nextPositionInWorld_.y) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.y = current.nextPositionInWorld_.y;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					current.currentPositionInWorld_.y = current.currentPositionInGrid_.y * BODY_SIZE;
				}

				current.nextPositionInGrid_.y = current.currentPositionInGrid_.y + 1;
				current.nextPositionInWorld_.y = current.nextPositionInGrid_.y * BODY_SIZE;
			}
			break;

		case SnakeDirection::DOWN:
			current.currentPositionInWorld_.y -= (int) ((SPEED + level - 1) * deltaTime);

			if (current.currentPositionInWorld_.y <= current.nextPositionInWorld_.y) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.y = current.nextPositionInWorld_.y;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (SnakeBodyPart::OffsetPosition(current.currentPositionInGrid_,
					grid_->getNumRows(), grid_->getNumColumns())) {

					current.currentPositionInWorld_.y = current.currentPositionInGrid_.y * BODY_SIZE;
				}

				current.nextPositionInGrid_.y = current.currentPositionInGrid_.y - 1;
				current.nextPositionInWorld_.y = current.nextPositionInGrid_.y * BODY_SIZE;
			}
			break;
		}		

		if (positionChangedInGrid) {

			current.nextPositionInGridOffset_ = current.nextPositionInGrid_;

			SnakeBodyPart::OffsetPosition(current.nextPositionInGridOffset_,
				grid_->getNumRows(), grid_->getNumColumns());

			if (current.type_ == SnakePart::TAIL) {

				grid_->clearCell(current.previousPositionInGrid_.y, current.previousPositionInGrid_.x);

				if (snake_.size() - 1 > i) { // have to add new part if true

					auto& newTail = snake_[(size_t) i + 1];

					if (current.previousPositionInGrid_ == newTail.currentPositionInGrid_) {

						current.type_ = SnakePart::BODY;

						newTail.type_ = SnakePart::TAIL;
						newTail.direction_ = current.direction_;
						newTail.nextPositionInGrid_ = current.currentPositionInGrid_;
						newTail.nextPositionInWorld_ = current.currentPositionInWorld_;

						newTail.nextPositionInGridOffset_ = newTail.nextPositionInGrid_;

						SnakeBodyPart::OffsetPosition(newTail.nextPositionInGridOffset_,
							grid_->getNumRows(), grid_->getNumColumns());

						grid_->addSnakeCell(newTail.currentPositionInGrid_.y, newTail.currentPositionInGrid_.x);
					}
				}
			}

			// ROTATION
			auto& currentRotation = current.rotations_[current.currentRotationIndex_];

			if (currentRotation.rotateDirection_ != SnakeDirection::NONE &&
				current.currentPositionInGrid_ == currentRotation.rotatePositionInGrid_) {

				current.direction_ = currentRotation.rotateDirection_;
				currentRotation.rotateDirection_ = SnakeDirection::NONE;

				current.currentRotationIndex_ = (current.currentRotationIndex_ + 1) % ((int) current.rotations_.size() - 1);

				switch (current.direction_) {
				case SnakeDirection::RIGHT:
					current.nextPositionInGrid_.x = current.currentPositionInGrid_.x + 1;
					current.nextPositionInGrid_.y = current.currentPositionInGrid_.y;
					break;

				case SnakeDirection::LEFT:
					current.nextPositionInGrid_.x = current.currentPositionInGrid_.x - 1;
					current.nextPositionInGrid_.y = current.currentPositionInGrid_.y;
					break;

				case SnakeDirection::UP:
					current.nextPositionInGrid_.x = current.currentPositionInGrid_.x;
					current.nextPositionInGrid_.y = current.currentPositionInGrid_.y + 1;
					break;

				case SnakeDirection::DOWN:
					current.nextPositionInGrid_.x = current.currentPositionInGrid_.x;
					current.nextPositionInGrid_.y = current.currentPositionInGrid_.y - 1;
					break;
				}

				current.nextPositionInWorld_.x = current.nextPositionInGrid_.x * Snake::BODY_SIZE;
				current.nextPositionInWorld_.y = current.nextPositionInGrid_.y * Snake::BODY_SIZE;

				current.nextPositionInGridOffset_ = current.nextPositionInGrid_;

				SnakeBodyPart::OffsetPosition(current.nextPositionInGridOffset_,
					grid_->getNumRows(), grid_->getNumColumns());

				if (current.type_ != SnakePart::TAIL) {
					auto& next = snake_[(size_t) i + 1];
						
					next.rotations_[next.newRotationIndex_].rotateDirection_ = current.direction_;
					next.rotations_[next.newRotationIndex_].rotatePositionInGrid_ = current.currentPositionInGrid_;

					next.newRotationIndex_ = (next.newRotationIndex_ + 1) % ((int) next.rotations_.size() - 1);
				}
			}


			if (current.type_ == SnakePart::HEAD) {
				
				// ATE FRUIT
				if (grid_->isSnakeCell(current.currentPositionInGrid_.y, current.currentPositionInGrid_.x)) {
					std::cout << "DIE!!!!!\n\n\n";
					return false;
				}
				else if (grid_->isFruitCell(current.currentPositionInGrid_.y, current.currentPositionInGrid_.x)) {

					createNewPart(
						SnakePart::NONE,
						glm::ivec2(current.currentPositionInGrid_.x, current.currentPositionInGrid_.y),
						SnakeDirection::NONE
					);

					fruit_->consumed();
				} 
				else if (grid_->isJackpotCell(current.currentPositionInGrid_.y, current.currentPositionInGrid_.x)) {
					
					createNewPart(
						SnakePart::NONE,
						glm::ivec2(current.currentPositionInGrid_.x, current.currentPositionInGrid_.y),
						SnakeDirection::NONE
					);

					jackpot_->consumed();
				}
				else {
					grid_->addSnakeCell(current.currentPositionInGrid_.y, current.currentPositionInGrid_.x);
				}
			}


		}
	}

	return true;
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

			head.newRotationIndex_ = (head.newRotationIndex_ + 1) % ((int) head.rotations_.size() - 1);
		}
		break;	

	case SnakeDirection::UP:
	case SnakeDirection::DOWN:

		if (head.direction_ == SnakeDirection::RIGHT || head.direction_ == SnakeDirection::LEFT) {

			head.rotations_[head.newRotationIndex_] = SnakeBodyPart::Rotation{ head.nextPositionInGrid_, newDirection };

			SnakeBodyPart::OffsetPosition(head.rotations_[head.newRotationIndex_].rotatePositionInGrid_,
				grid_->getNumRows(), grid_->getNumColumns());

			head.newRotationIndex_ = (head.newRotationIndex_ + 1) % ((int) head.rotations_.size() - 1);
		}
		break;
	}
}

void Snake::draw(Evolve::ShapeRenderer& renderer) {
	
	static Evolve::ColorRgba HEAD_COLOR{ 0, 200, 0, 255 };
	static Evolve::ColorRgba BODY_COLOR{ 0, 0, 200, 255 };
	static Evolve::ColorRgba TAIL_COLOR{ 200, 0, 0, 255 };

	for (int i = (int) snake_.size() - 1; i >= 0; i--) {

		auto& current = snake_[i];

		Evolve::RectDimension ghostDims;

		switch (current.direction_) {
		case SnakeDirection::RIGHT:
			
			if (current.nextPositionInGrid_.x == grid_->getNumColumns()) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.x - (int) grid_->getNumColumns() * Grid::CELL_SIZE,
					current.currentPositionInWorld_.y,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;

		case SnakeDirection::LEFT:

			if (current.nextPositionInGrid_.x == -1) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.x + (int) grid_->getNumColumns() * Grid::CELL_SIZE,
					current.currentPositionInWorld_.y,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;

		case SnakeDirection::UP:

			if (current.nextPositionInGrid_.y == grid_->getNumRows()) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.x,
					current.currentPositionInWorld_.y - (int) grid_->getNumRows() * Grid::CELL_SIZE,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;

		case SnakeDirection::DOWN:

			if (current.nextPositionInGrid_.y == -1) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.x,
					current.currentPositionInWorld_.y + (int) grid_->getNumRows() * Grid::CELL_SIZE,
					BODY_SIZE,
					BODY_SIZE
				);
			}
			break;
		}

		Evolve::RectDimension dims(
			Evolve::Origin::BOTTOM_LEFT,
			current.currentPositionInWorld_.x,
			current.currentPositionInWorld_.y,
			BODY_SIZE,
			BODY_SIZE
		);

		if (current.type_ == SnakePart::HEAD) {
			renderer.drawRectangle(ghostDims, HEAD_COLOR);
			renderer.drawRectangle(dims, HEAD_COLOR);
		}
		else {
			
			// have to print rotation cell
			if (current.rotations_[current.currentRotationIndex_].rotateDirection_ != SnakeDirection::NONE &&
				current.nextPositionInGridOffset_ == current.rotations_[current.currentRotationIndex_].rotatePositionInGrid_) {
				
				Evolve::RectDimension rotationDims(
					Evolve::Origin::BOTTOM_LEFT,
					current.nextPositionInGridOffset_.x * BODY_SIZE,
					current.nextPositionInGridOffset_.y * BODY_SIZE,
					BODY_SIZE,
					BODY_SIZE
				);

				renderer.drawRectangle(rotationDims, TAIL_COLOR);
			}
			renderer.drawRectangle(ghostDims, TAIL_COLOR);
			renderer.drawRectangle(dims, TAIL_COLOR);
		}
	}
}

void Snake::createNewPart(const SnakePart type, const glm::ivec2& positionInGrid, const SnakeDirection direction) {
	
	snake_.emplace_back(type, positionInGrid, direction, grid_->getNumRows(), grid_->getNumColumns());
	currentPartIndex_++;

	if (currentPartIndex_ == currentSnakeMaxSize_) {
		currentSnakeMaxSize_ *= 2;
		snake_.reserve(currentSnakeMaxSize_);
	}

	grid_->addSnakeCell(positionInGrid.y, positionInGrid.x);
}

SnakeBodyPart::SnakeBodyPart(const SnakePart type, const glm::ivec2& positionInGrid, const SnakeDirection direction,
	const size_t numRows, const size_t numCollumns) {

	type_ = type;
	direction_ = direction;

	currentPositionInGrid_ = positionInGrid;

	currentPositionInWorld_.x = currentPositionInGrid_.x * Snake::BODY_SIZE;
	currentPositionInWorld_.y = currentPositionInGrid_.y * Snake::BODY_SIZE;

	switch (direction_) {
	
	case SnakeDirection::RIGHT:
		nextPositionInGrid_.x = currentPositionInGrid_.x + 1;
		nextPositionInGrid_.y = currentPositionInGrid_.y;
		break;

	case SnakeDirection::LEFT:
		nextPositionInGrid_.x = currentPositionInGrid_.x - 1;
		nextPositionInGrid_.y = currentPositionInGrid_.y;
		break;

	case SnakeDirection::UP:
		nextPositionInGrid_.x = currentPositionInGrid_.x;
		nextPositionInGrid_.y = currentPositionInGrid_.y + 1;
		break;

	case SnakeDirection::DOWN:
		nextPositionInGrid_.x = currentPositionInGrid_.x;
		nextPositionInGrid_.y = currentPositionInGrid_.y - 1;
		break;

	default:
		nextPositionInGrid_.x = 0;
		nextPositionInGrid_.y = 0;
		break;
	}

	nextPositionInWorld_.x = nextPositionInGrid_.x * Snake::BODY_SIZE;
	nextPositionInWorld_.y = nextPositionInGrid_.y * Snake::BODY_SIZE;

	nextPositionInGridOffset_ = nextPositionInGrid_;
	OffsetPosition(nextPositionInGridOffset_, numRows, numCollumns);

	rotations_.resize(4);
}

bool SnakeBodyPart::OffsetPosition(glm::ivec2& position, size_t numRows, size_t numColumns) {
	if (position.x == -1) {
		position.x = (int) numColumns - 1;
		return true;
	}
	else if (position.x == numColumns) {
		position.x = 0;
		return true;
	}
	else if (position.y == -1) {
		position.y = (int) numRows - 1;
		return true;
	}
	else if (position.y == numRows) {
		position.y = 0;
		return true;
	}
	return false;
}
