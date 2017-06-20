#pragma once
#include "IPlayer.h"
#include "Board.h"
#include "IState.h"

struct Move;

class AbstractHumanPlayer : public IPlayer
{
public:
	AbstractHumanPlayer(IState::Winner symbol);

	void BufferAction(const Position &clickedTile);
	void BufferAction(const Position &clickedTile, const Position &releasedTile);

protected:
	const IState::Winner m_symbol;
	std::shared_ptr<Position> m_bufferedPositioning;
	std::shared_ptr<Move> m_bufferedMove;
};

