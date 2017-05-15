#pragma once
#include "Engine\Window.h"
#include "Engine\Camera2D.h"
#include "Engine\InputManager.h"
#include "Engine\Timing.h"
#include "Board.h"

enum class GameState 
{ 
	Play, 
	Exit 
};

class Game
{
public:
	Game();
	void Run();
	~Game();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void renderScene();

private:
	Engine::Window window;
	int screenWidth, screenHeight;
	GameState gameState;

	Engine::Camera2D camera;

	Engine::InputManager inputManager;
	Engine::FpsLimiter fpsLimiter;

	uint maxFps;
	float fps;

	Board board;
};

