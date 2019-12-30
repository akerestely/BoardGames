#include "Judger.h"

Judger::Judger(bool checkRepeatingRounds /*= true*/) :
	m_checkRepeatingRounds(checkRepeatingRounds)
{

}

void Judger::InitGame(std::shared_ptr<IPlayer> player1, std::shared_ptr<IPlayer> player2, std::shared_ptr<IState> startingState)
{
	m_crtPlayer = player1;
	m_nextPlayer = player2;

	m_currentState = startingState;

	m_gameEnded = false;
	m_winner = IState::Winner::None;

	m_lastStatesHashes.clear();
}

bool Judger::PlayTurn()
{
	auto &nextState = m_crtPlayer->TakeAction(m_currentState);
	if (!nextState)
		return false;

	if (m_currentState->GetNextPlayer() != nextState->GetNextPlayer())
		m_crtPlayer.swap(m_nextPlayer);

	m_currentState = nextState;

	// save state hash
	m_lastStatesHashes.push_front(nextState->GetHash());

	// check winning condition
	if (nextState->IsTerminal())
	{
		m_winner = nextState->GetWinner();
		m_gameEnded = true;
		return true;
	}

	// check draw condition
	if (m_checkRepeatingRounds && areLastRoundsRepeating())
	{
		m_winner = IState::Winner::None;
		m_gameEnded = true;
		return true;
	}

	return true;
}

bool Judger::HasGameEnded() const
{
	return m_gameEnded;
}

IState::Winner Judger::GetWinner() const
{
	return m_winner;
}

const std::shared_ptr<IPlayer>& Judger::GetCurrentPlayer() const
{
	return m_crtPlayer;
}

const std::shared_ptr<IState>& Judger::GetCurrentState() const
{
	return m_currentState;
}

bool Judger::areLastRoundsRepeating()
{
	const uint32_t kMinRequiredLastStates = 9;
	const uint32_t kFirstStageStates = 6;
	if (m_lastStatesHashes.size() >= kFirstStageStates + kMinRequiredLastStates)
	{
		const uint32_t step = 4;
		uint32_t statesCount = (uint32_t)m_lastStatesHashes.size() - kFirstStageStates;
		uint32_t appearanceCount = 1;
		for (uint32_t i = step; i < statesCount; i += step)
		{
			if (m_lastStatesHashes[i] == m_lastStatesHashes.front())
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
