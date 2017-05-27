#include "ChungToiGame.h"
#include "ChungToiBoard.h"
#include "BoardGame\Player.h"

ChungToiGame::ChungToiGame()
{
	m_windowTitle = "ChungToiGame";
}

void ChungToiGame::onInitRendering()
{
	initShaders();

	m_board = std::make_shared<ChungToiBoard>();
	m_board->Init(m_simpleProgram);

	m_player1 = std::make_shared<Player>(IState::Winner::FirstPlayer);
	m_player1->LoadPolicy();
	m_player2 = std::make_shared<Player>(IState::Winner::SecondPlayer);
	m_player2->LoadPolicy();
}

std::shared_ptr<IRenderable> ChungToiGame::getBoard()
{
	return m_board;
}

std::shared_ptr<IPlayer> ChungToiGame::getPlayer(IState::Winner type)
{
	switch (type)
	{
	case IState::Winner::FirstPlayer:	return m_player1;
	case IState::Winner::SecondPlayer:	return m_player2;

	default:							return std::shared_ptr<IPlayer>();
	}
}

std::shared_ptr<IState> ChungToiGame::getStartingState()
{
	return std::shared_ptr<IState>();
}

std::shared_ptr<IBoardConfiguration> ChungToiGame::getBoardConfiguration()
{
	return std::shared_ptr<IBoardConfiguration>();
}

void ChungToiGame::onRoundEnded(const Judger &judger)
{

}

void ChungToiGame::initShaders()
{

}
