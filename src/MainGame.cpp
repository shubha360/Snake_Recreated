#include "../include/MainGame.h"

MainGame::MainGame() {}

MainGame::~MainGame() {
	freeTetris();
}

bool MainGame::init() {
	return initEngineComps() && initGame();
}

void MainGame::run() {
	gameLoop();
}

bool MainGame::initEngineComps() {
	return
		window_.init(false, 1720, 980, CLEAR_COLOR) &&
		camera_.init(window_.getWindowWidth(), window_.getWindowHeight()) &&
		fps_.init(MAX_FPS) &&
		shapeRenderer_.init("../Evolve-Engine/engine-assets");
}

bool MainGame::initGame() {
	windowWidth_ = window_.getWindowWidth();
	windowHeight_ = window_.getWindowHeight();

	return grid_.init(windowWidth_, windowHeight_) &&
		snake_.init(&grid_);
}

void MainGame::gameLoop() {

	float previousTicks = (float) SDL_GetTicks();

	while (gameState_ != GameState::EXIT) {

		fps_.beginFrame();

		inputProcessor_.update();

		processInput();

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

		updateSnake(deltaTime);
		
		totalDeltaTime -= deltaTime;
		i++;
	}

	return newTicks;
}

void MainGame::updateSnake(float deltaTime) {

	static const float autoMoveDelay = 15.0f;
	static float currentDelay = 0.0f;

	if (currentDelay >= autoMoveDelay) {
		snake_.move();
		currentDelay = 0.0f;
	}
	else {
		currentDelay += deltaTime;
	}


	if (inputProcessor_.isKeyPressed(SDLK_RIGHT)) {
		snake_.changeDirection(SnakeDirection::RIGHT);
	}
	else if (inputProcessor_.isKeyPressed(SDLK_LEFT)) {
		snake_.changeDirection(SnakeDirection::LEFT);
	}
	else if (inputProcessor_.isKeyPressed(SDLK_UP)) {
		snake_.changeDirection(SnakeDirection::UP);
	}
	else if (inputProcessor_.isKeyPressed(SDLK_DOWN)) {
		snake_.changeDirection(SnakeDirection::DOWN);
	}	
}

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
}

void MainGame::draw() {
	window_.clearScreen(GL_COLOR_BUFFER_BIT);

	shapeRenderer_.begin();

	grid_.printGrid(shapeRenderer_);

	shapeRenderer_.end();

	shapeRenderer_.renderShapes(camera_);

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

void MainGame::freeTetris() {
	shapeRenderer_.freeShapeRenderer();

	window_.deleteWindow();
}