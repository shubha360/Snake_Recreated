#include "../include/Snake.h"

Snake::Snake() {}

Snake::~Snake() {
	freeTetris();
}

bool Snake::init() {
	return initEngineComps() && initGame();
}

void Snake::run() {
	gameLoop();
}

bool Snake::initGame() {
	return true;
}

bool Snake::initEngineComps() {
	return m_window.init(false, 1720, 980, CLEAR_COLOR);
}

void Snake::gameLoop() {
	while (m_gameState != GameState::EXIT) {
		processInput();
		draw();
	}
}

void Snake::processInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;

		case SDL_MOUSEMOTION:
			m_inputProcessor.setMouseCoords(event.motion.x, event.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_inputProcessor.pressKey(event.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			m_inputProcessor.releaseKey(event.button.button);
			break;

		case SDL_KEYDOWN:
			m_inputProcessor.pressKey(event.key.keysym.sym);
			break;

		case SDL_KEYUP:
			m_inputProcessor.releaseKey(event.key.keysym.sym);
			break;
		}
	}
}

void Snake::draw() {
	m_window.clearScreen(GL_COLOR_BUFFER_BIT);

	m_window.swapBuffer();
}

void Snake::freeTetris() {
	m_window.deleteWindow();
}