#pragma once
#include "State.h"
#include <memory>

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
	struct Move
	{
		Position from;
		Position to;
	};

public:
	TicTacToeState() : State(3, 3)
	{
	}

	std::shared_ptr<TicTacToeState> GetNextState(const Position &pos)
	{
		if (pos.Invalid() || board[pos] != TicTacToeChessmans::None)
			return std::shared_ptr<TicTacToeState>();

		auto nextState = std::make_shared<TicTacToeState>(*this);
		switch (nextPlayer)
		{
		case Winner::FirstPlayer:
			nextState->board[pos] = TicTacToeChessmans::Cross;
			nextState->nextPlayer = Winner::SecondPlayer;
			break;
		case Winner::SecondPlayer:
			nextState->board[pos] = TicTacToeChessmans::Nought;
			nextState->nextPlayer = Winner::FirstPlayer;
			break;
		}

		nextState->Invalidate();

		return nextState;
	}

	std::shared_ptr<TicTacToeState> GetNextState(const Move &move)
	{
		if (move.to.Invalid() || board[move.to] != TicTacToeChessmans::None)
			return std::shared_ptr<TicTacToeState>();

		auto nextState = std::make_shared<TicTacToeState>(*this);
		std::swap(nextState->board[move.from], nextState->board[move.to]);
		if (nextPlayer == Winner::FirstPlayer)
			nextState->nextPlayer = Winner::SecondPlayer;
		else
			nextState->nextPlayer = Winner::FirstPlayer;

		return nextState;
	}

	void GetPossibleNextStates(std::vector<std::shared_ptr<TicTacToeState>> &states)
	{
		uint n = board.Rows();
		uint m = board.Cols();
		for (uint i = 0; i < n; ++i)
			for (uint j = 0; j < m; ++j)
				if (board[i][j] == TicTacToeChessmans::None)
					states.push_back(this->GetNextState(Position(i,j)));
	}

private:
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

