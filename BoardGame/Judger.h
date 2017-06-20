#pragma once
#include <memory>
#include <deque>

#include "IPlayer.h"
#include "IState.h"

class Judger
{
public:
	Judger(bool checkRepeatingRounds = true);

	void InitGame(std::shared_ptr<IPlayer> player1, std::shared_ptr<IPlayer> player2, std::shared_ptr<IState> startingState);
	bool PlayTurn();

	bool HasGameEnded() const;
	IState::Winner GetWinner() const;
	const std::shared_ptr<IPlayer>& GetCurrentPlayer() const;
	const std::shared_ptr<IState>& GetCurrentState() const;

private:
	// check if 3 consecutive full rounds are the same
	bool areLastRoundsRepeating();

private:
	std::shared_ptr<IPlayer> m_crtPlayer;
	std::shared_ptr<IPlayer> m_nextPlayer;
	std::shared_ptr<IState> m_currentState;
	std::deque<IState::THash> m_lastStatesHashes;
	
	IState::Winner m_winner;
	bool m_gameEnded : 1;
	bool m_checkRepeatingRounds : 1;
};

