#pragma once
#include "IPlayer.h"
#include "Board.h"
#include "../TicTacToe/TicTacToeState.h"

class HumanPlayer : public IPlayer
{
public:
	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState) override
	{
		if (!bufferedAction)
			return std::shared_ptr<IState>();

		auto nextState = static_cast<TicTacToeState*>(crtState.get())->GetNextState(*bufferedAction);
		bufferedAction.reset();

		return nextState;
	}

	void BufferAction(const Position &clickedTile)
	{
		bufferedAction = std::make_shared<Position>(clickedTile);
	}

private:
	std::shared_ptr<Position> bufferedAction;
};

