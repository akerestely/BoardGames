#include "MillGame.h"

#include "Engine\GLSLProgram.h"
#include "BoardGame\Player.h"

#include "MillBoard.h"
#include "MillState.h"
#include "MillBoardConfiguration.h"
#include "HumanPlayer.h"
#include "Piece.h"

MillGame::MillGame()
{
	m_windowTitle = "Mill";
}

void MillGame::onInitRendering()
{
	initShaders();

	m_board = std::make_shared<MillBoard>();
	m_board->Init(m_simpleProgram);

	m_white = std::make_shared<Piece>();
	m_white->Init(m_simpleProgram);
	m_white->SetColor(Piece::Color::White);
	m_red = std::make_shared<Piece>();
	m_red->Init(m_simpleProgram);
	m_red->SetColor(Piece::Color::Red);

	m_player1 = std::make_shared<Player>(IState::Winner::FirstPlayer);
	m_player1->LoadPolicy();
	m_player2 = std::make_shared<Player>(IState::Winner::SecondPlayer);
	m_player2->LoadPolicy();

	m_humanPlayer1 = std::make_shared<HumanPlayer>(IState::Winner::FirstPlayer);
	m_humanPlayer2 = std::make_shared<HumanPlayer>(IState::Winner::SecondPlayer);
}

std::shared_ptr<IRenderable> MillGame::getBoard()
{
	return m_board;
}

std::shared_ptr<IPlayer> MillGame::getPlayer(IState::Winner type)
{
	switch (type)
	{
	case IState::Winner::FirstPlayer:	return m_player1;
	case IState::Winner::SecondPlayer:	return m_humanPlayer2;

	default:							return std::shared_ptr<IPlayer>();
	}
}

std::shared_ptr<IState> MillGame::getStartingState()
{
	return std::make_shared<MillState>();
}

std::shared_ptr<IBoardConfiguration> MillGame::getBoardConfiguration()
{
	return std::make_shared<MillBoardConfiguration>(m_white, m_red);
}

void MillGame::onRoundEnded(const Judger &judger)
{
	m_player1->FeedReward(judger.GetWinner());
	m_player2->FeedReward(judger.GetWinner());

	// HACK: to prevent memory allocation issues on training,
	// flush the learned estimations once in a while
	static uint kRounds = 0;
	if (++kRounds % 5000 == 0)
	{
		m_player1->SavePolicy();
		m_player2->SavePolicy();
		m_player1 = std::make_shared<Player>(IState::Winner::FirstPlayer);
		m_player1->LoadPolicy();
		m_player2 = std::make_shared<Player>(IState::Winner::SecondPlayer);
		m_player2->LoadPolicy();
	}
}

void MillGame::onDestroy()
{
	m_player1->SavePolicy();
	m_player2->SavePolicy();
}

void MillGame::initShaders()
{
	m_simpleProgram = std::make_shared<Engine::GLSLProgram>();
	m_simpleProgram->CompileShaders("../Resources/Shaders/simpleShading.vert", "../Resources/Shaders/simpleShading.frag");
	m_simpleProgram->AddAttribute("vertexPosition");
	m_simpleProgram->LinkShader();
}
