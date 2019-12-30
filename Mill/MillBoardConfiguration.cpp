#include "MillBoardConfiguration.h"

MillBoardConfiguration::MillBoardConfiguration(std::shared_ptr<IRenderable> white, std::shared_ptr<IRenderable> red) :
	m_white(white),
	m_red(red)
{

}

void MillBoardConfiguration::Init()
{
	m_boardTiles.reserve(3 * 8);

	const glm::vec2 kRef(-180, 180);
	const float kWidth = 60;
	rectf box;
	box.Set(posf{ 0, 0 }, sizef{ kWidth, kWidth });
	const uint32_t kLayers = 3;
	for (uint32_t iLayer = kLayers; iLayer > 0; --iLayer)
	{
		for (uint32_t i = 0; i < 3; ++i)
			for (uint32_t j = 0; j < 3; ++j)
			{
				if(i == j && j == 1)
					continue;

				Position indexPos(kLayers - iLayer + i * iLayer, kLayers - iLayer + j * iLayer);
				m_boardTiles.emplace_back(box.CenterOn(kRef.x + indexPos.j * kWidth, kRef.y - indexPos.i * kWidth), indexPos);
			}
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
