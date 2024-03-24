#include "../include/Fruit.h"

const int Fruit::FRUIT_SIZE = Grid::CELL_SIZE;

Fruit::Fruit() {}

Fruit::~Fruit() {}

bool Fruit::init(Grid * grid) {
	grid_ = grid;

	color_.set(0, 0, 100, 255);

	getRandomX_ = std::uniform_int_distribution<>(0, grid->getNumColumns() - 1);
	getRandomY_ = std::uniform_int_distribution<>(0, grid->getNumRows() - 1);

	changePosition();

	return true;
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

void Fruit::changePosition() {
	
	consumed_ = false;

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
