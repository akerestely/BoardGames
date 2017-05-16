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
			if (SDL_GetRelativeMouseMode() == SDL_TRUE)
				inputManager.SetMouseCoordsRel(evnt.motion.xrel, evnt.motion.yrel);
			else
				inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_WINDOWEVENT:
			switch (evnt.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				resize((uint)evnt.window.data1, (uint)evnt.window.data2);
				break;
			}
			break;
		}
	}

	if (inputManager.IsKeyDown(SDLK_ESCAPE))
		gameState = GameState::Exit;
	if (inputManager.IsKeyDownOnce(SDLK_F4))
		window.Fullscreen(!window.IsFullscreen());
	if (inputManager.IsKeyDownOnce(SDLK_v))
		SDL_SetRelativeMouseMode(SDL_GetRelativeMouseMode() == SDL_TRUE ? SDL_FALSE : SDL_TRUE);

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

void Game::resize(uint screenWidth, uint screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	camera.Init(screenWidth, screenHeight);

	printf("Screen is now: %dx%d pixels\n", screenWidth, screenHeight);
}
