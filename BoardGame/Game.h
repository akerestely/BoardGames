#pragma once
#include "Engine/IGame.h"
#include "Engine/Camera2D.h"
#include "Engine/Rectangle.h"

#include "Judger.h"

struct IRenderable;
struct IPlayer;

class Game : public Engine::IGame
{
public:
	Game();

private:
	virtual void onInit() override final;
	virtual void onUpdate() override final;
	virtual void onRender() override final;
	virtual void onDestroy() override final;

	virtual void onInitRendering() {};
	virtual std::shared_ptr<IRenderable> getBoard() = 0;
	virtual std::shared_ptr<IPlayer> getPlayer(IState::Winner type) = 0;
	virtual std::shared_ptr<IRenderable> getChessman(int type) = 0;
	virtual void onRoundEnded() {};

	void processInput();

protected:
	Judger m_judger;


private:
	Engine::Camera2D m_camera;
	bool m_bUpdate = true;

	struct BoardTile
	{
		rectf boundingBox;
		std::shared_ptr<IRenderable> chessman;
		Position boardIndexPos;
	};
	BoardTile m_boardTiles[9];

	uint m_lastTurnTime = 0;	//ms
	uint m_delayNextTurn = 0;	//ms
};

