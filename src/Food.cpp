#include "../include/Food.h"

const int Food::FOOD_SIZE = Grid::CELL_SIZE;

Food::Food() {}

Food::~Food() {
	
	for (int i = 0; i < 20; i++) {
		Evolve::ImageLoader::DeleteTexture(foodTextures_[i]);
	}
}

bool Food::init(Grid * grid) {
	grid_ = grid;

	getRandomX_ = std::uniform_int_distribution<int>(0, grid->getNumColumns() - 1);
	getRandomY_ = std::uniform_int_distribution<int>(0, grid->getNumRows() - 1);

	getRandomFood_ = std::uniform_int_distribution<size_t>(0, 19);

	{ // initializing food textures
		int index = 0;

		loadTexture("resources/images/foods/alpine-forget-me-not.png",	foodTextures_[index++]);
		loadTexture("resources/images/foods/blue-tea.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/broccoli.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/carrot.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/cheese.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/cherries.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/cherry-blossom.png",		foodTextures_[index++]);
		loadTexture("resources/images/foods/cupcake.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/daisy.png",					foodTextures_[index++]);
		loadTexture("resources/images/foods/doughnut.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/egg.png",					foodTextures_[index++]);
		loadTexture("resources/images/foods/green-tea.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/hamburguer.png",			foodTextures_[index++]);
		loadTexture("resources/images/foods/iris.png",					foodTextures_[index++]);
		loadTexture("resources/images/foods/pint.png",					foodTextures_[index++]);
		loadTexture("resources/images/foods/popcorn.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/purple-tea.png",			foodTextures_[index++]);
		loadTexture("resources/images/foods/raspberry.png",				foodTextures_[index++]);
		loadTexture("resources/images/foods/taco.png",					foodTextures_[index++]);
		loadTexture("resources/images/foods/tulip.png",					foodTextures_[index++]);
	}

	positionInGrid_.X = STARTING_X;
	positionInGrid_.Y = STARTING_Y;

	currentFoodIndex_ = getRandomFood_(randomEngine_);
	//currentFoodIndex_ = 0;

	grid_->addFoodCell(positionInGrid_.Y, positionInGrid_.X);

	return true;
}

void Food::restart() {
	consumed_ = false;
	grid_->clearCell(positionInGrid_.Y, positionInGrid_.X);

	positionInGrid_.X = STARTING_X;
	positionInGrid_.Y = STARTING_Y;

	currentFoodIndex_ = getRandomFood_(randomEngine_);
	//currentFoodIndex_ = 0;

	grid_->addFoodCell(positionInGrid_.Y, positionInGrid_.X);
}

void Food::draw(Evolve::TextureRenderer& renderer) const {

	static int fruitTextureSize = 40;

	static Evolve::UvDimension uv { 0.0f, 0.0f, 1.0f, 1.0f };
	static Evolve::ColorRgba color { 255, 255, 255, 255 };

	renderer.draw(
		Evolve::RectDimension(
			Evolve::Origin::BOTTOM_LEFT,
			positionInGrid_.X * FOOD_SIZE,
			positionInGrid_.Y * FOOD_SIZE,
			fruitTextureSize, fruitTextureSize
		),
		uv,
		foodTextures_[currentFoodIndex_].id,
		color
	);
}

void Food::reset() {
	
	consumed_ = false;

	grid_->clearCell(positionInGrid_.Y, positionInGrid_.X);

	int newX = getRandomX_(randomEngine_);
	int newY = getRandomY_(randomEngine_);

	while (true) {
		if (grid_->isEmptyCell(newY, newX)) {
			break;
		}

		newX = getRandomX_(randomEngine_);
		newY = getRandomY_(randomEngine_);		
	}

	positionInGrid_.X = newX;
	positionInGrid_.Y = newY;

	currentFoodIndex_ = getRandomFood_(randomEngine_);
	//currentFoodIndex_ = 0;

	grid_->addFoodCell(positionInGrid_.Y, positionInGrid_.X);
}
