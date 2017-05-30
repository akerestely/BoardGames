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
	const std::shared_ptr<IPlayer>& GetCrtPlayer() const;
	const std::shared_ptr<IState>& GetCrtState() const;

private:
	// check if 3 consecutive full rounds are the same
	bool areLastRoundsRepeating();

private:
	std::shared_ptr<IPlayer> crtPlayer;
	std::shared_ptr<IPlayer> nextPlayer;
	std::shared_ptr<IState> currentState;
	std::deque<IState::THash> lastStatesHashes;
	
	IState::Winner winner;
	bool gameEnded : 1;
	bool checkRepeatingRounds : 1;
};

