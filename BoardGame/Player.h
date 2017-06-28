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

	void SetLearningRate(float learningRate);
	void SetExploreRate(float exploreRate);
	void SetPolicy(const std::string &fileName);
	void SetCollectFeedback(bool value);

	void SavePolicy();
	void LoadPolicy();

private:
	using TEstimation = float;
	using TEstimations = std::map<IState::THash, TEstimation>;

private:
	TEstimation& getEstimation(const std::shared_ptr<IState> &state);
	float getInitialValue(const std::shared_ptr<IState> &state);
	void updateEstimation(const std::shared_ptr<IState> &state, const std::shared_ptr<IState> &nextState, float reward = 0);

private:
	const IState::Winner m_symbol;
	TEstimations m_estimations;
	uint m_nGamesPlayed = 0;
	uint m_nGamesWon = 0;
	uint m_nGamesLost = 0;

	std::shared_ptr<IState> m_prevState;

	// RL parameters
	bool m_bCollectFeedback = true;
	float m_stepSize = 0.1f;		// influences the rate of learning
	float m_exploreRate = 0.1f;
	std::string m_policyDb;

	std::random_device m_randomDevice;
	std::mt19937 m_randomEngine;

	// work vector
	std::vector<std::shared_ptr<IState>> m_possibleNextStates;
};

