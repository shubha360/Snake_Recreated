#include "../include/Fruit.h"

const int Fruit::FRUIT_SIZE = Grid::CELL_SIZE;

std::random_device Fruit::seed_;
std::mt19937 Fruit::randomEngine_(seed_());

Fruit::Fruit() {}

Fruit::~Fruit() {}

bool Fruit::init(Grid * grid) {
	grid_ = grid;

	positionInGrid_.x = 20;
	positionInGrid_.y = 10;

	grid_->addFruitCell(positionInGrid_.y, positionInGrid_.x);

	getRandomX_ = std::uniform_int_distribution<>(0, grid->getNumColumns() - 1);
	getRandomY_ = std::uniform_int_distribution<>(0, grid->getNumRows() - 1);

	return false;
}

void Fruit::changeFruitPosition() {
	
	grid_->clearCell(positionInGrid_.y, positionInGrid_.x);

	int newX = getRandomX_(randomEngine_);
	int newY = getRandomY_(randomEngine_);

	while (!grid_->isEmptyCell(newY, newX)) {
		newX = getRandomX_(randomEngine_);
		newY = getRandomY_(randomEngine_);
	}

	positionInGrid_.x = newX;
	positionInGrid_.y = newY;

	grid_->addFruitCell(positionInGrid_.y, positionInGrid_.x);
}

void Fruit::draw(Evolve::ShapeRenderer& renderer) {
	renderer.drawRectangle(
		Evolve::RectDimension(
			Evolve::Origin::BOTTOM_LEFT,
			positionInGrid_.x * FRUIT_SIZE,
			positionInGrid_.y * FRUIT_SIZE,
			FRUIT_SIZE, FRUIT_SIZE
		),
		color_
	);
}
