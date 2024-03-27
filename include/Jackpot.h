#pragma once

#include "Consumable.h"

class Jackpot : public Consumable {
public:
	static const int JACKPOT_SIZE;

	Jackpot();
	~Jackpot();

	bool init(Grid* grid) override;

	void draw(Evolve::ShapeRenderer& renderer) override;

	void reset() override;

	void startJackpot();

	void update(float deltaTime);

	inline bool isLost() const { return lost_; }

private:
	bool lost_ = false;

	const float maxTime_ = 300.0f;
	
	float currentTime_ = 0.0f;
	bool timerRunning_ = false;
	
	int windowWidth_ = 0, windowHeight_ = 0;

	// in glm::ivec2, x represents the horizontal position and y represents the vertical position

	std::vector<glm::ivec2> availablePositions_;
	int availablePositionIndex_ = 0;

	std::uniform_int_distribution<int> getRandom_;
};