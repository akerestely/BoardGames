#include "Engine/BaseTypes.h"
#include "TicTacToeBoardConfiguration.h"

TicTacToeBoardConfig::TicTacToeBoardConfig(std::shared_ptr<IRenderable> cross, std::shared_ptr<IRenderable> nought) :
	m_cross(cross), m_nought(nought)
{
}

void TicTacToeBoardConfig::Init()
{
	const float kTileWidth = 60;
	const float kDistBetweenTiles = 5;
	const float kDistBetweenTilesCenter = kTileWidth + kDistBetweenTiles;

	m_boardTiles.reserve(9);
	rectf box;
	box.Set(posf{ 0, 0 }, sizef{ kTileWidth, kTileWidth });
	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
			m_boardTiles.emplace_back(box.CenterOn(kDistBetweenTilesCenter * j, -kDistBetweenTilesCenter * i), Position(i + 1, j + 1));
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
