#pragma once
#include "Engine/IGame.h"
#include "Engine/Camera2D.h"
#include "Engine/Rectangle.h"

#include "Judger.h"

struct IRenderable;
struct IPlayer;
struct IState;
class IBoardConfiguration;

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
	virtual std::shared_ptr<IState> getStartingState() = 0;
	virtual std::shared_ptr<IBoardConfiguration> getBoardConfiguration() = 0;
	virtual void onRoundEnded(const Judger &judger) {};

	void processInput();

private:
	Engine::Camera2D m_camera;
	bool m_bUpdate = true;

	Judger m_judger;

	std::shared_ptr<IBoardConfiguration> m_boardConfig;

	uint m_lastTurnTime = 0;	//ms
	uint m_delayNextTurn = 0;	//ms
};

