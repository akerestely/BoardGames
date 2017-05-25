#include "Engine/BaseTypes.h"
#include "AbstractHumanPlayer.h"
#include "State.h"

AbstractHumanPlayer::AbstractHumanPlayer(IState::Winner symbol) :
	m_symbol(symbol)
{

}

void AbstractHumanPlayer::BufferAction(const Position &clickedTile)
{
	m_bufferedAction = std::make_shared<Position>(clickedTile);
}

void AbstractHumanPlayer::BufferAction(const Position &clickedTile, const Position &releasedTile)
{
	m_bufferedMove = std::make_shared<Move>(Move({ clickedTile, releasedTile }));
}
