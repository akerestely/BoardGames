#include "MillGame.h"

#include "Engine\GLSLProgram.h"
#include "BoardGame\Player.h"

#include "MillBoard.h"
#include "MillState.h"
#include "MillBoardConfiguration.h"
#include "HumanPlayer.h"
#include "Piece.h"
#include <string>

MillGame::MillGame()
{
	m_windowTitle = "Mill";
	m_screenSize = { 1024, 768 };
	m_scoreHeight = 0.55;
}

void MillGame::onInitRendering()
{
	m_board = std::make_shared<MillBoard>();
	m_board->Init(m_simpleProgramPtr);

	m_white = std::make_shared<Piece>();
	m_white->Init(m_simpleProgramPtr);
	m_white->SetColor(Piece::Color::White);
	m_red = std::make_shared<Piece>();
	m_red->Init(m_simpleProgramPtr);
	m_red->SetColor(Piece::Color::Red);

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

std::shared_ptr<IRenderable> MillGame::getBoard()
{
	return m_board;
}

std::shared_ptr<IPlayer> MillGame::getPlayer(IState::Winner type)
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
}

void MillGame::onDestroy()
{
	m_player1->SavePolicy();
	m_player2->SavePolicy();
}
