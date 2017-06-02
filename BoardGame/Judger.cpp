#include "Engine/BaseTypes.h"
#include "Judger.h"

Judger::Judger(bool checkRepeatingRounds /*= true*/) :
	checkRepeatingRounds(checkRepeatingRounds)
{

}

void Judger::InitGame(std::shared_ptr<IPlayer> player1, std::shared_ptr<IPlayer> player2, std::shared_ptr<IState> startingState)
{
	crtPlayer = player1;
	nextPlayer = player2;

	currentState = startingState;

	gameEnded = false;
	winner = IState::Winner::None;

	lastStatesHashes.clear();
}

bool Judger::PlayTurn()
{
	auto &nextState = crtPlayer->TakeAction(currentState);
	if (!nextState)
		return false;

	if (currentState->GetNextPlayer() != nextState->GetNextPlayer())
		crtPlayer.swap(nextPlayer);

	currentState = nextState;

	// save state hash
	lastStatesHashes.push_front(nextState->GetHash());

	// check winning condition
	if (nextState->IsEnd())
	{
		winner = nextState->GetWinner();
		gameEnded = true;
		return true;
	}

	// check draw condition
	if (checkRepeatingRounds && areLastRoundsRepeating())
	{
		winner = IState::Winner::None;
		gameEnded = true;
		return true;
	}

	return true;
}

bool Judger::HasGameEnded() const
{
	return gameEnded;
}

IState::Winner Judger::GetWinner() const
{
	return winner;
}

const std::shared_ptr<IPlayer>& Judger::GetCrtPlayer() const
{
	return crtPlayer;
}

const std::shared_ptr<IState>& Judger::GetCrtState() const
{
	return currentState;
}

bool Judger::areLastRoundsRepeating()
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
