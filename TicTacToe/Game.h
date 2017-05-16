#pragma once
#include "Engine\Camera2D.h"
#include "Engine\InputManager.h"
#include "Engine\Rectangle.h"
#include "Engine\Timing.h"
#include "Engine\Window.h"

#include "Board.h"
#include "IRenderable.h"
#include "Cross.h"
#include "Nought.h"

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
	void resize(uint screenWidth, uint screenHeight);

private:
	Engine::Window window;
	uint screenWidth, screenHeight;
	GameState gameState;

	Engine::Camera2D camera;

	Engine::InputManager inputManager;
	Engine::FpsLimiter fpsLimiter;

	uint maxFps;
	float fps;

	std::shared_ptr<Engine::GLSLProgram> simpleProgram;
	Board board;

	std::shared_ptr<Cross> cross;
	std::shared_ptr<Nought> nought;
	struct BoardTile
	{
		rectf boundingBox;
		std::shared_ptr<IRenderable> chessman;
	};
	BoardTile boardTiles[9];
};

