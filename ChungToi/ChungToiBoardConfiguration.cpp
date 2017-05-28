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
	m_boardTiles.resize(9);

	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
		{
			auto &boardTile = m_boardTiles[(i + 1) * 3 + j + 1];
			boardTile.boundingBox.Set(65.f * i, 65.f * j, 60, 60);
			boardTile.boardIndexPos = Position(i + 1, j + 1);
		}
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
