#pragma once

#include "Consumable.h"

class Fruit : public Consumable {
public:
	static const int FRUIT_SIZE;

	Fruit();
	~Fruit();

	bool init(Grid* grid) override;

	void draw(Evolve::ShapeRenderer& renderer) override;

	void reset() override;

private:

	/*std::uniform_int_distribution<int> getRandomRed_;
	std::uniform_int_distribution<int> getRandomYGreen_;*/
};