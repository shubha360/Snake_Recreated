#pragma once

#include <string>

#include <Evolve/Window.h>
#include <Evolve/ColorRgba.h>
#include <Evolve/InputProcessor.h>
#include <Evolve/ShapeRenderer.h>
#include <Evolve/Camera.h>
#include <Evolve/Fps.h>
#include <Evolve/Gui.h>
#include <Evolve/GuiRenderer.h>

#include "Snake.h"
#include "Fruit.h"
#include "Grid.h"
#include "Jackpot.h"

enum class GameState {
	MAIN_MENU, PLAY, PAUSE, ENDED, QUIT
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
	Evolve::Gui gui_;
	Evolve::GuiRenderer guiRenderer_;
	Evolve::Font viniqueFont32_;
	Evolve::Font viniqueFont128_;
	Evolve::Font viniqueFont16_;

	// gui font ids
	size_t guiFont_vinique32_ = -1;
	size_t guiFont_vinique128_ = -1;
	size_t guiFont_vinique16_ = -1;

	// gui component ids
	size_t gui_scoreText_ = -1;
	size_t gui_escText_ = -1;
	size_t gui_levelUpText_ = -1;

	size_t gui_bgPanel_ = -1;

	size_t gui_snakeText_ = -1;
	size_t gui_startButton_ = -1;

	size_t gui_pauseText_ = -1;
	size_t gui_resumeText_ = -1;
	size_t gui_restartButton_ = -1;
	size_t gui_quitButton_ = -1;

	Grid grid_;
	Snake snake_;
	Fruit fruit_;
	Jackpot jackpot_;

	int score_ = 0;
	int fruitsConsumed_ = 0;
	int level_ = 1;

	const int SCORE_FIRST_LEVEL_UP = 250;
	int scoreToLevelUp_ = SCORE_FIRST_LEVEL_UP;
	int currentLevelScore_ = 0;

	bool jackpotVisible_ = false;

	bool initEngineComps();
	bool initGame();

	void gameLoop();
	float runGameSimulations(float previousTicks);

	void updateSnake(float deltaTime, bool& inputProcessed);

	void pause();
	void resume();
	void restart();

	void processInput();
	void draw();
	
	void printFps();

	void freeSnake();
};