#pragma once

#include "Consumable.h"

class Jackpot : public Consumable {
public:
	Jackpot();
	~Jackpot();

	bool init(Grid* grid) override;

	void draw(Evolve::ShapeRenderer& renderer) override;

	void changePosition() override;

private:
};