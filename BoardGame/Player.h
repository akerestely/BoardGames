#pragma once
#include "IState.h"
#include <memory>
#include <map>
#include <random>

// in cpp
#include "../TicTacToe/TicTacToeState.h"
#include <algorithm>
#include <string>
#include <fstream>
#include <xfunctional>

class Player
{
public:
	Player(IState::Winner symbol) :
		symbol(symbol),
		randEngine(randomDevice())
	{
	}

	virtual ~Player() = default;

	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState)
	{
		TicTacToeState* pCrtState = static_cast<TicTacToeState*>(crtState.get());
		pCrtState->GetPossibleNextStates(possibleNextStates);
		std::shared_ptr<TicTacToeState> nextState;
		std::binomial_distribution<> binDist(1, exploreRate);
		if (binDist(randEngine))
		{
			std::uniform_int_distribution<uint> uniformDist(0, possibleNextStates.size() - 1);
			nextState = possibleNextStates[uniformDist(randEngine)];
		}
		else
		{
			if (1)	// +creative -optim
			{
				std::multimap<TEstimation, std::shared_ptr<TicTacToeState>, std::greater<TEstimation>> sorted;
				for (const auto &state : possibleNextStates)
					sorted.insert(std::make_pair(getEstimation(state), state));

				auto bounds = sorted.equal_range(sorted.begin()->first);
				uint count = std::distance(bounds.first, bounds.second);
				std::uniform_int_distribution<uint> uniformDist(0, count - 1);
				std::advance(bounds.first, uniformDist(randEngine));
				nextState = bounds.first->second;
			}
			else	// -creative +optim
			{
				auto itVec = std::max_element(possibleNextStates.begin(), possibleNextStates.end(),
					[&](const std::shared_ptr<IState> &state1, const std::shared_ptr<IState> &state2) {
					return getEstimation(state1) < getEstimation(state2);
				});
				nextState = *itVec;
			}
		}

		// clear work vector
		possibleNextStates.clear();

		// save state
		lastStates.push_back(nextState);

		return nextState;
	}

	virtual void FeedReward(const std::shared_ptr<IState> &state)
	{
		if (lastStates.size() == 0)
			return;

		float target = 0.0f;
		if (state->GetWinner() == symbol)
			target = 1.0f;
		else if (state->GetWinner() == IState::Winner::None)
			target = 0.5f;

		for (auto it = lastStates.rbegin(); it != lastStates.rend(); ++it)
		{
			auto &state = *it;
			auto &estimation = estimations[state->GetHash()];
			auto value = estimation + stepSize * (target - estimation);
			estimation = value;
			target = value;
		}

		lastStates.clear();
	}

	void SavePolicy(std::string fileName = "optimal_policy_")
	{
		fileName += std::to_string(int(symbol));
		std::ofstream out(fileName);
		for (auto &pair : estimations)
			if(pair.second != 0.0f && pair.second != 1.0f && pair.second != 0.5f)
				out << pair.first << ' ' << pair.second << std::endl;
	}

	void LoadPolicy(std::string fileName = "optimal_policy_")
	{
		fileName += std::to_string(int(symbol));
		std::ifstream in(fileName);
		if (in.fail())
			return;

		while (!in.eof())
		{
			IState::THash hash;
			float estimation;
			in >> hash >> estimation;
			estimations[hash] = estimation;
		}
	}

private:
	using TEstimation = float;
	using TEstimations = std::map<IState::THash, TEstimation>;

private:
	TEstimation getEstimation(const std::shared_ptr<IState> &state)
	{
		auto &itMap = estimations.insert(std::make_pair(state->GetHash(), 0.f));
		TEstimation &estimation = itMap.first->second;
		if (itMap.second == true)
		{
			// newly inserted; initialize
			if (state->IsEnd())
			{
				if (state->GetWinner() == symbol)
					estimation = 1.0f;
				else
					estimation = 0.0f;
			}
			else
				estimation = 0.5f;
		}

		return estimation;
	}

private:
	IState::Winner symbol;
	TEstimations estimations;

	std::vector<std::shared_ptr<TicTacToeState>> lastStates;

	// RL parameters
	const float stepSize = 0.1f;
	const float exploreRate = 0.1f;

	std::random_device randomDevice;
	std::mt19937 randEngine;

	// work vector
	std::vector<std::shared_ptr<TicTacToeState>> possibleNextStates;
};

