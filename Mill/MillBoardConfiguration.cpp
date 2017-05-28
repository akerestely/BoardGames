#include "Engine/BaseTypes.h"
#include "MillBoardConfiguration.h"

MillBoardConfiguration::MillBoardConfiguration(std::shared_ptr<IRenderable> white, std::shared_ptr<IRenderable> red) :
	m_white(white),
	m_red(red)
{

}

void MillBoardConfiguration::Init()
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

std::shared_ptr<IRenderable> MillBoardConfiguration::getChessman(MillChessmans type) const
{
	switch (type)
	{
	case MillChessmans::White:	return m_white;
	case MillChessmans::Red:	return m_red;
	}

	return std::shared_ptr<IRenderable>();
}
