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

	void startJackpot();

	void update(float deltaTime);

	inline bool isLost() const { return lost_; }

private:
	bool lost_ = false;

	const float maxTime_ = 300.0f;
	
	float currentTime_ = 0.0f;
	bool timerRunning_ = false;
	
	int windowWidth_ = 0, windowHeight_ = 0;
};