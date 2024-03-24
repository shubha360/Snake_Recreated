#include "../include/Jackpot.h"

const int Jackpot::JACKPOT_SIZE = Grid::CELL_SIZE * 2;

Jackpot::Jackpot() {}

Jackpot::~Jackpot() {}

bool Jackpot::init(Grid* grid) {
	grid_ = grid;

	color_.set(100, 0, 0, 255);

	getRandomX_ = std::uniform_int_distribution<>(0, grid->getNumColumns() - 2);
	getRandomY_ = std::uniform_int_distribution<>(0, grid->getNumRows() - 2);	

	/*positionInGrid_.x = 1;
	positionInGrid_.y = 0;*/

	return true;
}

void Jackpot::draw(Evolve::ShapeRenderer& renderer) {
	
	renderer.drawRectangle(
		Evolve::RectDimension(
			Evolve::Origin::BOTTOM_LEFT,
			positionInGrid_.x * Grid::CELL_SIZE,
			positionInGrid_.y * Grid::CELL_SIZE,
			JACKPOT_SIZE, JACKPOT_SIZE
		),
		color_
	);
}

void Jackpot::changePosition() {
	
	consumed_ = false;

	grid_->clearCell(positionInGrid_.y, positionInGrid_.x, true);

	int newX = getRandomX_(randomEngine_);
	int newY = getRandomY_(randomEngine_);

	while (true) {
		if (grid_->isEmptyCell(newY, newX) &&
			grid_->isEmptyCell(newY, newX + 1) &&
			grid_->isEmptyCell(newY + 1, newX) &&
			grid_->isEmptyCell(newY + 1, newX + 1)) {

			break;
		}
		
		newX = getRandomX_(randomEngine_);
		newY = getRandomY_(randomEngine_);
	}

	positionInGrid_.x = newX;
	positionInGrid_.y = newY;

	grid_->addJackpotCells(positionInGrid_.y, positionInGrid_.x);
}

void Jackpot::startTimer() {
}
