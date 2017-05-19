#pragma once
#include <memory>
#include <deque>

#include "Player.h"
#include "IState.h"

// TODO: remove inculde
#include "TicTacToeState.h"

class Judger
{
public:
	Judger(bool checkRepeatingRounds = true, bool giveFeedback = true) : 
		checkRepeatingRounds(checkRepeatingRounds),
		giveFeedback(giveFeedback)
	{

	}

	void InitGame(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
	{
		crtPlayer = player1;
		nextPlayer = player2;

		currentState = std::make_shared<TicTacToeState>();

		gameEnded = false;
		winner = IState::Winner::None;

		lastStatesHashes.clear();
	}

	bool PlayTurn()
	{
		auto &nextState = crtPlayer->TakeAction(currentState);
		if (!nextState)
			return false;

		currentState = nextState;

		// save state hash
		lastStatesHashes.push_front(currentState->GetHash());

		// check winning condition
		if (currentState->IsEnd())
		{
			if (giveFeedback)
				GiveReward();
			
			winner = currentState->GetWinner();
			gameEnded = true;
			return true;
		}

		// check draw condition
		if (checkRepeatingRounds && areLastRoundsRepeating())
		{
			if (giveFeedback)
				GiveReward();

			winner = IState::Winner::None;
			gameEnded = true;
			return true;
		}

		crtPlayer.swap(nextPlayer);
		return true;
	}

	bool HasGameEnded() const { return gameEnded; }

	IState::Winner GetWinner() const
	{
		return winner;
	}

	const std::shared_ptr<Player>& GetCrtPlayer() const { return crtPlayer; }

	Board<TicTacToeChessmans> GetBoard() const { return static_cast<TicTacToeState*>(currentState.get())->GetBoard(); }

private:
	void GiveReward()
	{
		crtPlayer->FeedReward(currentState);
		nextPlayer->FeedReward(currentState);
	}

	// check if 3 consecutive full rounds are the same
	bool areLastRoundsRepeating()
	{
		const uint kMinRequiredLastStates = 9;
		const uint kFirstStageStates = 6;
		if (lastStatesHashes.size() >= kFirstStageStates + kMinRequiredLastStates)
		{
			const uint step = 4;
			uint statesCount = lastStatesHashes.size() - kFirstStageStates;
			uint appearanceCount = 1;
			for (uint i = step; i < statesCount; i += step)
			{
				if (lastStatesHashes[i] == lastStatesHashes.front())
				{
					if (++appearanceCount >= 3)
					{
						return true;
					}
				}
			}
		}
		
		return false;
	}

private:
	std::shared_ptr<Player> crtPlayer;
	std::shared_ptr<Player> nextPlayer;
	std::shared_ptr<IState> currentState;
	std::deque<IState::THash> lastStatesHashes;
	
	IState::Winner winner;
	bool gameEnded : 1;
	bool checkRepeatingRounds : 1;

	bool giveFeedback : 1;
};

