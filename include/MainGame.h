#pragma once

#include <Evolve/Window.h>
#include <Evolve/ColorRgba.h>
#include <Evolve/InputProcessor.h>
#include <Evolve/ShapeRenderer.h>
#include <Evolve/Camera.h>
#include <Evolve/Fps.h>

#include "Snake.h"
#include "Fruit.h"
#include "Grid.h"
#include "Jackpot.h"

enum class GameState {
	MAIN_MENU, PLAY, PAUSE, ENDED, EXIT
};

class MainGame {
public:
	MainGame();
	~MainGame();

	bool init();
	void run();

private:
	const float MAX_FPS = 60.0f;
	const Evolve::ColorRgba CLEAR_COLOR { 230, 230, 230, 255 };
	GameState gameState_ = GameState::PLAY;
	unsigned int windowWidth_ = 0, windowHeight_ = 0;

	Evolve::Window window_;
	Evolve::Camera camera_;
	Evolve::Fps fps_;
	Evolve::InputProcessor inputProcessor_;
	Evolve::ShapeRenderer shapeRenderer_;
	Evolve::TextureRenderer textureRenderer_;

	Grid grid_;
	Snake snake_;
	Fruit fruit_;
	Jackpot jackpot_;

	int score_ = 0;
	int fruitsConsumed_ = 0;
	int jackpotsConsumed_ = 0;
	int level = 1;

	bool jackpotVisible_ = false;

	bool initEngineComps();
	bool initGame();

	void gameLoop();
	float runGameSimulations(float previousTicks);

	void updateSnake(float deltaTime, bool& inputProcessed);
	void updateScoreAndLevel();

	void processInput();
	void draw();
	
	void printFps();

	void freeSnake();
};