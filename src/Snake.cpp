#include "../include/Snake.h"

Snake::Snake() {	
}

Snake::~Snake() {
}

bool Snake::init(Grid* grid) {
	grid_ = grid;

	createNewSnakePart(SnakePart::TAIL, glm::ivec2(10, 20), SnakeDirection::RIGHT);
	createNewSnakePart(SnakePart::BODY, glm::ivec2(11, 20), SnakeDirection::RIGHT);
	createNewSnakePart(SnakePart::HEAD, glm::ivec2(12, 20), SnakeDirection::RIGHT);

	return true;
}

void Snake::move() {
	
	for (int i = snake_.size() - 1; i >= 0; i--) {

		auto& current = snake_[i];

		switch (current.direction_) {
	
		case SnakeDirection::RIGHT:
			movePart(current, glm::ivec2(current.position_.x + 1, current.position_.y));
			break;

		case SnakeDirection::LEFT:
			movePart(current, glm::ivec2(current.position_.x - 1, current.position_.y));
			break;

		case SnakeDirection::UP:
			movePart(current, glm::ivec2(current.position_.x, current.position_.y + 1));
			break;

		case SnakeDirection::DOWN:
			movePart(current, glm::ivec2(current.position_.x, current.position_.y - 1));
			break;
		}
	}
}

void Snake::movePart(SnakeBodyPart& part, const glm::ivec2& newPosition) {

	grid_->grid_[part.position_.y][part.position_.x] = ' ';
	grid_->grid_[newPosition.y][newPosition.x] = 'S';

	part.position_ = newPosition;
}

void Snake::changeDirection(const SnakeDirection newDirection) {

	auto& head = snake_.back();

	bool directionChanged = false;

	switch (newDirection) {

	case SnakeDirection::RIGHT:

		if (head.direction_ != SnakeDirection::RIGHT && head.direction_ != SnakeDirection::LEFT) {
			head.direction_ = SnakeDirection::RIGHT;
		}
		break;

	case SnakeDirection::LEFT:

		if (head.direction_ != SnakeDirection::RIGHT && head.direction_ != SnakeDirection::LEFT) {
			head.direction_ = SnakeDirection::LEFT;
		}
		break;

	case SnakeDirection::UP:

		if (head.direction_ != SnakeDirection::UP && head.direction_ != SnakeDirection::DOWN) {
			head.direction_ = SnakeDirection::UP;
		}
		break;

	case SnakeDirection::DOWN:

		if (head.direction_ != SnakeDirection::UP && head.direction_ != SnakeDirection::DOWN) {
			head.direction_ = SnakeDirection::DOWN;
		}
		break;
	}
}

Snake::SnakeBodyPart::SnakeBodyPart(const SnakePart type, const glm::ivec2& position, const SnakeDirection direction) {

	type_ = type;
	position_ = position;
	direction_ = direction;

}

void Snake::createNewSnakePart(const SnakePart type, const glm::ivec2& position, const SnakeDirection direction) {

	snake_.emplace_back(type, position, direction);

	grid_->grid_[position.y][position.x] = 'S';
}
