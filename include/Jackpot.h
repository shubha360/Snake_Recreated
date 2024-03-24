#pragma once

#include "Consumable.h"

class Jackpot : public Consumable {
public:
	static const int JACKPOT_SIZE;

	Jackpot();
	~Jackpot();

	bool init(Grid* grid) override;

	void draw(Evolve::ShapeRenderer& renderer) override;

	void changePosition() override;

	void startTimer();

private:
};