#pragma once
#include "BoardGame/Game.h"

class Player;
class AbstractHumanPlayer;
class ChungToiBoard;

namespace Engine
{
	class GLSLProgram;
}

class ChungToiGame : public Game
{
public:
	ChungToiGame();

private:
	virtual void onInitRendering() override;
	virtual std::shared_ptr<IRenderable> getBoard() override;
	virtual std::shared_ptr<IPlayer> getPlayer(IState::Winner type) override;
	virtual std::shared_ptr<IState> getStartingState() override;
	virtual std::shared_ptr<IBoardConfiguration> getBoardConfiguration() override;
	virtual void onRoundEnded(const Judger &judger) override;

	void initShaders();

private:
	std::shared_ptr<Player> m_player1;
	std::shared_ptr<Player> m_player2;
	std::shared_ptr<AbstractHumanPlayer> m_humanPlayer;

	std::shared_ptr<ChungToiBoard> m_board;
	std::shared_ptr<Engine::GLSLProgram> m_simpleProgram;
};