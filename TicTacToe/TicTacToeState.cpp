#include "Engine/BaseTypes.h"
#include "TicTacToeState.h"

TicTacToeState::TicTacToeState() : State(3, 3)
{

}

void TicTacToeState::GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const
{
	TicTacToeChessmans chessman;
	if (m_nextPlayer == Winner::FirstPlayer)
		chessman = TicTacToeChessmans::Cross;
	else
		chessman = TicTacToeChessmans::Nought;

	uint n = m_board.Rows();
	uint m = m_board.Cols();
	for (uint i = 0; i < n; ++i)
		for (uint j = 0; j < m; ++j)
			if (m_board[i][j] == TicTacToeChessmans::None)
				states.push_back(this->GetNextState(Position(i, j), chessman));
}

std::shared_ptr<State<TicTacToeChessmans>> TicTacToeState::Produce(const State<TicTacToeChessmans> &fromState) const
{
	return std::make_shared<TicTacToeState>(static_cast<const TicTacToeState&>(fromState));
}

uint TicTacToeState::getChessmanValue(TicTacToeChessmans chessman)
{
	switch (chessman)
	{
	case TicTacToeChessmans::None:
		return 0;
	case TicTacToeChessmans::Cross:
		return 1;
	case TicTacToeChessmans::Nought:
		return 2;
	case TicTacToeChessmans::Count:
		return 3;
	default:
		return -1;
		// assert
	}
}

void TicTacToeState::computeEnd()
{
	std::vector<int> results(8, 0);
	uint chessmanCount = 0;

	// check rows, cols, and diagonals
	uint n = m_board.Rows();
	uint m = m_board.Cols();
	for (uint i = 0; i < n; ++i)
	{
		for (uint j = 0; j < m; ++j)
		{
			int sumVal = 0;
			switch (m_board[i][j])
			{
			case TicTacToeChessmans::Cross:
				sumVal = 1; break;
			case TicTacToeChessmans::Nought:
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

	// check if it's a tie
	if (chessmanCount == n*m)
	{
		m_winner = Winner::None;
		m_isEnd = true;
		return;
	}

	m_isEnd = false;
}
