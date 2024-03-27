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

	void startJackpot(int level);

	void update(float deltaTime);

	inline bool isLost() const { return lost_; }

	inline float getMaxTime() const { return maxTime_; }

private:
	bool lost_ = false;

	float maxTime_ = 300.0f;
	
	float currentTime_ = 0.0f;
	bool timerRunning_ = false;
	
	int windowWidth_ = 0, windowHeight_ = 0;

	int currentLevel_ = 1;

	// in glm::ivec2, x represents the horizontal position and y represents the vertical position

	std::vector<glm::ivec2> availablePositions_;
	size_t availablePositionIndex_ = 0;

	std::uniform_int_distribution<int> getRandom_;
};