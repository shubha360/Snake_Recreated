#include "../include/Jackpot.h"

const int Jackpot::JACKPOT_SIZE = Grid::CELL_SIZE * 2;
const Evolve::UvDimension Jackpot::uv { 0.0f, 0.0f, 1.0f, 1.0f };

Jackpot::Jackpot() {}

Jackpot::~Jackpot() {	
	for (int i = 0; i < 5; i++) {
		Evolve::ImageLoader::DeleteTexture(jackpotTextures_[i]);
	}
	Evolve::ImageLoader::DeleteTexture(timerTexture_);
}

bool Jackpot::init(Grid* grid) {
	grid_ = grid;

	windowWidth_ = (int) grid_->getNumColumns() * Grid::CELL_SIZE;
	windowHeight_ = (int) grid_->getNumRows() * Grid::CELL_SIZE;
	
	availablePositions_.resize((size_t) ((grid_->getNumRows() - 3) - 2) * (grid_->getNumColumns() - 1));

	getRandomJackpot_ = std::uniform_int_distribution<size_t>(0, 4);

	{ // initializing jackpot and timer textures and colors
		int index = 0;

		loadTexture("resources/images/jackpots/diamond.png",	jackpotTextures_[index++]);
		loadTexture("resources/images/jackpots/earth.png",		jackpotTextures_[index++]);
		loadTexture("resources/images/jackpots/fireworks.png",	jackpotTextures_[index++]);
		loadTexture("resources/images/jackpots/rock.png",		jackpotTextures_[index++]);
		loadTexture("resources/images/jackpots/star.png",		jackpotTextures_[index++]);

		timerColors_[0].set ( 255,  57, 151, 200 );
		timerColors_[1].set (   0, 140, 223, 200 );
		timerColors_[2].set ( 142,  81, 217, 200 );
		timerColors_[3].set (  74,  74, 104, 200 );
		timerColors_[4].set ( 255, 200,  80, 200 );

		loadTexture("resources/images/timer.png", timerTexture_);
	}

	return true;
}

void Jackpot::restart() {
	reset();
}

void Jackpot::draw(Evolve::TextureRenderer& renderer) const {
	
	static const Evolve::ColorRgba texColor { 255, 255, 255, 255 };

	renderer.draw(
		Evolve::RectDimension(
			Evolve::Origin::BOTTOM_LEFT,
			positionInGrid_.X * Grid::CELL_SIZE,
			positionInGrid_.Y * Grid::CELL_SIZE,
			JACKPOT_SIZE, JACKPOT_SIZE
		),
		uv,
		jackpotTextures_[currentJackpot_].id,
		texColor
	);
}

void Jackpot::drawTimer(Evolve::TextureRenderer& renderer) const {
	static float timerMaxWidth = windowWidth_ / 6.0f * 5.0f;

	int timerWidth = (int)(((maxTime_ - currentTime_) / maxTime_) * timerMaxWidth);

	renderer.draw(
		Evolve::RectDimension(
			Evolve::Origin::CENTER,
			windowWidth_ / 2,
			64,
			timerWidth, 16
		),
		uv,
		timerTexture_.id,
		timerColors_[currentJackpot_]
	);
}

void Jackpot::reset() {

	if (haveToReset_) {
		consumed_ = false;
		lost_ = false;
		timerRunning_ = false;
		haveToReset_ = false;

		grid_->clearCell(positionInGrid_.Y, positionInGrid_.X, true);
	}
}

void Jackpot::startJackpot(int level) {	

	if (level > currentLevel_) {
		currentLevel_ = level;
		maxTime_ -= 20.0f;
	}

	currentTime_ = 0.0f;
	timerRunning_ = true;

	for (int row = 2; row < grid_->getNumRows() - 3; row++) {
		for (int column = 0; column < grid_->getNumColumns() - 1; column++) {
			
			if (grid_->isEmptyCell(row, column) &&
				grid_->isEmptyCell(row, column + 1) &&
				grid_->isEmptyCell(row + 1, column) &&
				grid_->isEmptyCell(row + 1, column + 1)) {

				availablePositions_[availablePositionIndex_].X = (int) column;
				availablePositions_[availablePositionIndex_].Y = (int) row;
				availablePositionIndex_++;
			}
		}
	}

	getRandomPosition_ = std::uniform_int_distribution<size_t>(0, (int) availablePositionIndex_ - 1);

	positionInGrid_ = availablePositions_[getRandomPosition_(randomEngine_)];

	availablePositionIndex_ = 0;

	grid_->addJackpotCells(positionInGrid_.Y, positionInGrid_.X);

	currentJackpot_ = getRandomJackpot_(randomEngine_);

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
