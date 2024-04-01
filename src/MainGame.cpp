#include "../include/MainGame.h"

MainGame::MainGame() {}

MainGame::~MainGame() {
	freeSnake();
}

bool MainGame::init() {
	return initEngineComps() && initGame();
}

void MainGame::run() {
	gameLoop();
}

bool MainGame::initEngineComps() {
	return
		window_.init(true, 1720, 960, CLEAR_COLOR) &&
		camera_.init(window_.getWindowWidth(), window_.getWindowHeight()) &&
		fps_.init(MAX_FPS) &&
		shapeRenderer_.init("../Evolve-Engine/engine-assets") &&
		textureRenderer_.init("../Evolve-Engine/engine-assets") &&
		viniqueFont32_.initFromFontFile("Sunshine 32", "resources/fonts/vinque.rg-regular.otf", 32) &&
		viniqueFont128_.initFromFontFile("Sunshine 128", "resources/fonts/vinque.rg-regular.otf", 128) &&
		viniqueFont16_.initFromFontFile("Sunshine 16", "resources/fonts/vinque.rg-regular.otf", 20) &&
		gui_.init() &&
		guiRenderer_.init("../Evolve-Engine/engine-assets");
}

bool MainGame::initGame() {
	windowWidth_ = window_.getWindowWidth();
	windowHeight_ = window_.getWindowHeight();

	grid_.init(windowWidth_, windowHeight_);
	
	food_.init(&grid_);
	
	jackpot_.init(&grid_);
	
	snake_.init(&grid_, &food_, &jackpot_);

	initGuiComponents();

	return true;
}

void MainGame::initGuiComponents() {
	guiFont_vinique32_ = gui_.addFont(viniqueFont32_);
	guiFont_vinique128_ = gui_.addFont(viniqueFont128_);
	guiFont_vinique16_ = gui_.addFont(viniqueFont16_);

	Evolve::ColorRgba greyColor { 50, 50, 50, 255 };
	Evolve::ColorRgba bgColor { CLEAR_COLOR.red, CLEAR_COLOR.green, CLEAR_COLOR.blue, 255 };
	Evolve::ColorRgba transparentColor { 0, 0, 0, 0 };

	int topMargin = 10, horizontalMargin = 20;

	// score and level
	gui_scoreText_ = gui_.addPlainText(
		"",
		guiFont_vinique32_,
		1.0f,
		greyColor,
		glm::ivec2(horizontalMargin, windowHeight_ - topMargin)
	);

	gui_.hideComponent(gui_scoreText_);

	// restart and quit buttons
	std::string restartText = "Restart";
	std::string quitText = "Quit";

	Evolve::RectDimension quitButtonDims(Evolve::Origin::TOP_RIGHT,
		windowWidth_ - horizontalMargin, windowHeight_ - topMargin,
		viniqueFont32_.getLineWidth(quitText), viniqueFont32_.getLineHeight());

	gui_quitButton_ = gui_.addTextButton(
		quitText,
		guiFont_vinique32_,
		1.0f,
		greyColor,
		transparentColor,
		quitButtonDims,
		[&]() { gameState_ = GameState::QUIT; }
	);

	Evolve::RectDimension restartButtonDims(Evolve::Origin::TOP_RIGHT,
		quitButtonDims.getLeft() - horizontalMargin, quitButtonDims.getTop(),
		viniqueFont32_.getLineWidth(restartText), viniqueFont32_.getLineHeight());

	gui_restartButton_ = gui_.addTextButton(
		restartText,
		guiFont_vinique32_,
		1.0f,
		greyColor,
		transparentColor,
		restartButtonDims,
		[&]() { restart(); }
	);

	gui_.hideComponent(gui_restartButton_);

	// background panel
	gui_bgPanel_ = gui_.addPanel(
		Evolve::RectDimension(
			Evolve::Origin::BOTTOM_LEFT,
			0, 0, windowWidth_, windowHeight_
		),
		Evolve::ColorRgba{ CLEAR_COLOR.red, CLEAR_COLOR.green, CLEAR_COLOR.blue, 200 }
	);

	// start menu
	std::string snakeText = "SNAKE";

	glm::ivec2 snakePos{ windowWidth_ / 2 - viniqueFont128_.getLineWidth(snakeText) / 2, windowHeight_ / 5 * 4 };

	gui_snakeText_ = gui_.addPlainText(
		snakeText,
		guiFont_vinique128_,
		1.0f,
		greyColor,
		snakePos
	);

	Evolve::RectDimension startButtonDims(Evolve::Origin::CENTER, windowWidth_ / 2, snakePos.y - 350, 300, 64);

	gui_startButton_ = gui_.addTextButton(
		"Start",
		guiFont_vinique32_,
		1.0f,
		bgColor,
		greyColor,
		startButtonDims,
		[&]() {
			gui_.hideComponent(gui_snakeText_);
			gui_.hideComponent(gui_startButton_);
			gui_.hideComponent(gui_bgPanel_);

			gui_.showComponent(gui_scoreText_);
			gui_.showComponent(gui_quitButton_);
			gui_.showComponent(gui_restartButton_);

			gameState_ = GameState::PLAY;
		}
	);

	// pause
	std::string pauseText = "PAUSED";

	glm::ivec2 pauseTextPos { windowWidth_ / 2 - viniqueFont128_.getLineWidth(pauseText) / 2, windowHeight_ / 4 * 3 };

	gui_pauseText_ = gui_.addBlinkingText(
		pauseText,
		guiFont_vinique128_,
		1.0f,
		greyColor,
		pauseTextPos
	);

	gui_.hideComponent(gui_pauseText_);

	// moving texts at top
	movingTextStartingY_ = windowHeight_ + viniqueFont32_.getLineHeight();
	movintTextEndingY_ = windowHeight_ - topMargin;

	std::string gameOverText = "Game over!";

	gameOverTextPos_.x = windowWidth_ / 2 - viniqueFont32_.getLineWidth(gameOverText) / 2;
	gameOverTextPos_.y = movingTextStartingY_;

	gui_gameOverText_ = gui_.addPlainText(
		gameOverText,
		guiFont_vinique32_,
		1.0f,
		greyColor,
		gameOverTextPos_
	);

	gui_.hideComponent(gui_gameOverText_);

	std::string levelUpText = "Level up!";

	levelUpTextPos_.x = windowWidth_ / 2 - viniqueFont32_.getLineWidth(levelUpText) / 2;
	levelUpTextPos_.y = movingTextStartingY_;

	gui_levelUpText_ = gui_.addPlainText(
		levelUpText,
		guiFont_vinique32_,
		1.0f,
		greyColor,
		levelUpTextPos_
	);

	gui_.hideComponent(gui_levelUpText_);
}

void MainGame::gameLoop() {

	float previousTicks = (float) SDL_GetTicks();

	while (gameState_ != GameState::QUIT) {

		fps_.beginFrame();

		inputProcessor_.update();

		processInput();
		
		gui_.updateGui(inputProcessor_, camera_);

		previousTicks = runGameSimulations(previousTicks);

		draw();

		fps_.endFrame();

		printFps();
	}
}

float MainGame::runGameSimulations(float previousTicks) {
	static const float MS_PER_SECOND = 1000.0f;
	static const float DESIRED_Fps = 60.0f;

	static const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_Fps;

	static const int MAX_PHYSICS_SIMS = 6;
	static const float MAX_DELTA_TIME = 1.0f;

	float newTicks = (float)SDL_GetTicks();
	float frameTime = newTicks - previousTicks;

	float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

	bool inputProcessed = false;

	int i = 0;

	while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_SIMS) {
		float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

		if (gameState_ == GameState::PLAY) {
			updateSnake(deltaTime, inputProcessed);
			jackpot_.update(deltaTime);
		}
		
		if (levelingUp_) {
			updatelevelUpText(deltaTime);
		}

		if (gameOverUpdateNeeded_) {
			updateGameOverText(deltaTime);
		}
		
		gui_.updateTime(deltaTime);

		if (gameState_ == GameState::ENDED) {
			break;
		}

		totalDeltaTime -= deltaTime;
		i++;
	}

	return newTicks;
}

void MainGame::updateSnake(float deltaTime, bool& inputProcessed) {

	static const int POINT_FRUIT = 10;
	static const int POINT_JACKPOT = 50;

	static const int NUM_FRUIT_FOR_JACKPOT_SPAWN = 2;

	static const int ADD_LEVEL_UP_SCORE_PER_LEVEL = 50;
	

	if (!inputProcessed) {
		if (inputProcessor_.isKeyPressed(SDLK_RIGHT)) {
			snake_.changeDirection(SnakeDirection::RIGHT);
			inputProcessed = true;
		}
		else if (inputProcessor_.isKeyPressed(SDLK_LEFT)) {
			snake_.changeDirection(SnakeDirection::LEFT);
			inputProcessed = true;
		}
		else if (inputProcessor_.isKeyPressed(SDLK_UP)) {
			snake_.changeDirection(SnakeDirection::UP);
			inputProcessed = true;
		}
		else if (inputProcessor_.isKeyPressed(SDLK_DOWN)) {
			snake_.changeDirection(SnakeDirection::DOWN);
			inputProcessed = true;
		}
	}

	int nextMove = snake_.move(deltaTime, level_);

	if (nextMove == -1) {
		gameState_ = GameState::ENDED;
		gameOverUpdateNeeded_ = true;
	}
	else {
		score_ += nextMove;
		currentLevelScore_ += nextMove;

		if (food_.isConsumed()) {

			score_ += POINT_FRUIT;
			currentLevelScore_ += POINT_FRUIT;

			food_.reset();
			fruitsConsumed_++;

			if (fruitsConsumed_ % NUM_FRUIT_FOR_JACKPOT_SPAWN == 0) {
				jackpot_.reset();
				jackpot_.startJackpot(level_);
				jackpotVisible_ = true;
			}
		}

		if (jackpot_.isConsumed()) {
			score_ += POINT_JACKPOT;
			currentLevelScore_ += POINT_JACKPOT;

			jackpot_.reset();

			jackpotVisible_ = false;
		}
		else if (jackpot_.isLost()) {
			jackpotVisible_ = false;

			jackpot_.reset();
		}

		// level up
		if (currentLevelScore_ >= scoreToLevelUp_) {
			level_++;
			currentLevelScore_ = 0;
			scoreToLevelUp_ += ADD_LEVEL_UP_SCORE_PER_LEVEL;
			levelingUp_ = true;
		}
	}
}

void MainGame::updatelevelUpText(float deltaTime) {
	static bool goingDown = true;
	static bool atEndingPos = false;

	static float stayedAtEndingPos = 0.0f;
	static const float TO_STAY_AT_ENDING_POS = 120.0f;

	if (gameState_ != GameState::PAUSE) {

		if (goingDown) {

			if (!atEndingPos) {
				if (!gui_.isComponentVisible(gui_levelUpText_)) {
					gui_.showComponent(gui_levelUpText_);
				}

				if (levelUpTextPos_.y > movintTextEndingY_) {
					levelUpTextPos_.y -= (int) (2.0f * deltaTime);
					gui_.setComponentPosition(gui_levelUpText_, levelUpTextPos_);
				}
				else {
					atEndingPos = true;
				}
			}

			// at the ending position
			else {
				if (stayedAtEndingPos < TO_STAY_AT_ENDING_POS) {
					stayedAtEndingPos += deltaTime;
				}

				// now start goin up
				else {
					goingDown = false;
					stayedAtEndingPos = 0.0f;
					atEndingPos = false;
				}
			}
		}
		else {
			if (levelUpTextPos_.y < movingTextStartingY_) {
				levelUpTextPos_.y += (int) (2.0f * deltaTime);
				gui_.setComponentPosition(gui_levelUpText_, levelUpTextPos_);
			}
			else {
				gui_.hideComponent(gui_levelUpText_);
				goingDown = true;
				levelingUp_ = false;
			}
		}
	}
}

void MainGame::updateGameOverText(float deltaTime) {
	
	if (gameState_ == GameState::ENDED) {
		
		// hide level up if visible
		if (gui_.isComponentVisible(gui_levelUpText_)) {
			gui_.hideComponent(gui_levelUpText_);
		}

		if (!gui_.isComponentVisible(gui_gameOverText_)) {
			gui_.showComponent(gui_gameOverText_);
		}

		if (gameOverTextPos_.y > movintTextEndingY_) {
			gameOverTextPos_.y -= (int) (2.0f * deltaTime);
			gui_.setComponentPosition(gui_gameOverText_, gameOverTextPos_);
		}
		else {
			gameOverUpdateNeeded_ = false;
		}
	}
	else {
		if (gameOverTextPos_.y < movingTextStartingY_) {
			gameOverTextPos_.y += (int) (2.0f * deltaTime);
			gui_.setComponentPosition(gui_gameOverText_, gameOverTextPos_);
		}
		else {
			gui_.hideComponent(gui_gameOverText_);
			gameOverUpdateNeeded_ = false;
		}
	}
}

void MainGame::pause() {
	gameState_ = GameState::PAUSE;

	gui_.showComponent(gui_pauseText_);
}

void MainGame::resume() {
	gameState_ = GameState::PLAY;

	gui_.hideComponent(gui_pauseText_);
}

void MainGame::restart() {
	score_ = 0;
	fruitsConsumed_ = 0;
	level_ = 1;

	scoreToLevelUp_ = SCORE_FIRST_LEVEL_UP;
	currentLevelScore_ = 0;

	jackpotVisible_ = false;

	snake_.restart();
	food_.restart();
	jackpot_.restart();

	if (gameState_ == GameState::ENDED) {
		gameOverUpdateNeeded_ = true;
	}

	resume();
}


void MainGame::processInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			gameState_ = GameState::QUIT;
			break;

		case SDL_MOUSEMOTION:
			inputProcessor_.setMouseCoords(event.motion.x, event.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			inputProcessor_.pressKey(event.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			inputProcessor_.releaseKey(event.button.button);
			break;

		case SDL_KEYDOWN:
			inputProcessor_.pressKey(event.key.keysym.sym);
			break;

		case SDL_KEYUP:
			inputProcessor_.releaseKey(event.key.keysym.sym);
			break;
		}
	}

	if (inputProcessor_.isKeyPressed(SDLK_ESCAPE)) {
		if (gameState_ == GameState::PLAY) {
			pause();
		}
		else if (gameState_ == GameState::PAUSE) {
			resume();
		}
	}
}

void MainGame::draw() {
	window_.clearScreen(GL_COLOR_BUFFER_BIT);

	textureRenderer_.begin();

	food_.draw(textureRenderer_);

	if (jackpotVisible_) {
		jackpot_.draw(textureRenderer_);
	}

	textureRenderer_.end();

	textureRenderer_.renderTextures(camera_);

	shapeRenderer_.begin();

	snake_.draw(shapeRenderer_);

	if (jackpotVisible_) {
		jackpot_.drawTimer(shapeRenderer_);
	}

	shapeRenderer_.end();

	shapeRenderer_.renderShapes(camera_);

	std::string scoreText = "Level: " + std::to_string(level_) + "\n" +
		"Score: " + std::to_string(score_);

	gui_.setComponentLabel(gui_scoreText_, scoreText);

	guiRenderer_.renderGui(gui_, camera_);

	window_.swapBuffer();
}

void MainGame::printFps() {
	float currentFps = fps_.calculateFps();

	static int frameCount = 0;
	frameCount++;

	if (frameCount == 100) {
		printf("%f\n", currentFps);
		frameCount = 0;
	}
}

void MainGame::freeSnake() {
	shapeRenderer_.freeShapeRenderer();

	window_.deleteWindow();
}