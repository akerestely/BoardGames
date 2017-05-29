#include "Engine/BaseTypes.h"
#include "MillState.h"

#include <assert.h>

MillState::MillState() : State(7, 7)
{

}

void MillState::GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const
{
	MillChessmans chessman;
	if (nextPlayer == Winner::FirstPlayer)
		chessman = MillChessmans::White;
	else
		chessman = MillChessmans::Red;

	uint n = board.Rows();
	uint m = board.Cols();

	if (NextPlayerHasChessmans())
	{
		for (uint i = 0; i < n; ++i)
			for (uint j = 0; j < m; ++j)
				if (board[i][j] == MillChessmans::None)
						states.push_back(GetNextState(Position(i, j), chessman));
	}
}

bool MillState::NextPlayerHasChessmans() const
{
	uint sum = 0;
	uint n = board.Rows();
	uint m = board.Cols();
	for (uint i = 0; i < n; ++i)
		for (uint j = 0; j < m; ++j)
			if (nextPlayer == getChessmanPlayer(board[i][j]))
				++sum;

	assert(sum <= 3);

	return sum < 3;
}

IState::Winner MillState::getChessmanPlayer(MillChessmans chessman) const
{
	return IState::Winner::None;
}

bool MillState::canMoveTo(const Position &position) const
{
	return position.i >= 0 && position.i < board.Rows() && position.j >= 0 && position.j < board.Cols() && board[position] == MillChessmans::None;
}

std::shared_ptr<State<MillChessmans>> MillState::Produce(const State<MillChessmans> &fromState) const
{
	return std::make_shared<MillState>(static_cast<const MillState&>(fromState));
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
		return -1;
	}
}

void MillState::computeEnd()
{
	std::vector<int> results(8, 0);
	uint chessmanCount = 0;

	// check rows, cols, and diagonals
	uint n = board.Rows();
	uint m = board.Cols();
	for (uint i = 0; i < n; ++i)
	{
		for (uint j = 0; j < m; ++j)
		{
			int sumVal = 0;
// 			switch (board[i][j])
// 			{
// 			case MillChessmans::WhiteCardinal:
// 			case MillChessmans::WhiteDiagonal:
// 				sumVal = 1; break;
// 			case MillChessmans::RedCardinal:
// 			case MillChessmans::RedDiagonal:
// 				sumVal = -1; break;
// 			}

			if (sumVal != 0)
			{
				// rows
				results[i] += sumVal;
				// cols
				results[3 + j] += sumVal;
				// primary diagonal
				if (i == j)
					results[6] += sumVal;
				// secondary diagonal
				if (i == m - 1 - j)
					results[7] += sumVal;

				++chessmanCount;
			}
		}
	}

	// check if we have winner
	for (auto result : results)
	{
		if (result == 3)
		{
			winner = Winner::FirstPlayer;
			isEnd = true;
			return;
		}
		if (result == -3)
		{
			winner = Winner::SecondPlayer;
			isEnd = true;
			return;
		}
	}

	// check if it's a tie
	if (chessmanCount == n*m)
	{
		winner = Winner::None;
		isEnd = true;
		return;
	}

	isEnd = false;
}
