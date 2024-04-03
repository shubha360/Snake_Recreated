#pragma once

#include "Consumable.h"

class Jackpot : public Consumable {
public:
	static const int JACKPOT_SIZE;

	Jackpot();
	~Jackpot();

	bool init(Grid* grid) override;

	void restart() override;

	void draw(Evolve::TextureRenderer& renderer) const override;

	void drawTimer(Evolve::TextureRenderer& renderer) const;

	void reset() override;

	void startJackpot(int level);

	void update(float deltaTime);

	inline bool isLost() const { return lost_; }

private:
	bool lost_ = false;
	bool haveToReset_ = false;

	float maxTime_ = 300.0f;
	
	float currentTime_ = 0.0f;
	bool timerRunning_ = false;
	
	int windowWidth_ = 0, windowHeight_ = 0;

	int currentLevel_ = 1;

	// in glm::ivec2, x represents the horizontal position and y represents the vertical position

	std::vector<glm::ivec2> availablePositions_;
	size_t availablePositionIndex_ = 0;

	std::uniform_int_distribution<int> getRandomPosition_;
	std::uniform_int_distribution<int> getRandomJackpot_;

	Evolve::TextureData jackpotTextures_[5] {};
	int currentJackpot_ = 0;

	Evolve::TextureData timerTexture_;
	Evolve::ColorRgba timerColors_[5] {};

	static const Evolve::UvDimension uv;
};