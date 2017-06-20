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
		m_nextPlayer(Winner::FirstPlayer),
		m_winner(Winner::None),
		m_isEnd(false),
		m_computedHash(false),
		m_computedEnd(false),
		m_board(boardRows, boardCols, defaultBoardValues),
		m_hashRoatations(hashRoatations),
		m_hashSymmetryCardinal(hashSymmetryCardinal),
		m_hashSymmetryDiagonal(hashSymmetryDiagonal)
	{
		Invalidate();
	}

	virtual THash GetHash() override final
	{
		if (!m_computedHash)
		{
			m_hash = 0;

			THash crtHash;
			uint n = m_board.Rows();
			uint m = m_board.Cols();
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
					auto chessmanVal = getChessmanValue(m_board[i][j]);
					if (chessmanVal != kInvalidChessmanValue)
					{
						partialHash *= chessmanCount;
						partialHash += chessmanVal;
					}
				}
				crtHash *= partialHashMultiplier;
				crtHash += partialHash;
			}
			if (crtHash > m_hash)
				m_hash = crtHash;

			if (m_hashRoatations)
			{
				// rotated 90 deg
				crtHash = 0;
				for (uint j = m - 1; ~j; --j)
				{
					partialHash = 0;
					for (uint i = 0; i < n; ++i)
					{
						auto chessmanVal = getChessmanValue(m_board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > m_hash)
					m_hash = crtHash;

				// rotated 180 deg
				crtHash = 0;
				for (uint i = n - 1; ~i; --i)
				{
					partialHash = 0;
					for (uint j = m - 1; ~j; --j)
					{
						auto chessmanVal = getChessmanValue(m_board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > m_hash)
					m_hash = crtHash;

				// rotated 270(i.e. -90) deg
				crtHash = 0;
				for (uint j = 0; j < m; ++j)
				{
					partialHash = 0;
					for (uint i = n - 1; ~i; --i)
					{
						auto chessmanVal = getChessmanValue(m_board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > m_hash)
					m_hash = crtHash;
			}

			if (m_hashSymmetryCardinal)
			{
				// symmetry by vertical axis
				crtHash = 0;
				for (uint i = 0; i < n; ++i)
				{
					partialHash = 0;
					for (uint j = m - 1; ~j; --j)
					{
						auto chessmanVal = getChessmanValue(m_board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > m_hash)
					m_hash = crtHash;

				// symmetry by horizontal axis
				crtHash = 0;
				for (uint i = n - 1; ~i; --i)
				{
					partialHash = 0;
					for (uint j = 0; j < m; ++j)
					{
						auto chessmanVal = getChessmanValue(m_board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > m_hash)
					m_hash = crtHash;
			}

			if (m_hashSymmetryDiagonal)
			{
				// symmetry by primary diagonal
				crtHash = 0;
				for (uint j = 0; j < m; ++j)
				{
					partialHash = 0;
					for (uint i = 0; i < n; ++i)
					{
						auto chessmanVal = getChessmanValue(m_board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > m_hash)
					m_hash = crtHash;

				// symmetry by secondary diagonal
				crtHash = 0;
				for (uint j = m - 1; ~j; --j)
				{
					partialHash = 0;
					for (uint i = n - 1; ~i; --i)
					{
						auto chessmanVal = getChessmanValue(m_board[i][j]);
						if (chessmanVal != kInvalidChessmanValue)
						{
							partialHash *= chessmanCount;
							partialHash += chessmanVal;
						}
					}
					crtHash *= partialHashMultiplier;
					crtHash += partialHash;
				}
				if (crtHash > m_hash)
					m_hash = crtHash;
			}
			m_computedHash = true;
		}

		return m_hash;
	}
	virtual bool IsTerminal() override final
	{
		if (!m_computedEnd)
		{
			computeEnd();
			m_computedEnd = true;
		}

		return m_isEnd;
	}
	virtual Winner GetWinner() override final
	{
		if (!m_computedEnd)
			IsTerminal();

		return m_winner;
	}

	virtual const Board<TChessman> &GetBoard() const { return m_board; }

	virtual std::shared_ptr<State<TChessman>> GetNextState(const Position &pos, TChessman chessman) const
	{
		if (pos.Invalid())
			return std::shared_ptr<State<TChessman>>();

		auto nextState = Produce(*this);
		nextState->m_board[pos] = chessman;
		if (m_nextPlayer == Winner::FirstPlayer)
			nextState->m_nextPlayer = Winner::SecondPlayer;
		else
			nextState->m_nextPlayer = Winner::FirstPlayer;

		nextState->Invalidate();

		return nextState;
	}

	virtual std::shared_ptr<State<TChessman>> GetNextState(const Move &move) const
	{
		if (move.from.Invalid() || move.to.Invalid())
			return std::shared_ptr<State<TChessman>>();

		auto nextState = Produce(*this);
		std::swap(nextState->m_board[move.from], nextState->m_board[move.to]);
		if (m_nextPlayer == Winner::FirstPlayer)
			nextState->m_nextPlayer = Winner::SecondPlayer;
		else
			nextState->m_nextPlayer = Winner::FirstPlayer;

		nextState->Invalidate();

		return nextState;
	}

	std::shared_ptr<State<TChessman>> GetNextState(const Move &move, TChessman chessman) const
	{
		auto nextState = State<TChessman>::GetNextState(move);
		if(nextState)
			nextState->m_board[move.to] = chessman;
		return nextState;
	}

	virtual Winner GetNextPlayer() override
	{
		return m_nextPlayer;
	}

protected:
	virtual std::shared_ptr<State<TChessman>> Produce(const State<TChessman> &fromState) const = 0;

	void Invalidate()
	{
		m_computedHash = false;
		m_computedEnd = false;
		m_winner = IState::Winner::None;
	}

private:
	virtual uint getChessmanValue(TChessman chessman) = 0;
	virtual void computeEnd() = 0;

protected:
	Board<TChessman> m_board;
	THash m_hash;
	Winner m_winner;
	Winner m_nextPlayer;
	bool m_isEnd : 1;

private:
	bool m_computedHash : 1;
	bool m_computedEnd : 1;

	bool m_hashRoatations : 1;
	bool m_hashSymmetryCardinal : 1;	// vertical and horizontal symmetry
	bool m_hashSymmetryDiagonal : 1;	// symmetry along primary and secondary diagonal
};

