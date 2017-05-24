#include "TicTacToeGame.h"

#include "Engine/GLSLProgram.h"
#include "BoardGame/Player.h"
#include "BoardGame/HumanPlayer.h"

#include "Cross.h"
#include "Nought.h"
#include "TicTacToeState.h"
#include "TicTacToeBoard.h"

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
	m_humanPlayer = std::make_shared<HumanPlayer>();
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

std::shared_ptr<IRenderable> TicTacToeGame::getChessman(int type)
{
	switch (TicTacToeChessmans(type))
	{
	case TicTacToeChessmans::Cross:		return m_cross;
	case TicTacToeChessmans::Nought:	return m_nought;

	default:							return std::shared_ptr<IRenderable>();
	}
}

void TicTacToeGame::onRoundEnded()
{
	m_player1->FeedReward(m_judger.GetWinner());
	m_player2->FeedReward(m_judger.GetWinner());
}

void TicTacToeGame::initShaders()
{
	m_simpleProgram = std::make_shared<Engine::GLSLProgram>();
	m_simpleProgram->CompileShaders("Shaders/simpleShading.vert", "Shaders/simpleShading.frag");
	m_simpleProgram->AddAttribute("vertexPosition");
	m_simpleProgram->LinkShader();
}
