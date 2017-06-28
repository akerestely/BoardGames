#include "TicTacToeGame.h"

#include "Engine/GLSLProgram.h"
#include "BoardGame/Player.h"

#include "HumanPlayer.h"
#include "Cross.h"
#include "Nought.h"
#include "TicTacToeState.h"
#include "TicTacToeBoard.h"
#include "TicTacToeBoardConfiguration.h"
#include <string>

TicTacToeGame::TicTacToeGame()
{
	m_windowTitle = "TicTacToe";
}

void TicTacToeGame::onInitRendering()
{
	m_cross = std::make_shared<Cross>();
	m_cross->Init(m_simpleProgramPtr);
	m_nought = std::make_shared<Nought>();
	m_nought->Init(m_simpleProgramPtr);
	m_board = std::make_shared<TicTacToeBoard>();
	m_board->Init(m_simpleProgramPtr);

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
}

std::shared_ptr<IRenderable> TicTacToeGame::getBoard()
{
	return m_board;
}

std::shared_ptr<IPlayer> TicTacToeGame::getPlayer(IState::Winner type)
{
	switch (type)
	{
	case IState::Winner::FirstPlayer:
		if (m_settings["player1"] != "RL")
			return std::make_shared<HumanPlayer>(IState::Winner::FirstPlayer);
		else
			return m_player1;
	case IState::Winner::SecondPlayer:
		if (m_settings["player2"] != "RL")
			return std::make_shared<HumanPlayer>(IState::Winner::SecondPlayer);
		else
			return m_player2;

	default:
		return std::shared_ptr<IPlayer>();
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

void TicTacToeGame::onDestroy()
{
	m_player1->SavePolicy();
	m_player2->SavePolicy();
}
