#pragma once
#include "IState.h"
#include "Board.h"

static const uint kInvalidChessmanValue = -1;

struct Move
{
	Position from;
	Position to;
};

template<class TChessman>
class State : public IState
{
public:
	State(uint boardRows, uint boardCols, TChessman defaultBoardValues = TChessman::None, bool hashSymmetryDiagonal = true, bool hashSymmetryCardinal = true, bool hashRoatations = true) :
		nextPlayer(Winner::FirstPlayer),
		winner(Winner::None),
		isEnd(false),
		computedHash(false),
		computedEnd(false),
		board(boardRows, boardCols, defaultBoardValues),
		hashRoatations(hashRoatations),
		hashSymmetryCardinal(hashSymmetryCardinal),
		hashSymmetryDiagonal(hashSymmetryDiagonal)
	{
		Invalidate();
	}

	virtual THash GetHash() override final
	{
		if (!computedHash)
		{
			hash = 0;

			THash crtHash;
			uint n = board.Rows();
			uint m = board.Cols();
			uint chessmanCount = getChessmanValue(TChessman::Count);
			uint partialHash;
			uint partialHashMultiplier = (uint)pow(chessmanCount, m);

			// normal traversal
			crtHash = 0;
			for (uint i = 0; i < n; ++i)
			{
				partialHash = 0;
				for (uint j = 0; j < m; ++j)
				{
					auto chessmanVal = getChessmanValue(board[i][j]);
					if (chessmanVal != kInvalidChessmanValue)
					{
						partialHash *= chessmanCount;
						partialHash += chessmanVal;
					}
				}
				crtHash *= partialHashMultiplier;
				crtHash += partialHash;
			}
			if (crtHash > hash)
				hash = crtHash;

			if (hashRoatations)
			{
				// rotated 90 deg
				crtHash = 0;
				for (uint j = m - 1; ~j; --j)
				{
					partialHash = 0;
					for (uint i = 0; i < n; ++i)
					{
						auto chessmanVal = getChessmanValue(board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > hash)
					hash = crtHash;

				// rotated 180 deg
				crtHash = 0;
				for (uint i = n - 1; ~i; --i)
				{
					partialHash = 0;
					for (uint j = m - 1; ~j; --j)
					{
						auto chessmanVal = getChessmanValue(board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > hash)
					hash = crtHash;

				// rotated 270(i.e. -90) deg
				crtHash = 0;
				for (uint j = 0; j < m; ++j)
				{
					partialHash = 0;
					for (uint i = n - 1; ~i; --i)
					{
						auto chessmanVal = getChessmanValue(board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > hash)
					hash = crtHash;
			}

			if (hashSymmetryCardinal)
			{
				// symmetry by vertical axis
				crtHash = 0;
				for (uint i = 0; i < n; ++i)
				{
					partialHash = 0;
					for (uint j = m - 1; ~j; --j)
					{
						auto chessmanVal = getChessmanValue(board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > hash)
					hash = crtHash;

				// symmetry by horizontal axis
				crtHash = 0;
				for (uint i = n - 1; ~i; --i)
				{
					partialHash = 0;
					for (uint j = 0; j < m; ++j)
					{
						auto chessmanVal = getChessmanValue(board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > hash)
					hash = crtHash;
			}

			if (hashSymmetryDiagonal)
			{
				// symmetry by primary diagonal
				crtHash = 0;
				for (uint j = 0; j < m; ++j)
				{
					partialHash = 0;
					for (uint i = 0; i < n; ++i)
					{
						auto chessmanVal = getChessmanValue(board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > hash)
					hash = crtHash;

				// symmetry by secondary diagonal
				crtHash = 0;
				for (uint j = m - 1; ~j; --j)
				{
					partialHash = 0;
					for (uint i = n - 1; ~i; --i)
					{
						auto chessmanVal = getChessmanValue(board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > hash)
					hash = crtHash;
			}
			computedHash = true;
		}

		return hash;
	}
	virtual bool IsEnd() override final
	{
		if (!computedEnd)
		{
			computeEnd();
			computedEnd = true;
		}

		return isEnd;
	}
	virtual Winner GetWinner() override final
	{
		if (!computedEnd)
			IsEnd();

		return winner;
	}

	virtual const Board<TChessman> &GetBoard() const { return board; }

	virtual std::shared_ptr<State<TChessman>> GetNextState(const Position &pos, TChessman chessman) const
	{
		if (pos.Invalid())
			return std::shared_ptr<State<TChessman>>();

		auto nextState = Produce(*this);
		nextState->board[pos] = chessman;
		if (nextPlayer == Winner::FirstPlayer)
			nextState->nextPlayer = Winner::SecondPlayer;
		else
			nextState->nextPlayer = Winner::FirstPlayer;

		nextState->Invalidate();

		return nextState;
	}

	virtual std::shared_ptr<State<TChessman>> GetNextState(const Move &move) const
	{
		if (move.from.Invalid() || move.to.Invalid())
			return std::shared_ptr<State<TChessman>>();

		auto nextState = Produce(*this);
		std::swap(nextState->board[move.from], nextState->board[move.to]);
		if (nextPlayer == Winner::FirstPlayer)
			nextState->nextPlayer = Winner::SecondPlayer;
		else
			nextState->nextPlayer = Winner::FirstPlayer;

		nextState->Invalidate();

		return nextState;
	}

	std::shared_ptr<State<TChessman>> GetNextState(const Move &move, TChessman chessman) const
	{
		auto nextState = State<TChessman>::GetNextState(move);
		if(nextState)
			nextState->board[move.to] = chessman;
		return nextState;
	}

	virtual Winner GetNextPlayer() override
	{
		return nextPlayer;
	}

protected:
	virtual std::shared_ptr<State<TChessman>> Produce(const State<TChessman> &fromState) const = 0;

	void Invalidate()
	{
		computedHash = false;
		computedEnd = false;
		winner = IState::Winner::None;
	}

private:
	virtual uint getChessmanValue(TChessman chessman) = 0;
	virtual void computeEnd() = 0;

protected:
	Board<TChessman> board;
	THash hash;
	Winner winner;
	Winner nextPlayer;
	bool isEnd : 1;

private:
	bool computedHash : 1;
	bool computedEnd : 1;

	bool hashRoatations : 1;
	bool hashSymmetryCardinal : 1;	// vertical and horizontal symmetry
	bool hashSymmetryDiagonal : 1;	// symmetry along primary and secondary diagonal
};

