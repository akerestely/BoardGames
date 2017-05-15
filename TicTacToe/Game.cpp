#include "Game.h"
#include "Engine\Engine.h"
#include "Engine\Logger.h"
#include "Engine\GLSLProgram.h"

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

	board.Init(simpleProgram);
	cross.Init(simpleProgram);
	nought.Init(simpleProgram);

	fpsLimiter.Init(maxFps);
}

void Game::initShaders()
{
	simpleProgram = std::make_shared<Engine::GLSLProgram>();
	simpleProgram->CompileShaders("Shaders/simpleShading.vert", "Shaders/simpleShading.frag");
	simpleProgram->AddAttribute("vertexPosition");
	simpleProgram->LinkShader();
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
			if (evnt.key.keysym.sym == SDLK_ESCAPE)
				gameState = GameState::Exit;
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
		Engine::log("At scale %.2fx coords are: %.2f, %.2f", camera.GetScale(), mouseCoords.x, mouseCoords.y);
	}
}

void Game::renderScene()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//actual drawing here
	board.Render(camera);
	cross.Render(camera, glm::vec2(-65,0));
	nought.Render(camera, glm::vec2(0,0));

	window.SwappBuffer();
}
