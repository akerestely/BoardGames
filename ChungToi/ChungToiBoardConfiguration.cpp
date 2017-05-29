#include "Engine/BaseTypes.h"
#include "ChungToiBoardConfiguration.h"

ChungToiBoardConfiguration::ChungToiBoardConfiguration(std::shared_ptr<IRenderable> whiteCardinal, std::shared_ptr<IRenderable> whiteDiagonal, std::shared_ptr<IRenderable> redCardinal, std::shared_ptr<IRenderable> redDiagonal) :
	m_whiteCardinal(whiteCardinal),
	m_whiteDiagonal(whiteDiagonal),
	m_redCardinal(redCardinal),
	m_redDiagonal(redDiagonal)
{

}

void ChungToiBoardConfiguration::Init()
{
	const float kTileWidth = 60;
	const float kDistBetweenTiles = 5;
	const float kDistBetweenTilesCenter = kTileWidth + kDistBetweenTiles;

	m_boardTiles.reserve(9);
	rectf box;
	box.Set(0, 0, kTileWidth, kTileWidth);
	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
			m_boardTiles.emplace_back(box.CenterOn(kDistBetweenTilesCenter * j, -kDistBetweenTilesCenter * i), Position(i + 1, j + 1));
}

std::shared_ptr<IRenderable> ChungToiBoardConfiguration::getChessman(ChungToiChessmans type) const
{
	switch (type)
	{
	case ChungToiChessmans::WhiteCardinal:	return m_whiteCardinal;
	case ChungToiChessmans::WhiteDiagonal:	return m_whiteDiagonal;
	case ChungToiChessmans::RedCardinal:	return m_redCardinal;
	case ChungToiChessmans::RedDiagonal:	return m_redDiagonal;
	}

	return std::shared_ptr<IRenderable>();
}
