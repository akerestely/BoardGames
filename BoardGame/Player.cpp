#include "Player.h"

#include <algorithm>
#include <string>
#include <fstream>
#include <functional>

Player::Player(IState::Winner symbol) :
	m_symbol(symbol),
	m_policyDb("policy_"),
	m_randomEngine(m_randomDevice())
{
}

std::shared_ptr<IState> Player::TakeAction(const std::shared_ptr<IState> &crtState)
{
	crtState->GetPossibleNextStates(m_possibleNextStates);
	std::shared_ptr<IState> nextState;
	std::binomial_distribution<> binDist(1, m_exploreRate);
	if (binDist(m_randomEngine))
	{
		std::uniform_int_distribution<uint32_t> uniformDist(0, (uint32_t)m_possibleNextStates.size() - 1);
		nextState = m_possibleNextStates[uniformDist(m_randomEngine)];
	}
	else
	{
		if (1)	// +creative -optim
		{
			std::multimap<TEstimation, std::shared_ptr<IState>, std::greater<TEstimation>> sorted;
			for (const auto &state : m_possibleNextStates)
				sorted.insert(std::make_pair(getEstimation(state), state));

			auto bounds = sorted.equal_range(sorted.begin()->first);
			uint32_t count = (uint32_t)std::distance(bounds.first, bounds.second);
			std::uniform_int_distribution<uint32_t> uniformDist(0, count - 1);
			std::advance(bounds.first, uniformDist(m_randomEngine));
			nextState = bounds.first->second;
		}
		else	// -creative +optim
		{
			auto itVec = std::max_element(m_possibleNextStates.begin(), m_possibleNextStates.end(),
				[&](const std::shared_ptr<IState> &state1, const std::shared_ptr<IState> &state2) {
				return getEstimation(state1) < getEstimation(state2);
			});
			nextState = *itVec;
		}
	}

	// clear work vector
	m_possibleNextStates.clear();

	if (!nextState->IsTerminal())
	{
		if (m_prevState)
			updateEstimation(m_prevState, nextState);
		// save chosen state
		m_prevState = nextState;
	}

	return nextState;
}

void Player::FeedReward(IState::Winner winner)
{
	if (!m_bCollectFeedback || !m_prevState)
		return;

	float target = 0.0f;
	if (winner == m_symbol)
	{
		target = 1.0f;
		++m_nGamesWon;
	}
	else if (winner == IState::Winner::None)
	{
		target = 0.5f;
	}
	else
	{
		++m_nGamesLost;
	}

	updateEstimation(m_prevState, std::shared_ptr<IState>(), target);

	m_prevState.reset();

	++m_nGamesPlayed;
}

void Player::SetLearningRate(float learningRate)
{
	m_stepSize = learningRate;
}

void Player::SetExploreRate(float exploreRate)
{
	m_exploreRate = exploreRate;
}

void Player::SetPolicy(const std::string &fileName)
{
	m_policyDb = fileName;
}

void Player::SetCollectFeedback(bool value)
{
	m_bCollectFeedback = value;
}

void Player::SavePolicy()
{
	if (m_nGamesPlayed == 0)
		return;

	std::ofstream out(m_policyDb + std::to_string(int(m_symbol)));

	out << m_nGamesPlayed << ' ' << m_nGamesWon << ' ' << m_nGamesLost << '\n';

	for (auto &pair : m_estimations)
		if (pair.second != 0.0f && pair.second != 1.0f && pair.second != 0.5f)
			out << pair.first << ' ' << pair.second << std::endl;
}

void Player::LoadPolicy()
{
	std::ifstream in(m_policyDb + std::to_string(int(m_symbol)));
	if (in.fail())
		return;

	in >> m_nGamesPlayed >> m_nGamesWon >> m_nGamesLost;

	while (!in.eof())
	{
		IState::THash hash;
		float estimation;
		in >> hash >> estimation;
		m_estimations[hash] = estimation;
	}
}

Player::TEstimation& Player::getEstimation(const std::shared_ptr<IState> &state)
{
	auto itMap = m_estimations.insert(std::make_pair(state->GetHash(), 0.f));
	TEstimation &estimation = itMap.first->second;
	if (itMap.second == true)
	{
		// newly inserted; initialize
		estimation = getInitialValue(state);
	}

	return estimation;
}

float Player::getInitialValue(const std::shared_ptr<IState> &state)
{
	if (state->IsTerminal())
	{
		if (state->GetWinner() == m_symbol)
			return 1.0f;
		else
			return 0.0f;
	}
	else
		return  0.5f;
}

void Player::updateEstimation(const std::shared_ptr<IState> &state, const std::shared_ptr<IState> &nextState, float reward /*= 0*/)
{
	float &estimation = getEstimation(state);
	float nextStateEstimation = 0;
	if (nextState)
		// it's a non terminal state
		nextStateEstimation = getEstimation(nextState);

	estimation = estimation + m_stepSize * (reward + nextStateEstimation - estimation);
}
