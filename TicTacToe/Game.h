#pragma once
#include "Engine\Camera2D.h"
#include "Engine\InputManager.h"
#include "Engine\Rectangle.h"
#include "Engine\Timing.h"
#include "Engine\Window.h"

#include "BoardRenderer.h"
#include "IRenderable.h"
#include "Cross.h"
#include "Nought.h"
#include "Judger.h"
#include "Player.h"

enum class GameState 
{ 
	Play,
	Pause,
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
	BoardRenderer boardRenderer;

	std::shared_ptr<Cross> cross;
	std::shared_ptr<Nought> nought;
	struct BoardTile
	{
		rectf boundingBox;
		std::shared_ptr<IRenderable> chessman;
		Position boardIndexPos;
	};
	BoardTile boardTiles[9];

	Judger judger;
	std::shared_ptr<Player> player1;
	std::shared_ptr<Player> player2;

	uint lastTurnTime = 0;
	uint delayNextTurn = 0; //ms
};

