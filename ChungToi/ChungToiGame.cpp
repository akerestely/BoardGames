#include "ChungToiGame.h"
#include "ChungToiBoard.h"

#include "Engine\GLSLProgram.h"
#include "BoardGame\Player.h"

#include "ChungToiState.h"
#include "ChungToiBoardConfiguration.h"
#include "HumanPlayer.h"
#include "Piece.h"
#include <string>

ChungToiGame::ChungToiGame()
{
	m_windowTitle = "Chung Toi";
}

void ChungToiGame::onInitRendering()
{
	m_board = std::make_shared<ChungToiBoard>();
	m_board->Init(m_simpleProgramPtr);

	for (auto &piece : m_pieces)
	{
		piece = std::make_shared<Piece>();
		piece->Init(m_simpleProgramPtr);
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
	if (m_settings["player1"] == "RL")
	{
		if (m_settings.find("RL1_explore_rate") != m_settings.end())
			m_player1->SetExploreRate(std::stof(m_settings["RL1_explore_rate"]));
		if (m_settings.find("RL1_learning_rate") != m_settings.end())
			m_player1->SetLearningRate(std::stof(m_settings["RL1_learning_rate"]));
		if (m_settings.find("RL1_policy_db") != m_settings.end())
			m_player1->SetPolicy(m_settings["RL1_policy_db"]);
		if (m_settings.find("RL1_collect_feedback") != m_settings.end())
			m_player1->SetCollectFeedback(m_settings["RL1_collect_feedback"] == "true");

		m_player1->LoadPolicy();
	}

	m_player2 = std::make_shared<Player>(IState::Winner::SecondPlayer);
	if (m_settings["player2"] == "RL")
	{
		if (m_settings.find("RL2_explore_rate") != m_settings.end())
			m_player2->SetExploreRate(std::stof(m_settings["RL2_explore_rate"]));
		if (m_settings.find("RL2_learning_rate") != m_settings.end())
			m_player2->SetLearningRate(std::stof(m_settings["RL2_learning_rate"]));
		if (m_settings.find("RL2_policy_db") != m_settings.end())
			m_player2->SetPolicy(m_settings["RL2_policy_db"]);
		if (m_settings.find("RL2_collect_feedback") != m_settings.end())
			m_player2->SetCollectFeedback(m_settings["RL2_collect_feedback"] == "true");

		m_player2->LoadPolicy();
	}

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
	case IState::Winner::FirstPlayer:
		if (m_settings["player1"] != "RL")
			return m_humanPlayer1;
		else
			return m_player1;
	case IState::Winner::SecondPlayer:
		if (m_settings["player2"] != "RL")
			return m_humanPlayer2;
		else
			return m_player2;

	default:
		return std::shared_ptr<IPlayer>();
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