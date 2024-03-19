#include "../include/Snake.h"

const int Snake::BODY_SIZE = 36;
const int Snake::SPEED = 6;

Snake::Snake() {	
}

Snake::~Snake() {
}

bool Snake::init(const int windowWidth, const int windowHeight) {
	windowWidth_ = windowWidth;
	windowHeight_ = windowHeight;

	int startingX = 100;

	snake_.emplace_back(SnakePart::TAIL, glm::ivec2(startingX, 500), SnakeDirection::RIGHT);
	snake_.emplace_back(SnakePart::BODY, glm::ivec2(startingX + BODY_SIZE, 500), SnakeDirection::RIGHT);
	snake_.emplace_back(SnakePart::HEAD, glm::ivec2(startingX + BODY_SIZE * 2, 500), SnakeDirection::RIGHT);

	return true;
}

void Snake::move() {

	for (int i = snake_.size() - 1; i >= 0; i--) {

		auto& current = snake_[i];

		current.currentRotation_ %= current.rotations_.size() - 1;

		auto& currentRotation = current.rotations_[current.currentRotation_];

		if (currentRotation.rotateDirection_ != SnakeDirection::NONE) {
			
			bool directionChanged = false;

			if (current.type_ == SnakePart::HEAD) {
				directionChanged = true;
			}
			else {
				if (current.direction_ == SnakeDirection::RIGHT &&
					current.position_.x >= currentRotation.rotatePosition_.x &&
					current.horizontalLoopFlag_ == snake_[i + 1].horizontalLoopFlag_) {

					directionChanged = true;
				}

				else if (current.direction_ == SnakeDirection::LEFT &&
					current.position_.x <= currentRotation.rotatePosition_.x && 
					current.horizontalLoopFlag_ == snake_[i + 1].horizontalLoopFlag_) {
					
					directionChanged = true;
				}

				else if (current.direction_ == SnakeDirection::UP &&
					current.position_.y >= currentRotation.rotatePosition_.y &&
					current.verticalLoopFlag_ == snake_[i + 1].verticalLoopFlag_) {

					directionChanged = true;
				}

				else if (current.direction_ == SnakeDirection::DOWN &&
					current.position_.y <= currentRotation.rotatePosition_.y && 
					current.verticalLoopFlag_ == snake_[i + 1].verticalLoopFlag_) {
					
					directionChanged = true;
				}
			}

			if (directionChanged) {
				if (i > 0) {

					auto& next = snake_[i - 1];

					// insert circularly
					next.newRotation_ %= (next.rotations_.size() - 1);

					next.rotations_[next.newRotation_++] = 
						SnakeBodyPart::Rotation{ 
						currentRotation.rotatePosition_,
						currentRotation.rotateDirection_
					};
				}

				current.direction_ = currentRotation.rotateDirection_;
				currentRotation.rotateDirection_ = SnakeDirection::NONE;
				current.currentRotation_++;
			}
		}

		switch (current.direction_) {
	
		case SnakeDirection::RIGHT:
			current.position_.x += SPEED;
			break;

		case SnakeDirection::LEFT:
			current.position_.x -= SPEED;
			break;

		case SnakeDirection::UP:
			current.position_.y += SPEED;
			break;

		case SnakeDirection::DOWN:
			current.position_.y -= SPEED;
			break;
		}

		if ((current.position_.x + BODY_SIZE) <= 0) {
			current.position_.x = windowWidth_ - BODY_SIZE;
			current.horizontalLoopFlag_ = !current.horizontalLoopFlag_;
		}
		else if (current.position_.x >= windowWidth_) {
			current.position_.x = 0;
			current.horizontalLoopFlag_ = !current.horizontalLoopFlag_;
		}

		if (current.position_.y <= 0) {
			current.position_.y = windowHeight_;
			current.verticalLoopFlag_ = !current.verticalLoopFlag_;
		}
		else if (current.position_.y - BODY_SIZE >= windowHeight_) {
			current.position_.y = BODY_SIZE;
			current.verticalLoopFlag_ = !current.verticalLoopFlag_;
		}
	}
}

void Snake::changeDirection(const SnakeDirection newDirection) {

	auto& head = snake_.back();

	switch (newDirection) {

	case SnakeDirection::RIGHT:
	case SnakeDirection::LEFT:

		if (head.direction_ == SnakeDirection::UP || head.direction_ == SnakeDirection::DOWN) {

			// insert circularly
			head.newRotation_ %= (head.rotations_.size() - 1);

			head.rotations_[head.newRotation_++] = SnakeBodyPart::Rotation { head.position_, newDirection };
		}
		break;	

	case SnakeDirection::UP:
	case SnakeDirection::DOWN:

		if (head.direction_ == SnakeDirection::RIGHT || head.direction_ == SnakeDirection::LEFT) {
			
			// insert circularly
			head.newRotation_ %= (head.rotations_.size() - 1);

			head.rotations_[head.newRotation_++] = SnakeBodyPart::Rotation{ head.position_, newDirection };
		}
		break;
	}
}

void Snake::printSnake(Evolve::ShapeRenderer& renderer) {
	
	static Evolve::ColorRgba HEAD_COLOR{ 0, 200, 0, 255 };
	static Evolve::ColorRgba BODY_COLOR{ 0, 0, 200, 255 };
	static Evolve::ColorRgba TAIL_COLOR{ 200, 0, 0, 255 };

	for (auto& current : snake_) {
		Evolve::RectDimension dims(
			Evolve::Origin::TOP_LEFT,
			current.position_.x,
			current.position_.y,
			BODY_SIZE,
			BODY_SIZE
		);

		if (current.type_ == SnakePart::HEAD) {
			renderer.drawRectangle(dims, HEAD_COLOR);
		}
		else if (current.type_ == SnakePart::BODY) {
			renderer.drawRectangle(dims, BODY_COLOR);
		}
		else {
			renderer.drawRectangle(dims, TAIL_COLOR);
		}
	}
}

void Snake::addNewPart() {

	auto& head = snake_.back();

	glm::ivec2 newPosition {};

	switch (head.direction_) {
	
	case SnakeDirection::RIGHT:
		newPosition.x = head.position_.x + BODY_SIZE;
		newPosition.y = head.position_.y;
		break;

	case SnakeDirection::LEFT:
		newPosition.x = head.position_.x - BODY_SIZE;
		newPosition.y = head.position_.y;
		break;

	case SnakeDirection::UP:
		newPosition.x = head.position_.x;
		newPosition.y = head.position_.y + BODY_SIZE;
		break;

	case SnakeDirection::DOWN:
		newPosition.x = head.position_.x;
		newPosition.y = head.position_.y - BODY_SIZE;
		break;
	}

	head.type_ = SnakePart::BODY;
	snake_.emplace_back(SnakePart::HEAD, newPosition, head.direction_);
}

SnakeBodyPart::SnakeBodyPart(const SnakePart type, const glm::ivec2& position, const SnakeDirection direction) {

	type_ = type;
	position_ = position;
	direction_ = direction;

	rotations_.resize(4);
}
