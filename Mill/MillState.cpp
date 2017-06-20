#include "Engine/BaseTypes.h"
#include "MillState.h"

#include <assert.h>

const uint kMaxMills = 16;
const byte kMaxPieces = 9;
const byte kMaxLoosablePieces = 6;
const uint kLayers = 3;

MillState::MillState() : 
	State(7, 7, MillChessmans::Skip),
	m_bNeedToRemovePiece(false)
{
	// setup board
	Position pos;
	for (uint iLayer = kLayers; iLayer > 0; --iLayer)
	{
		uint crtLayer = kLayers - iLayer;
		for (uint i = 0; i < 3; ++i)
		{
			pos.i = crtLayer + i * iLayer;
			for (uint j = 0; j < 3; ++j)
			{
				if (i == j && j == 1)
					continue;

				pos.j = crtLayer + j * iLayer;
				m_board[pos] = MillChessmans::None;
			}
		}
	}

	memset(m_pPlacedPieces, 0, sizeof(m_pPlacedPieces));
	memset(m_pLostPieces, 0, sizeof(m_pLostPieces));
}

void MillState::GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const
{
	MillChessmans chessman, enemyChessman;
	if (m_nextPlayer == Winner::FirstPlayer)
	{
		chessman = MillChessmans::White;
		enemyChessman = MillChessmans::Red;
	}
	else
	{
		chessman = MillChessmans::Red;
		enemyChessman = MillChessmans::White;
	}

	if (m_bNeedToRemovePiece)
	{
		auto &mills = getMills();
		uint nRows = m_board.Rows();

		Position pos;
		for (uint iLayer = kLayers; iLayer > 0; --iLayer)
		{
			uint crtLayer = kLayers - iLayer;
			for (uint i = 0; i < 3; ++i)
			{
				pos.i = crtLayer + i * iLayer;
				for (uint j = 0; j < 3; ++j)
				{
					if (i == j && j == 1)
						continue;

					pos.j = crtLayer + j * iLayer;
					if (m_board[pos] != enemyChessman)
						continue;

					if (mills[i != 1 || i > j ? pos.i : 14] == getCurrentPlayer())
						continue;
					if (mills[j != 1 || j > i ? nRows + pos.j : 15] == getCurrentPlayer())
						continue;

					states.push_back(GetNextState(pos, MillChessmans::None));
				}
			}
		}
	}
	else if (NextPlayerHasChessmans())
	{
		Position pos;
		for (uint iLayer = kLayers; iLayer > 0; --iLayer)
		{
			uint crtLayer = kLayers - iLayer;
			for (uint i = 0; i < 3; ++i)
			{
				pos.i = crtLayer + i * iLayer;
				for (uint j = 0; j < 3; ++j)
				{
					if (i == j && j == 1)
						continue;

					pos.j = crtLayer + j * iLayer;
					if (m_board[pos] != MillChessmans::None)
						continue;

					states.push_back(GetNextState(pos, chessman));
				}
			}
		}
	}
	else if (const_cast<MillState*>(this)->getLostPieces(m_nextPlayer) < kMaxLoosablePieces)
	{
		std::vector<Position> neighbors;

		Position pos;
		for (uint iLayer = kLayers; iLayer > 0; --iLayer)
		{
			uint crtLayer = kLayers - iLayer;
			for (uint i = 0; i < 3; ++i)
			{
				pos.i = crtLayer + i * iLayer;
				for (uint j = 0; j < 3; ++j)
				{
					if (i == j && j == 1)
						continue;

					pos.j = crtLayer + j * iLayer;
					if (m_board[pos] != chessman)
						continue;

					neighbors.clear();
					if(i != 1)
						neighbors.insert(neighbors.end(), {
							// move horizontally on layer
							{ pos.i, pos.j - iLayer },{ pos.i, pos.j + iLayer }
						});
					else
						neighbors.insert(neighbors.end(), {
							// move horizontally on adjacent tiles
							{ pos.i, pos.j - 1 },{ pos.i, pos.j + 1 }
						});
					if(j != 1)
						neighbors.insert(neighbors.end(), {
							// move vertically on layer
							{pos.i - iLayer, pos.j}, {pos.i + iLayer, pos.j}
						});
					else
						neighbors.insert(neighbors.end(), {
							// move vertically on adjacent tiles
							{ pos.i - 1, pos.j },{ pos.i + 1, pos.j }
						});

					for (const auto &nextPos : neighbors)
						if (canMoveTo(nextPos))
							states.push_back(GetNextState(Move({ pos, nextPos })));
				}
			}
		}
	}
	else if(getLostPieces(m_nextPlayer) == kMaxLoosablePieces)
	{
		std::vector<Position> crtPieces;
		crtPieces.reserve(3);
		std::vector<Position> emptyTiles;
		emptyTiles.reserve(6 + getLostPieces(Winner::FirstPlayer) + getLostPieces(Winner::SecondPlayer));

		Position pos;
		for (uint iLayer = kLayers; iLayer > 0; --iLayer)
		{
			uint crtLayer = kLayers - iLayer;
			for (uint i = 0; i < 3; ++i)
			{
				pos.i = crtLayer + i * iLayer;
				for (uint j = 0; j < 3; ++j)
				{
					if (i == j && j == 1)
						continue;

					pos.j = crtLayer + j * iLayer;
					if (m_board[pos] == chessman)
						crtPieces.push_back(pos);
					else if (m_board[pos] == MillChessmans::None)
						emptyTiles.push_back(pos);
				}
			}
		}

		for (const auto &crtPos : crtPieces)
			for (const auto &emptyPos : emptyTiles)
				states.push_back(GetNextState(Move({ crtPos, emptyPos })));
	}
	else
	{
		throw "Unhandled case";
	}

	if (states.empty())
	{
		auto nextState = std::static_pointer_cast<MillState>(Produce(*this));
		nextState->m_nextPlayer = getCurrentPlayer();
		states.push_back(nextState);
	}
}

bool MillState::NextPlayerHasChessmans() const
{
	return ((MillState*)this)->getPlacedPieces(m_nextPlayer) < kMaxPieces;
}

IState::Winner MillState::getChessmanPlayer(MillChessmans chessman) const
{
	switch (chessman)
	{
	case MillChessmans::White:	return IState::Winner::FirstPlayer;
	case MillChessmans::Red:	return IState::Winner::SecondPlayer;
	}

	throw "Unhandled case";
}

bool MillState::canMoveTo(const Position &position) const
{
	return position.i >= 0 && position.i < m_board.Rows() && position.j >= 0 && position.j < m_board.Cols() 
		&& m_board[position] == MillChessmans::None;
}

IState::Winner MillState::getCurrentPlayer() const
{
	switch (m_nextPlayer)
	{
	case IState::Winner::FirstPlayer:	return Winner::SecondPlayer;
	case IState::Winner::SecondPlayer:	return Winner::FirstPlayer;
	}

	throw "Unhandled case";
}

byte& MillState::getPlacedPieces(Winner player)
{
	switch (player)
	{
	case IState::Winner::FirstPlayer:	return m_pPlacedPieces[0];
	case IState::Winner::SecondPlayer:	return m_pPlacedPieces[1];
	}

	throw "Unhandled case";
}

byte& MillState::getLostPieces(Winner player)
{
	switch (player)
	{
	case IState::Winner::FirstPlayer:	return m_pLostPieces[0];
	case IState::Winner::SecondPlayer:	return m_pLostPieces[1];
	}

	throw "Unhandled case";
}

byte MillState::getLostPieces(Winner player) const
{
	switch (player)
	{
	case IState::Winner::FirstPlayer:	return m_pLostPieces[0];
	case IState::Winner::SecondPlayer:	return m_pLostPieces[1];
	}

	throw "Unhandled case";
}

const std::vector<IState::Winner>& MillState::getMills() const
{
	if (!m_mills.empty())
		return m_mills;

	const uint n = m_board.Rows();
	const uint m = m_board.Cols();

	int results[kMaxMills] = {};
	Position pos;
	for (uint iLayer = kLayers; iLayer > 0; --iLayer)
	{
		uint crtLayer = kLayers - iLayer;
		for (uint i = 0; i < 3; ++i)
		{
			pos.i = crtLayer + i * iLayer;
			for (uint j = 0; j < 3; ++j)
			{
				if (i == j && j == 1)
					continue;

				pos.j = crtLayer + j * iLayer;
				int sumVal = 0;
				switch (m_board[pos])
				{
				case MillChessmans::White:
					sumVal = 1; break;
				case MillChessmans::Red:
					sumVal = -1; break;
				}

				if (sumVal != 0)
				{
					// rows
					results[i != 1 || i > j ? pos.i : 14] += sumVal;

					// cols
					results[j != 1 || j > i ? n + pos.j : 15] += sumVal;
				}
			}
		}
	}

	const_cast<std::vector<Winner>&>(m_mills).reserve(kMaxMills);
	for (auto result : results)
		const_cast<std::vector<Winner>&>(m_mills).push_back(result == 3 ? Winner::FirstPlayer : result == -3 ? Winner::SecondPlayer : Winner::None);

	return m_mills;
}

bool MillState::needToRemoveEnemyPiece(const IState::Winner crtPlayer) const
{
	if (m_previousStateMills.empty())
		return false;

	for (uint i = 0; i < kMaxMills; ++i)
		if (m_mills[i] == crtPlayer && m_previousStateMills[i] != crtPlayer)
			return true;

	return false;
}

std::shared_ptr<State<MillChessmans>> MillState::Produce(const State<MillChessmans> &fromState) const
{
	return std::make_shared<MillState>(static_cast<const MillState&>(fromState));
}

std::shared_ptr<State<MillChessmans>> MillState::GetNextState(const Position &pos, MillChessmans chessman) const
{
	auto &nextState = std::static_pointer_cast<MillState>(State<MillChessmans>::GetNextState(pos, chessman));
	nextState->m_bNeedToRemovePiece = false;
	nextState->m_previousStateMills.clear();
	std::swap(nextState->m_previousStateMills, nextState->m_mills);
	nextState->getMills();

	if (!m_bNeedToRemovePiece)
	{
		nextState->getPlacedPieces(m_nextPlayer)++;

		if (nextState->needToRemoveEnemyPiece(m_nextPlayer))
		{
			nextState->m_nextPlayer = m_nextPlayer;
			nextState->m_bNeedToRemovePiece = true;
		}
	}
	else
	{
		nextState->getLostPieces(getCurrentPlayer())++;
		nextState->m_board[pos] = MillChessmans::None;
	}

	return nextState;
}

std::shared_ptr<State<MillChessmans>> MillState::GetNextState(const Move &move) const
{
	auto &nextState = std::static_pointer_cast<MillState>(State<MillChessmans>::GetNextState(move));
	nextState->m_bNeedToRemovePiece = false;
	nextState->m_previousStateMills.clear();
	std::swap(nextState->m_previousStateMills, nextState->m_mills);
	nextState->getMills();

	if (nextState->needToRemoveEnemyPiece(m_nextPlayer))
	{
		nextState->m_nextPlayer = m_nextPlayer;
		nextState->m_bNeedToRemovePiece = true;
	}

	return nextState;
}

uint MillState::getChessmanValue(MillChessmans chessman)
{
	switch (chessman)
	{
	case MillChessmans::None:
		return 0;
	case MillChessmans::White:
		return 1;
	case MillChessmans::Red:
		return 2;
	case MillChessmans::Count:
		return 3;
	default:
		return kInvalidChessmanValue;
	}
}

void MillState::computeEnd()
{
	if (getPlacedPieces(m_nextPlayer) < kMaxPieces || getLostPieces(m_nextPlayer) <= kMaxLoosablePieces)
		return;		// it's not end game, and we don't have winner (default values)

	m_isEnd = true;
	m_winner = getCurrentPlayer();
}
