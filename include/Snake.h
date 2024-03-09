#pragma once

#include <Evolve/Window.h>
#include <Evolve/ColorRgba.h>
#include <Evolve/InputProcessor.h>

enum class GameState {
	MAIN_MENU, PLAY, PAUSE, EXIT
};

class Snake {
public:
	Snake();
	~Snake();

	bool init();
	void run();

private:
	const Evolve::ColorRgba CLEAR_COLOR { 230, 230, 230, 255 };

	Evolve::Window m_window;

	GameState m_gameState = GameState::PLAY;
	Evolve::InputProcessor m_inputProcessor;
	
	bool initGame();
	bool initEngineComps();

	void gameLoop();
	void processInput();
	void draw();

	void freeTetris();
};