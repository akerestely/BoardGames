#pragma once
#include "IBoardConfiguration.h"

template <class TChessman>
class AbstractBoardConfiguration : public IBoardConfiguration
{
	virtual void Update(const std::shared_ptr<IState> &state) override final
	{
		const State<TChessman>* pState = static_cast<State<TChessman>*>(state.get());
		auto &board = pState->GetBoard();
		for (uint32_t i = 0; i < board.Rows(); ++i)
			for (uint32_t j = 0; j < board.Cols(); ++j)
			{
				BoardTile *pBoard = nullptr;
				Position searchedPosition = { i, j };
				for(auto &boardTile : m_boardTiles)
					if (boardTile.boardIndexPos == searchedPosition)
					{
						pBoard = &boardTile;
						break;
					}

				if (!pBoard )
					continue;

				auto &boardTile = *pBoard;
				if (board[i][j] != TChessman::None)
					boardTile.chessman = getChessman(board[i][j]);
				else
					boardTile.chessman.reset();
			}
	}

	virtual std::shared_ptr<IRenderable> getChessman(TChessman type) const = 0;
};
