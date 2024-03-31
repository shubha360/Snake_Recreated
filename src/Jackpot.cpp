#include "../include/Jackpot.h"

const int Jackpot::JACKPOT_SIZE = Grid::CELL_SIZE * 2;

Jackpot::Jackpot() {}

Jackpot::~Jackpot() {}

bool Jackpot::init(Grid* grid) {
	grid_ = grid;

	color_.set(255, 154, 114, 255);

	windowWidth_ = (int) grid_->getNumColumns() * Grid::CELL_SIZE;
	windowHeight_ = (int) grid_->getNumRows() * Grid::CELL_SIZE;
	
	availablePositions_.resize((size_t)((grid_->getNumRows() - 3) - 2) * (grid_->getNumColumns() - 1));

	return true;
}

void Jackpot::restart() {
	reset();
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

	// TIMER
	static float timerMaxWidth = windowWidth_ / 6.0f * 5.0f;

	int timerWidth = (int) (((maxTime_ - currentTime_) / maxTime_) * timerMaxWidth);

	renderer.drawRectangle(
		Evolve::RectDimension(
			Evolve::Origin::CENTER,
			windowWidth_ / 2,
			64,
			timerWidth, 16
		),
		color_
	);
}

void Jackpot::reset() {

	if (haveToReset_) {
		consumed_ = false;
		lost_ = false;
		timerRunning_ = false;
		haveToReset_ = false;

		grid_->clearCell(positionInGrid_.y, positionInGrid_.x, true);
	}
}

void Jackpot::startJackpot(int level) {	

	if (level > currentLevel_) {
		currentLevel_ = level;
		maxTime_ -= 20.0f;
	}

	currentTime_ = 0.0f;
	timerRunning_ = true;

	for (size_t row = 2; row < grid_->getNumRows() - 3; row++) {
		for (size_t column = 0; column < grid_->getNumColumns() - 1; column++) {
			
			if (grid_->isEmptyCell(row, column) &&
				grid_->isEmptyCell(row, column + 1) &&
				grid_->isEmptyCell(row + 1, column) &&
				grid_->isEmptyCell(row + 1, column + 1)) {

				availablePositions_[availablePositionIndex_].x = (int) column;
				availablePositions_[availablePositionIndex_].y = (int) row;
				availablePositionIndex_++;
			}
		}
	}

	getRandom_ = std::uniform_int_distribution<int>(0, (int) availablePositionIndex_ - 1);

	positionInGrid_ = availablePositions_[getRandom_(randomEngine_)];

	availablePositionIndex_ = 0;

	grid_->addJackpotCells(positionInGrid_.y, positionInGrid_.x);

	haveToReset_ = true;
}

void Jackpot::update(float deltaTime) {

	if (timerRunning_) {
		currentTime_ += deltaTime;

		if (currentTime_ >= maxTime_) {
			lost_ = true;
		}
	}
}
