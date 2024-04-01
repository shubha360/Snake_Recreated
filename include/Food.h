#pragma once

#include "Consumable.h"

class Food : public Consumable {
public:
	static const int FOOD_SIZE;

	Food();
	~Food();

	bool init(Grid* grid) override;

	void restart() override;

	void draw(Evolve::TextureRenderer& renderer) const override;

	void reset() override;

private:

	// in glm::ivec2, x represents the horizontal position and y represents the vertical position

	std::uniform_int_distribution<size_t> getRandomX_;
	std::uniform_int_distribution<size_t> getRandomY_;

	Evolve::TextureData foodTextures_[20];
	int currentFoodIndex_ = 0;

	std::uniform_int_distribution<size_t> getRandomFood_;
};