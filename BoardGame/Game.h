#pragma once
#include "Engine/IGame.h"
#include "Engine/Camera2D.h"
#include "Engine/Rectangle.h"

#include "Board.h"
#include "Judger.h"

struct IRenderable;
struct IPlayer;
struct IState;
class IBoardConfiguration;

class Game : public Engine::IGame
{
public:
	Game();

protected:
	virtual void onKeyDown(void *pkey);
	virtual void onKeyUp(void *pkey);
	virtual void onMouseMove(void *pVec2i);

private:
	virtual void onInit() override final;
	virtual void onUpdate() override final;
	virtual void onRender() override final;
	virtual void onDestroy() override;

	virtual void onInitRendering() {};
	virtual std::shared_ptr<IRenderable> getBoard() = 0;
	virtual std::shared_ptr<IPlayer> getPlayer(IState::Winner type) = 0;
	virtual std::shared_ptr<IState> getStartingState() = 0;
	virtual std::shared_ptr<IBoardConfiguration> getBoardConfiguration() = 0;
	virtual void onTurnBegining(std::shared_ptr<IPlayer> crtPlayer, std::shared_ptr<IState> crtState) {};
	virtual void onTurnEnding(std::shared_ptr<IPlayer> crtPlayer, std::shared_ptr<IState> crtState) {};
	virtual void onRoundEnded(const Judger &judger) {};

	void processInput();

private:
	Engine::Camera2D m_camera;
	bool m_bUpdate = true;

	Judger m_judger;

	std::shared_ptr<IBoardConfiguration> m_boardConfig;

	uint m_lastTurnTime = 0;	//ms
	uint m_delayNextTurn = 0;	//ms

	Position m_clickedTilePosIndex;
};

