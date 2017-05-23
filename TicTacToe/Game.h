#pragma once
#include "Engine\IGame.h"
#include "Engine\Camera2D.h"
#include "Engine\Rectangle.h"

#include "BoardRenderer.h"
#include "Judger.h"

struct IRenderable;
class Player;

class Game : public Engine::IGame
{
public:
	Game();

private:
	virtual void onInit() override;
	virtual void onUpdate() override;
	virtual void onRender() override;
	virtual void onDestroy() override;

	void initShaders();
	void processInput();

private:
	Engine::Camera2D m_camera;
	bool m_bUpdate = true;

	std::shared_ptr<Engine::GLSLProgram> m_simpleProgram;
	BoardRenderer m_boardRenderer;

	std::shared_ptr<IRenderable> m_cross;
	std::shared_ptr<IRenderable> m_nought;
	struct BoardTile
	{
		rectf boundingBox;
		std::shared_ptr<IRenderable> chessman;
		Position boardIndexPos;
	};
	BoardTile m_boardTiles[9];

	Judger m_judger;
	std::shared_ptr<Player> m_player1;
	std::shared_ptr<Player> m_player2;

	uint m_lastTurnTime = 0;	//ms
	uint m_delayNextTurn = 0;	//ms
};

