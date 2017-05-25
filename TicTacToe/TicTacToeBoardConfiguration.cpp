#include "Engine/BaseTypes.h"
#include "TicTacToeBoardConfiguration.h"

TicTacToeBoardConfig::TicTacToeBoardConfig(std::shared_ptr<IRenderable> cross, std::shared_ptr<IRenderable> nought) :
	m_cross(cross), m_nought(nought)
{
}

void TicTacToeBoardConfig::Init()
{
	m_boardTiles.resize(9);

	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
		{
			auto &boardTile = m_boardTiles[(i + 1) * 3 + j + 1];
			boardTile.boundingBox.Set(65.f * i, 65.f * j, 60, 60);
			boardTile.boardIndexPos = Position(i + 1, j + 1);
		}
}

std::shared_ptr<IRenderable> TicTacToeBoardConfig::getChessman(TicTacToeChessmans type) const
{
	switch (type)
	{
	case TicTacToeChessmans::Cross:		return m_cross;
	case TicTacToeChessmans::Nought:	return m_nought;

	default:							return std::shared_ptr<IRenderable>();
	}
}
