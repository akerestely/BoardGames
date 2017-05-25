#pragma once
#include "BoardGame/State.h"

enum class TicTacToeChessmans
{
	None,
	Cross,
	Nought,

	Count
};

class TicTacToeState : public State<TicTacToeChessmans>
{
public:
	TicTacToeState() : State(3, 3)
	{
	}

	virtual void GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const override
	{
		TicTacToeChessmans chessman;
		if (nextPlayer == Winner::FirstPlayer)
			chessman = TicTacToeChessmans::Cross;
		else
			chessman = TicTacToeChessmans::Nought;

		uint n = board.Rows();
		uint m = board.Cols();
		for (uint i = 0; i < n; ++i)
			for (uint j = 0; j < m; ++j)
				if (board[i][j] == TicTacToeChessmans::None)
					states.push_back(this->GetNextState(Position(i,j), chessman));
	}

private:
	virtual std::shared_ptr<State<TicTacToeChessmans>> Produce(const State<TicTacToeChessmans> &fromState) const override
	{
		return std::make_shared<TicTacToeState>(static_cast<const TicTacToeState&>(fromState));
	}

	virtual THash getChessmanValue(TicTacToeChessmans chessman) override
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
	virtual void computeEnd() override
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
};

