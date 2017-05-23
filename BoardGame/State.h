#pragma once
#include "IState.h"
#include "Board.h"

template<class TChessman>
class State : public IState
{
public:
	State(uint boardRows, uint boardCols, bool hashSymmetryDiagonal = true, bool hashSymmetryCardinal = true, bool hashRoatations = true) :
		nextPlayer(Winner::FirstPlayer),
		board(boardRows, boardCols),
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
			uint chessmanCount = 10;// getChessmanValue(Chessmans::Count);

			// normal traversal
			crtHash = 0;
			for (uint i = 0; i < n; ++i)
				for (uint j = 0; j < m; ++j)
					crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
			if (crtHash > hash)
				hash = crtHash;

			if (hashRoatations)
			{
				// rotated 90 deg
				crtHash = 0;
				for (uint j = m - 1; ~j; --j)
					for (uint i = 0; i < n; ++i)
						crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
				if (crtHash > hash)
					hash = crtHash;

				// rotated 180 deg
				crtHash = 0;
				for (uint i = n - 1; ~i; --i)
					for (uint j = m - 1; ~j; --j)
						crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
				if (crtHash > hash)
					hash = crtHash;

				// rotated 270(i.e. -90) deg
				crtHash = 0;
				for (uint j = 0; j < m; ++j)
					for (uint i = n - 1; ~i; --i)
						crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
				if (crtHash > hash)
					hash = crtHash;
			}

			if (hashSymmetryCardinal)
			{
				// symmetry by vertical axis
				crtHash = 0;
				for (uint i = 0; i < n; ++i)
					for (uint j = m - 1; ~j; --j)
						crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
				if (crtHash > hash)
					hash = crtHash;

				// symmetry by horizontal axis
				crtHash = 0;
				for (uint i = n - 1; ~i; --i)
					for (uint j = 0; j < m; ++j)
						crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
				if (crtHash > hash)
					hash = crtHash;
			}

			if (hashSymmetryDiagonal)
			{
				// symmetry by primary diagonal
				crtHash = 0;
				for (uint j = 0; j < m; ++j)
					for (uint i = 0; i < n; ++i)
						crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
				if (crtHash > hash)
					hash = crtHash;

				// symmetry by secondary diagonal
				crtHash = 0;
				for (uint j = m - 1; ~j; --j)
					for (uint i = n - 1; ~i; --i)
						crtHash = crtHash * chessmanCount + getChessmanValue(board[i][j]);
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

protected:
	void Invalidate()
	{
		computedHash = false;
		computedEnd = false;
		winner = IState::Winner::None;
	}

private:
	virtual THash getChessmanValue(TChessman chessman) = 0;
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
