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
		mainFont_.initFromFontFile("Sunshine", "resources/fonts/Mr Sunshine 2.ttf", 32) &&
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

	guiFont_main_ = gui_.addFont(mainFont_);

	gui_scoreText_ = gui_.addPlainText(
		"",
		guiFont_main_,
		1.0f,
		Evolve::ColorRgba{ 0, 0, 0, 255 },
		glm::ivec2(20, windowHeight_ - 10)
		);

	std::string escText = "Pres ESC to pause";

	glm::ivec2 escPos { windowWidth_ - mainFont_.getLineWidth(escText) - 20, windowHeight_ - 10 };

	gui_escText_ = gui_.addPlainText(
		escText,
		guiFont_main_,
		1.0f,
		Evolve::ColorRgba{ 0, 0, 0, 255 },
		escPos
	);

	return true;
}

void MainGame::gameLoop() {

	float previousTicks = (float) SDL_GetTicks();

	while (gameState_ != GameState::EXIT) {

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

	static int pointForFruit = 10;
	static int pointForJackpot = 50;

	static int perLevelFruitPointIncrease = 1;
	static int perLevelJackpotPointIncrease = 10;

	static int numFruitsForJackpotSpawn = 10;

	static int nextLevelUp = 250;
	static int levelUpIncreasePerLevel = 50;
	static int currentLevelScore = 0;

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

			score_ += pointForFruit + (perLevelFruitPointIncrease * (level_ - 1));
			currentLevelScore += pointForFruit + (perLevelFruitPointIncrease * (level_ - 1));

			fruit_.reset();
			fruitsConsumed_++;

			if (fruitsConsumed_ % numFruitsForJackpotSpawn == 0) {
				jackpot_.startJackpot(level_);
				jackpotVisible_ = true;
			}
		}

		if (jackpot_.isConsumed()) {
			score_ += pointForJackpot + (perLevelJackpotPointIncrease * (level_ - 1));
			currentLevelScore += pointForJackpot + (perLevelJackpotPointIncrease * (level_ - 1));

			jackpot_.reset();

			jackpotsConsumed_++;
			jackpotVisible_ = false;
		}
		else if (jackpot_.isLost()) {
			jackpotVisible_ = false;

			jackpot_.reset();
		}

		// level up
		if (currentLevelScore >= nextLevelUp) {
			level_++;
			currentLevelScore = 0;
			nextLevelUp += levelUpIncreasePerLevel;
		}
	}
}

void MainGame::updateScoreAndLevel() {}

void MainGame::processInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			gameState_ = GameState::EXIT;
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
			gameState_ = GameState::PAUSE;
		}
		else if (gameState_ == GameState::PAUSE) {
			gameState_ = GameState::PLAY;
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

	std::string scoreText = "Level: " + std::to_string(level_) + " (MAX 10)\n" +
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