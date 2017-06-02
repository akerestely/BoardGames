#pragma once
#include "IState.h"
#include "IPlayer.h"
#include <map>
#include <random>

class Player : public IPlayer
{
public:
	Player(IState::Winner symbol);

	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState) override;

	virtual void FeedReward(IState::Winner winner);

	void SavePolicy(std::string fileName = "optimal_policy_");

	void LoadPolicy(std::string fileName = "optimal_policy_");

private:
	using TEstimation = float;
	using TEstimations = std::map<IState::THash, TEstimation>;

private:
	TEstimation& getEstimation(const std::shared_ptr<IState> &state);

private:
	IState::Winner m_symbol;
	TEstimations m_estimations;

	std::vector<std::shared_ptr<IState>> m_lastStates;

	// RL parameters
	const float m_stepSize = 0.1f;		// influences the rate of learning
	const float m_exploreRate = 0.1f;

	std::random_device m_randomDevice;
	std::mt19937 m_randomEngine;

	// work vector
	std::vector<std::shared_ptr<IState>> m_possibleNextStates;
};

