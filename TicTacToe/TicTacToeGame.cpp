#include "TicTacToeGame.h"

#include "Engine/GLSLProgram.h"
#include "BoardGame/Player.h"
#include "BoardGame/HumanPlayer.h"

#include "Cross.h"
#include "Nought.h"
#include "TicTacToeState.h"
#include "TicTacToeBoard.h"
#include "TicTacToeBoardConfiguration.h"

TicTacToeGame::TicTacToeGame()
{
	m_windowTitle = "TicTacToe";
}

void TicTacToeGame::onInitRendering()
{
	initShaders();

	m_cross = std::make_shared<Cross>();
	m_cross->Init(m_simpleProgram);
	m_nought = std::make_shared<Nought>();
	m_nought->Init(m_simpleProgram);
	m_board = std::make_shared<TicTacToeBoard>();
	m_board->Init(m_simpleProgram);

	m_player1 = std::make_shared<Player>(IState::Winner::FirstPlayer);
	m_player1->LoadPolicy();
	m_player2 = std::make_shared<Player>(IState::Winner::SecondPlayer);
	m_player2->LoadPolicy();
	m_humanPlayer = std::make_shared<HumanPlayer>(IState::Winner::SecondPlayer);
}

std::shared_ptr<IRenderable> TicTacToeGame::getBoard()
{
	return m_board;
}

std::shared_ptr<IPlayer> TicTacToeGame::getPlayer(IState::Winner type)
{
	switch (type)
	{
	case IState::Winner::FirstPlayer:	return m_player1;
	case IState::Winner::SecondPlayer:	return m_humanPlayer;

	default:							return std::shared_ptr<IPlayer>();
	}
}

std::shared_ptr<IState> TicTacToeGame::getStartingState()
{
	return std::make_shared<TicTacToeState>();
}

std::shared_ptr<IBoardConfiguration> TicTacToeGame::getBoardConfiguration()
{
	return std::make_shared<TicTacToeBoardConfig>(m_cross, m_nought);
}

void TicTacToeGame::onRoundEnded(const Judger &judger)
{
	m_player1->FeedReward(judger.GetWinner());
	m_player2->FeedReward(judger.GetWinner());
}

void TicTacToeGame::initShaders()
{
	m_simpleProgram = std::make_shared<Engine::GLSLProgram>();
	m_simpleProgram->CompileShaders("Shaders/simpleShading.vert", "Shaders/simpleShading.frag");
	m_simpleProgram->AddAttribute("vertexPosition");
	m_simpleProgram->LinkShader();
}
