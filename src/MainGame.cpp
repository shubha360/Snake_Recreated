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
		window_.init(false, 1720, 960, CLEAR_COLOR) &&
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
	
	fruit_.init(&grid_);
	
	jackpot_.init(&grid_);
	
	snake_.init(&grid_, &fruit_, &jackpot_);

	guiFont_vinique32_ = gui_.addFont(viniqueFont32_);
	guiFont_vinique128_ = gui_.addFont(viniqueFont128_);
	guiFont_vinique16_ = gui_.addFont(viniqueFont16_);

	Evolve::ColorRgba blackColor { 0, 0, 0, 255 };
	Evolve::ColorRgba whiteColor { 230, 230, 230, 255 };

	gui_scoreText_ = gui_.addPlainText(
		"",
		guiFont_vinique32_,
		1.0f,
		blackColor,
		glm::ivec2(20, windowHeight_ - 10)
		);

	std::string escText = "ESC to pause";

	glm::ivec2 escPos { windowWidth_ - viniqueFont32_.getLineWidth(escText) - 20, windowHeight_ - 10 };

	gui_escText_ = gui_.addPlainText(
		escText,
		guiFont_vinique32_,
		1.0f,
		blackColor,
		escPos
	);

	gui_bgPanel_ = gui_.addPanel(
		Evolve::RectDimension(
			Evolve::Origin::BOTTOM_LEFT,
			0, 0, windowWidth_, windowHeight_
		),
		Evolve::ColorRgba{ 230, 230, 230, 200 }
	);

	gui_.hideComponent(gui_bgPanel_);

	std::string pauseText = "PAUSED";
	std::string resumeText = "Press ESC again to resume playing";

	glm::ivec2 pauseTextPos { windowWidth_ / 2 - viniqueFont128_.getLineWidth(pauseText) / 2,  windowHeight_ / 5 * 4 };

	gui_pauseText_ = gui_.addPlainText(
		pauseText,
		guiFont_vinique128_,
		1.0f,
		blackColor,
		pauseTextPos
		);

	gui_.hideComponent(gui_pauseText_);

	glm::ivec2 resumeTextPos { windowWidth_ / 2 - viniqueFont16_.getLineWidth(resumeText) / 2,  pauseTextPos.y - 180 };

	gui_resumeText_ = gui_.addPlainText(
		resumeText,
		guiFont_vinique16_,
		1.0f,
		blackColor,
		resumeTextPos
	);

	gui_.hideComponent(gui_resumeText_);

	int buttonWidth = 300;
	int buttonHeight = 64;

	Evolve::RectDimension restartButtonDims(Evolve::Origin::CENTER, windowWidth_ / 2, pauseTextPos.y - 350, buttonWidth, buttonHeight);

	gui_restartButton_ = gui_.addTextButton(
		"Restart",
		guiFont_vinique32_,
		1.0f,
		whiteColor,
		blackColor,
		restartButtonDims,
		[&]() { restart(); }
	);

	gui_.hideComponent(gui_restartButton_);

	Evolve::RectDimension quitButtonDims(Evolve::Origin::CENTER, windowWidth_ / 2, restartButtonDims.getBottom() - 50, buttonWidth, buttonHeight);

	gui_quitButton_ = gui_.addTextButton(
		"Quit",
		guiFont_vinique32_,
		1.0f,
		whiteColor,
		blackColor,
		quitButtonDims,
		[&]() { gameState_ = GameState::QUIT; }
	);

	gui_.hideComponent(gui_quitButton_);

	return true;
}

void MainGame::gameLoop() {

	float previousTicks = (float) SDL_GetTicks();

	while (gameState_ != GameState::QUIT) {

		fps_.beginFrame();

		inputProcessor_.update();

		processInput();
		
		gui_.updateGui(inputProcessor_, camera_);

		if (gameState_ == GameState::PLAY) {
			previousTicks = runGameSimulations(previousTicks);
		}

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

		updateSnake(deltaTime, inputProcessed);
		jackpot_.update(deltaTime);
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

	static const int ADD_FRUIT_POINT_PER_LEVEL = 1;
	static const int ADD_JACKPOT_POINT_PER_LEVEL = 10;

	static const int NUM_FRUIT_FOR_JACKPOT_SPAWN = 10;

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

	if (!snake_.move(deltaTime, level_)) {
		gameState_ = GameState::ENDED;
	}
	else {
		if (fruit_.isConsumed()) {

			score_ += POINT_FRUIT + (ADD_FRUIT_POINT_PER_LEVEL * (level_ - 1));
			currentLevelScore_ += POINT_FRUIT + (ADD_FRUIT_POINT_PER_LEVEL * (level_ - 1));

			fruit_.reset();
			fruitsConsumed_++;

			if (fruitsConsumed_ % NUM_FRUIT_FOR_JACKPOT_SPAWN == 0) {
				jackpot_.reset();
				jackpot_.startJackpot(level_);
				jackpotVisible_ = true;
			}
		}

		if (jackpot_.isConsumed()) {
			score_ += POINT_JACKPOT + (ADD_JACKPOT_POINT_PER_LEVEL * (level_ - 1));
			currentLevelScore_ += POINT_JACKPOT + (ADD_JACKPOT_POINT_PER_LEVEL * (level_ - 1));

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
		}
	}
}

void MainGame::pause() {
	gameState_ = GameState::PAUSE;

	gui_.showComponent(gui_bgPanel_);
	gui_.showComponent(gui_pauseText_);
	gui_.showComponent(gui_resumeText_);
	gui_.showComponent(gui_restartButton_);
	gui_.showComponent(gui_quitButton_);

	gui_.hideComponent(gui_escText_);
}

void MainGame::resume() {
	gameState_ = GameState::PLAY;

	gui_.hideComponent(gui_bgPanel_);
	gui_.hideComponent(gui_pauseText_);
	gui_.hideComponent(gui_resumeText_);
	gui_.hideComponent(gui_restartButton_);
	gui_.hideComponent(gui_quitButton_);

	gui_.showComponent(gui_escText_);
}

void MainGame::restart() {
	score_ = 0;
	fruitsConsumed_ = 0;
	level_ = 1;

	scoreToLevelUp_ = SCORE_FIRST_LEVEL_UP;
	currentLevelScore_ = 0;

	jackpotVisible_ = false;

	snake_.restart();
	fruit_.restart();
	jackpot_.restart();

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

	/*textureRenderer_.begin();

	grid_.printGrid(textureRenderer_);

	textureRenderer_.end();

	textureRenderer_.renderTextures(camera_);*/

	shapeRenderer_.begin();

	if (jackpotVisible_) {
		jackpot_.draw(shapeRenderer_);
	}

	fruit_.draw(shapeRenderer_);

	snake_.draw(shapeRenderer_);

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