#include "../include/Fruit.h"

std::random_device Fruit::seed_;
std::mt19937 Fruit::randomEngine_(seed_());

Fruit::Fruit() {}

Fruit::~Fruit()
{}

bool Fruit::init(Snake* snake, const int minX, const int maxX, const int minY, const int maxY) {
	snake_ = snake;
	getRandomX_ = std::uniform_int_distribution<int>(minX, maxX);
	getRandomY_ = std::uniform_int_distribution<int>(minY, maxY);

	position_.x = maxX / 2;
	position_.y = maxY / 2;

	return true;
}

void Fruit::update() {
	if (isSnakePartInsideFruit(snake_->getSnakeHead())) {
		changeFruitPosition();
		snake_->addNewPart();
	}
}

void Fruit::changeFruitPosition() {
	position_.x = 0;
	position_.y = 0;
}

void Fruit::draw(Evolve::ShapeRenderer& renderer) {
	renderer.drawRectangle(
		Evolve::RectDimension(
			Evolve::Origin::TOP_LEFT,
			position_.x, position_.y,
			FRUIT_SIZE, FRUIT_SIZE
		),
		color_
	);
}

bool Fruit::isSnakePartInsideFruit(const SnakeBodyPart& part) {
	
	int fTop = position_.y, fBottom = position_.y - FRUIT_SIZE;
	int fLeft = position_.x, fRight = position_.x + FRUIT_SIZE;

	int sTop = part.position_.y, sBottom = part.position_.y - Snake::BODY_SIZE;
	int sLeft = part.position_.x, sRight = part.position_.x + Snake::BODY_SIZE;

	if (fBottom >= sTop || fTop <= sBottom || fRight <= sLeft || fLeft >= sRight) {
		return false;
	}

	return true;
}
