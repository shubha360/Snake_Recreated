#pragma once

#include "Grid.h"

#include <random>

#include <Evolve/ShapeRenderer.h>

class Consumable {
public:
	virtual bool init(Grid* grid) = 0;

	virtual void restart() = 0;

	virtual void draw(Evolve::ShapeRenderer& renderer) = 0;

	virtual void reset() = 0;

	inline void consumed() { consumed_ = true; }
	inline bool isConsumed() const { return consumed_; }

protected:
	Grid* grid_ = nullptr;

	bool consumed_ = false;

	glm::ivec2 positionInGrid_ = glm::ivec2(0, 0);
	Evolve::ColorRgba color_ = {};

	static std::random_device seed_;
	static std::mt19937 randomEngine_;
};

