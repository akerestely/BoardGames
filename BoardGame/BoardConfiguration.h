#pragma once
#include "IBoardConfiguration.h"

template <class TChessman>
class BoardConfiguration : public IBoardConfiguration
{
	virtual void Update(const std::shared_ptr<IState> &state) override final
	{
		const State<TChessman>* pState = static_cast<State<TChessman>*>(state.get());
		auto &board = pState->GetBoard();
		for (uint i = 0; i < board.Rows(); ++i)
			for (uint j = 0; j < board.Cols(); ++j)
			{
				auto &boardTile = m_boardTiles[i * board.Cols() + j];
				if (board[i][j] != TChessman::None)
					boardTile.chessman = getChessman(board[i][j]);
				else
					boardTile.chessman.reset();
			}
	}

	virtual std::shared_ptr<IRenderable> getChessman(TChessman type) const = 0;
};