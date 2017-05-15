#include "Game.h"
#include "Engine\Engine.h"

Game::Game() :
	screenWidth(800),
	screenHeight(600),
	gameState(GameState::Play),
	maxFps(60)
{
	camera.Init(screenWidth, screenHeight);
}

void Game::Run()
{
	initSystems();

	gameLoop();
}

Game::~Game()
{
}

void Game::initSystems()
{
	Engine::Init();
	window.Create("TicTacToe", screenWidth, screenHeight, 0);

	initShaders();

	fpsLimiter.Init(maxFps);
}

void Game::initShaders()
{
}

void Game::gameLoop()
{
	while (gameState != GameState::Exit)
	{
		fpsLimiter.Begin();

		processInput();

		camera.Update();
		renderScene();

		fps = fpsLimiter.End();
	}
}

void Game::processInput()
{
	SDL_Event evnt;
	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.1f;
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			gameState = GameState::Exit;
			break;
		case SDL_KEYDOWN:
			inputManager.PressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.ReleaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.PressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.ReleaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			inputManager.SetMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		}
	}

	if (inputManager.IsKeyDown(SDLK_w))
		camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	if (inputManager.IsKeyDown(SDLK_s))
		camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	if (inputManager.IsKeyDown(SDLK_a))
		camera.SetPosition(camera.GetPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	if (inputManager.IsKeyDown(SDLK_d))
		camera.SetPosition(camera.GetPosition() + glm::vec2(CAMERA_SPEED, -0.0f));
	if (inputManager.IsKeyDown(SDLK_q))
		camera.SetScale(camera.GetScale() + SCALE_SPEED);
	if (inputManager.IsKeyDown(SDLK_e))
		camera.SetScale(camera.GetScale() - SCALE_SPEED);

	if (inputManager.IsKeyDown(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = inputManager.GetMouseCoords();
		mouseCoords = camera.ConvertScreenToWorld(mouseCoords);
	}
}

void Game::renderScene()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//actual drawing here

	window.SwappBuffer();
}
