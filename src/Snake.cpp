#include "../include/Snake.h"

const int Snake::BODY_SIZE = Grid::CELL_SIZE;
const float Snake::SPEED = 5.0f;

Snake::Snake() {	
}

Snake::~Snake() {
}

bool Snake::init(Grid* grid) {

	snake_.reserve(currentSnakeMaxSize_);

	grid_ = grid;
	fruit_.init(grid_);

	createNewPart(SnakePart::HEAD, glm::ivec2(12, 10), SnakeDirection::RIGHT);
	createNewPart(SnakePart::BODY, glm::ivec2(11, 10), SnakeDirection::RIGHT);
	createNewPart(SnakePart::TAIL, glm::ivec2(10, 10), SnakeDirection::RIGHT);

	return true;
}

bool Snake::move(float deltaTime) {

	for (int i = 0; i < snake_.size(); i++) {

		auto& current = snake_[i];

		bool positionChangedInGrid = false;

		switch (current.direction_) {

		case SnakeDirection::RIGHT:
			current.currentPositionInWorld_.x += (int) (SPEED * deltaTime);

			if (current.currentPositionInWorld_.x >= current.nextPositionInWorld_.x) {
				
				positionChangedInGrid = true;
				
				current.currentPositionInWorld_.x = current.nextPositionInWorld_.x;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (current.currentPositionInGrid_.x == grid_->getNumColumns()) {
					current.currentPositionInGrid_.x = 0;
					current.currentPositionInWorld_.x = current.currentPositionInGrid_.x * BODY_SIZE;
				}
				
				current.nextPositionInGrid_.x = current.currentPositionInGrid_.x + 1;
				current.nextPositionInWorld_.x = current.nextPositionInGrid_.x * BODY_SIZE;
			}

			break;

		case SnakeDirection::LEFT:
			current.currentPositionInWorld_.x -= (int) (SPEED * deltaTime);

			if (current.currentPositionInWorld_.x <= current.nextPositionInWorld_.x) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.x = current.nextPositionInWorld_.x;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (current.currentPositionInGrid_.x == -1) {
					current.currentPositionInGrid_.x = grid_->getNumColumns() - 1;
					current.currentPositionInWorld_.x = current.currentPositionInGrid_.x * BODY_SIZE;
				}

				current.nextPositionInGrid_.x = current.currentPositionInGrid_.x - 1;
				current.nextPositionInWorld_.x = current.nextPositionInGrid_.x * BODY_SIZE;
			}

			break;

		case SnakeDirection::UP:
			current.currentPositionInWorld_.y += (int) (SPEED * deltaTime);

			if (current.currentPositionInWorld_.y >= current.nextPositionInWorld_.y) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.y = current.nextPositionInWorld_.y;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (current.currentPositionInGrid_.y == grid_->getNumRows()) {
					current.currentPositionInGrid_.y = 0;
					current.currentPositionInWorld_.y = current.currentPositionInGrid_.y * BODY_SIZE;
				}

				current.nextPositionInGrid_.y = current.currentPositionInGrid_.y + 1;
				current.nextPositionInWorld_.y = current.nextPositionInGrid_.y * BODY_SIZE;
			}
			break;

		case SnakeDirection::DOWN:
			current.currentPositionInWorld_.y -= (int) (SPEED * deltaTime);

			if (current.currentPositionInWorld_.y <= current.nextPositionInWorld_.y) {
				
				positionChangedInGrid = true;

				current.currentPositionInWorld_.y = current.nextPositionInWorld_.y;

				current.previousPositionInGrid_ = current.currentPositionInGrid_;
				current.currentPositionInGrid_ = current.nextPositionInGrid_;

				if (current.currentPositionInGrid_.y == -1) {
					current.currentPositionInGrid_.y = grid_->getNumRows() - 1;
					current.currentPositionInWorld_.y = current.currentPositionInGrid_.y * BODY_SIZE;
				}

				current.nextPositionInGrid_.y = current.currentPositionInGrid_.y - 1;
				current.nextPositionInWorld_.y = current.nextPositionInGrid_.y * BODY_SIZE;
			}
			break;
		}		

		if (positionChangedInGrid) {

			if (current.type_ == SnakePart::TAIL) {

				grid_->clearCell(current.previousPositionInGrid_.y, current.previousPositionInGrid_.x);

				if (snake_.size() - 1 > i) { // have to add new part if true

					auto& newTail = snake_[i + 1];

					if (current.previousPositionInGrid_ == newTail.currentPositionInGrid_) {

						current.type_ = SnakePart::BODY;

						newTail.type_ = SnakePart::TAIL;
						newTail.direction_ = current.direction_;
						newTail.nextPositionInGrid_ = current.currentPositionInGrid_;
						newTail.nextPositionInWorld_ = current.currentPositionInWorld_;

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

				current.currentRotationIndex_ = (current.currentRotationIndex_ + 1) % (current.rotations_.size() - 1);

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

				if (current.type_ != SnakePart::TAIL) {
					auto& next = snake_[i + 1];
						
					next.rotations_[next.newRotationIndex_].rotateDirection_ = current.direction_;
					next.rotations_[next.newRotationIndex_].rotatePositionInGrid_ = current.currentPositionInGrid_;

					next.newRotationIndex_ = (next.newRotationIndex_ + 1) % (next.rotations_.size() - 1);
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

					fruit_.changeFruitPosition();
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

			if (head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.y == grid_->getNumRows()) {
				head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.y = 0;
			}
			else if (head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.y == -1) {
				head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.y = grid_->getNumRows() - 1;
			}

			head.newRotationIndex_ = (head.newRotationIndex_ + 1) % (head.rotations_.size() - 1);
		}
		break;	

	case SnakeDirection::UP:
	case SnakeDirection::DOWN:

		if (head.direction_ == SnakeDirection::RIGHT || head.direction_ == SnakeDirection::LEFT) {

			head.rotations_[head.newRotationIndex_] = SnakeBodyPart::Rotation{ head.nextPositionInGrid_, newDirection };

			if (head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.x == grid_->getNumColumns()) {
				head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.x = 0;
			}
			else if (head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.x == -1) {
				head.rotations_[head.newRotationIndex_].rotatePositionInGrid_.x = grid_->getNumColumns() - 1;
			}

			head.newRotationIndex_ = (head.newRotationIndex_ + 1) % (head.rotations_.size() - 1);
		}
		break;
	}
}

void Snake::draw(Evolve::ShapeRenderer& renderer) {
	
	static Evolve::ColorRgba HEAD_COLOR{ 0, 200, 0, 255 };
	static Evolve::ColorRgba BODY_COLOR{ 0, 0, 200, 255 };
	static Evolve::ColorRgba TAIL_COLOR{ 200, 0, 0, 255 };

	fruit_.draw(renderer);

	for (int i = snake_.size() - 1; i >= 0; i--) {

		auto& current = snake_[i];

		Evolve::RectDimension ghostDims;

		switch (current.direction_) {
		case SnakeDirection::RIGHT:
			
			if (current.nextPositionInGrid_.x == grid_->getNumColumns()) {
				ghostDims.set(
					Evolve::Origin::BOTTOM_LEFT,
					current.currentPositionInWorld_.x - grid_->getNumColumns() * Grid::CELL_SIZE,
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
					current.currentPositionInWorld_.x + grid_->getNumColumns() * Grid::CELL_SIZE,
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
					current.currentPositionInWorld_.y - grid_->getNumRows() * Grid::CELL_SIZE,
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
					current.currentPositionInWorld_.y + grid_->getNumRows() * Grid::CELL_SIZE,
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
				current.nextPositionInGrid_ == current.rotations_[current.currentRotationIndex_].rotatePositionInGrid_) {
				
				Evolve::RectDimension rotationDims(
					Evolve::Origin::BOTTOM_LEFT,
					current.nextPositionInWorld_.x,
					current.nextPositionInWorld_.y,
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
	
	snake_.emplace_back(type, positionInGrid, direction, grid_->getNumColumns(), grid_->getNumRows());
	currentPartIndex_++;

	if (currentPartIndex_ == currentSnakeMaxSize_) {
		currentSnakeMaxSize_ *= 2;
		snake_.reserve(currentSnakeMaxSize_);
	}

	grid_->addSnakeCell(positionInGrid.y, positionInGrid.x);
}

SnakeBodyPart::SnakeBodyPart(const SnakePart type, const glm::ivec2& positionInGrid, const SnakeDirection direction,
	const int numCollumns, const int numRows) {

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

	rotations_.resize(4);
}
