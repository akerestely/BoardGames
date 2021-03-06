#include "ChungToiState.h"

#include <assert.h>

ChungToiState::ChungToiState() : State(3, 3)
{

}

void ChungToiState::GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const
{
	ChungToiChessmans chessmans[2];
	if (m_nextPlayer == Winner::FirstPlayer)
	{
		chessmans[0] = ChungToiChessmans::WhiteCardinal;
		chessmans[1] = ChungToiChessmans::WhiteDiagonal;
	}
	else
	{
		chessmans[0] = ChungToiChessmans::RedCardinal;
		chessmans[1] = ChungToiChessmans::RedDiagonal;
	}

	uint32_t n = m_board.Rows();
	uint32_t m = m_board.Cols();

	if (NextPlayerHasChessmans())
	{
		for (uint32_t i = 0; i < n; ++i)
			for (uint32_t j = 0; j < m; ++j)
				if (m_board[i][j] == ChungToiChessmans::None)
					for (auto &chessman : chessmans)
						states.push_back(GetNextState(Position(i, j), chessman));
	}
	else
	{
		for (uint32_t i = 0; i < n; ++i)
			for (uint32_t j = 0; j < m; ++j)
				if (m_nextPlayer == getChessmanPlayer(m_board[i][j]))
				{
					Position crtPos = { i, j };
					std::vector<Position> neighbors;

					switch (m_board[crtPos])
					{
					case ChungToiChessmans::WhiteCardinal:
					case ChungToiChessmans::RedCardinal:
						neighbors.insert(neighbors.begin(), {
							// vertical tiles
							{ i - 2, j }, { i - 1, j }, { i + 1, j }, { i + 2, j },
							// horizontal tiles
							{ i, j - 2 }, { i, j - 1 }, { i, j + 1 }, { i, j + 2 }
						});
						break;
					case ChungToiChessmans::WhiteDiagonal:
					case ChungToiChessmans::RedDiagonal:
						neighbors.insert(neighbors.begin(), {
							// primary diagonal
							{ i - 2, j - 2 }, { i - 1, j - 1 }, { i + 1, j + 1 },
							// secondary diagonal
							{ i - 2, j + 2 }, { i - 1, j + 1 }, { i + 1 , j - 1 }, { i + 2 , j - 2 }
						});
						break;
					}

					// move chessmans
					for (const auto &nextPos : neighbors)
						if (canMoveTo(nextPos))
						{
							for (auto &chessman : chessmans)
								states.push_back(GetNextState(Move({ crtPos, nextPos }), chessman));
						}

					// rotate current chessman
					ChungToiChessmans rotatedChessman;
					if (m_board[crtPos] == chessmans[0])
						rotatedChessman = chessmans[1];
					if (m_board[crtPos] == chessmans[1])
						rotatedChessman = chessmans[0];

					states.push_back(GetNextState(crtPos, rotatedChessman));
				}
	}
}

bool ChungToiState::NextPlayerHasChessmans() const
{
	uint32_t sum = 0;
	uint32_t n = m_board.Rows();
	uint32_t m = m_board.Cols();
	for (uint32_t i = 0; i < n; ++i)
		for (uint32_t j = 0; j < m; ++j)
			if (m_nextPlayer == getChessmanPlayer(m_board[i][j]))
				++sum;

	assert(sum <= 3);

	return sum < 3;
}

IState::Winner ChungToiState::getChessmanPlayer(ChungToiChessmans chessman) const
{
	switch (chessman)
	{
	case ChungToiChessmans::WhiteCardinal:
	case ChungToiChessmans::WhiteDiagonal:
		return IState::Winner::FirstPlayer;
	case ChungToiChessmans::RedCardinal:
	case ChungToiChessmans::RedDiagonal:
		return IState::Winner::SecondPlayer;
	}

	return IState::Winner::None;
}

bool ChungToiState::canMoveTo(const Position &position) const
{
	return position.i >= 0 && position.i < m_board.Rows() && position.j >= 0 && position.j < m_board.Cols() && m_board[position] == ChungToiChessmans::None;
}

std::shared_ptr<State<ChungToiChessmans>> ChungToiState::Produce(const State<ChungToiChessmans> &fromState) const
{
	return std::make_shared<ChungToiState>(static_cast<const ChungToiState&>(fromState));
}

uint32_t ChungToiState::getChessmanValue(ChungToiChessmans chessman)
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

	// check rows, cols, and diagonals
	uint32_t n = m_board.Rows();
	uint32_t m = m_board.Cols();
	for (uint32_t i = 0; i < n; ++i)
	{
		for (uint32_t j = 0; j < m; ++j)
		{
			int sumVal = 0;
			switch (m_board[i][j])
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
			}
		}
	}

	// check if we have winner
	for (auto result : results)
	{
		if (result == 3)
		{
			m_winner = Winner::FirstPlayer;
			m_isEnd = true;
			return;
		}
		if (result == -3)
		{
			m_winner = Winner::SecondPlayer;
			m_isEnd = true;
			return;
		}
	}

	m_isEnd = false;
}
