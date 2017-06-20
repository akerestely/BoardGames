#include "ChungToiGame.h"
#include "ChungToiBoard.h"

#include "Engine\GLSLProgram.h"
#include "BoardGame\Player.h"

#include "ChungToiState.h"
#include "ChungToiBoardConfiguration.h"
#include "HumanPlayer.h"
#include "Piece.h"

ChungToiGame::ChungToiGame()
{
	m_windowTitle = "ChungToiGame";
}

void ChungToiGame::onInitRendering()
{
	initShaders();

	m_board = std::make_shared<ChungToiBoard>();
	m_board->Init(m_simpleProgram);

	for (auto &piece : m_pieces)
	{
		piece = std::make_shared<Piece>();
		piece->Init(m_simpleProgram);
	}
	m_pieces[0]->SetColor(Piece::Color::White);
	m_pieces[0]->SetOrientation(Piece::Orientation::Cardinal);
	m_pieces[1]->SetColor(Piece::Color::White);
	m_pieces[1]->SetOrientation(Piece::Orientation::Diagonal);
	m_pieces[2]->SetColor(Piece::Color::Red);
	m_pieces[2]->SetOrientation(Piece::Orientation::Cardinal);
	m_pieces[3]->SetColor(Piece::Color::Red);
	m_pieces[3]->SetOrientation(Piece::Orientation::Diagonal);

	m_player1 = std::make_shared<Player>(IState::Winner::FirstPlayer);
	m_player1->LoadPolicy();
	m_player2 = std::make_shared<Player>(IState::Winner::SecondPlayer);
	m_player2->LoadPolicy();

	m_humanPlayer1 = std::make_shared<HumanPlayer>(IState::Winner::FirstPlayer);
	m_humanPlayer2 = std::make_shared<HumanPlayer>(IState::Winner::SecondPlayer);
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
	case IState::Winner::SecondPlayer:	return m_humanPlayer2;

	default:							return std::shared_ptr<IPlayer>();
	}
}

std::shared_ptr<IState> ChungToiGame::getStartingState()
{
	return std::make_shared<ChungToiState>();
}

std::shared_ptr<IBoardConfiguration> ChungToiGame::getBoardConfiguration()
{
	return std::make_shared<ChungToiBoardConfiguration>(m_pieces[0], m_pieces[1], m_pieces[2], m_pieces[3]);
}

void ChungToiGame::onRoundEnded(const Judger &judger)
{
	m_player1->FeedReward(judger.GetWinner());
	m_player2->FeedReward(judger.GetWinner());
}

void ChungToiGame::onDestroy()
{
	m_player1->SavePolicy();
	m_player2->SavePolicy();
}

void ChungToiGame::onKeyUp(void *pkey)
{
	Game::onKeyUp(pkey);

	Engine::Key key = *static_cast<Engine::Key*>(pkey);
	switch (key)
	{
	case Engine::Key::R:
		m_humanPlayer1->BufferRotation();
		m_humanPlayer2->BufferRotation();
	}
}

void ChungToiGame::initShaders()
{
	m_simpleProgram = std::make_shared<Engine::GLSLProgram>();
	m_simpleProgram->CompileShaders("Shaders/simpleShading.vert", "Shaders/simpleShading.frag");
	m_simpleProgram->AddAttribute("vertexPosition");
	m_simpleProgram->LinkShader();
}
