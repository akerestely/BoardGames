#pragma once
#include "BoardGame/Game.h"

class Player;
class HumanPlayer;
class TicTacToeBoard;

namespace Engine
{
	class GLSLProgram;
}

class TicTacToeGame : public Game
{
public:
	TicTacToeGame();

private:
	virtual void onInitRendering() override;
	virtual std::shared_ptr<IRenderable> getBoard() override;
	virtual std::shared_ptr<IPlayer> getPlayer(IState::Winner type) override;
	virtual std::shared_ptr<IRenderable> getChessman(int type) override;
	virtual void onRoundEnded(const Judger &judger) override;

	void initShaders();

private:
	std::shared_ptr<Player> m_player1;
	std::shared_ptr<Player> m_player2;
	std::shared_ptr<HumanPlayer> m_humanPlayer;

	std::shared_ptr<IRenderable> m_cross;
	std::shared_ptr<IRenderable> m_nought;
	std::shared_ptr<TicTacToeBoard> m_board;
	std::shared_ptr<Engine::GLSLProgram> m_simpleProgram;
};

