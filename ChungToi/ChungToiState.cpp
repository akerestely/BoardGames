#include "Engine/BaseTypes.h"
#include "ChungToiState.h"

ChungToiState::ChungToiState() : State(3, 3)
{

}

void ChungToiState::GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const
{
	ChungToiChessmans chessman;
	if (nextPlayer == Winner::FirstPlayer)
		chessman = ChungToiChessmans::WhiteCardinal;
	else
		chessman = ChungToiChessmans::RedCardinal;

	uint n = board.Rows();
	uint m = board.Cols();
	for (uint i = 0; i < n; ++i)
		for (uint j = 0; j < m; ++j)
			if (board[i][j] == ChungToiChessmans::None)
				states.push_back(this->GetNextState(Position(i, j), chessman));
}

std::shared_ptr<State<ChungToiChessmans>> ChungToiState::Produce(const State<ChungToiChessmans> &fromState) const
{
	return std::make_shared<ChungToiState>(static_cast<const ChungToiState&>(fromState));
}

IState::THash ChungToiState::getChessmanValue(ChungToiChessmans chessman)
{
	switch (chessman)
	{
	case ChungToiChessmans::None:
		return 0;
	case ChungToiChessmans::WhiteCardinal:
		return 1;
	case ChungToiChessmans::WhiteDiagonal:
		return 2;
	case ChungToiChessmans::RedCardinal:
		return 3;
	case ChungToiChessmans::RedDiagonal:
		return 4;
	case ChungToiChessmans::Count:
		return 5;
	default:
		return -1;
	}
}

void ChungToiState::computeEnd()
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
			switch (board[i][j])
			{
			case ChungToiChessmans::WhiteCardinal:
			case ChungToiChessmans::WhiteDiagonal:
				sumVal = 1; break;
			case ChungToiChessmans::RedCardinal:
			case ChungToiChessmans::RedDiagonal:
				sumVal = -1; break;
			}

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
