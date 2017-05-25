#pragma once
#include "IPlayer.h"
#include "Board.h"
#include "IState.h"

struct Move;

class AbstractHumanPlayer : public IPlayer
{
public:

	AbstractHumanPlayer(IState::Winner symbol) :
		m_symbol(symbol)
	{

	}

	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState) override
	{
		if (!m_bufferedAction)
			return std::shared_ptr<IState>();

		TicTacToeChessmans chessman;
		if (m_symbol == IState::Winner::FirstPlayer)
			chessman = TicTacToeChessmans::Cross;
		else
			chessman = TicTacToeChessmans::Nought;

		auto nextState = static_cast<TicTacToeState*>(crtState.get())->GetNextState(*m_bufferedAction, chessman);
		m_bufferedAction.reset();

		return nextState;
	}

	void BufferAction(const Position &clickedTile)
	{
		m_bufferedAction = std::make_shared<Position>(clickedTile);
	}
	void BufferAction(const Position &clickedTile, const Position &releasedTile)
	{
		m_bufferedMove = std::make_shared<Move>(Move({ clickedTile, releasedTile }));
	}

private:
	const IState::Winner &m_symbol;
	std::shared_ptr<Position> m_bufferedAction;
	std::shared_ptr<Move> m_bufferedMove;
};

