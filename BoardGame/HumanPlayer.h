#pragma once
#include "Player.h"
#include "Board.h"
#include "../TicTacToe/TicTacToeState.h"

class HumanPlayer : public Player
{
public:
	HumanPlayer(IState::Winner symbol) : Player(symbol)
	{
	}

	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState) override
	{
		if (!bufferedAction)
			return std::shared_ptr<IState>();

		auto nextState = static_cast<TicTacToeState*>(crtState.get())->GetNextState(*bufferedAction);
		bufferedAction.reset();

		return nextState;
	}

	virtual void FeedReward(const std::shared_ptr<IState> &state) override
	{

	}

	void BufferAction(const Position &clickedTile)
	{
		bufferedAction = std::make_shared<Position>(clickedTile);
	}


private:
	std::shared_ptr<Position> bufferedAction;
};

