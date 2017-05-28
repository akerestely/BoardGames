#pragma once
#include "Engine/Rectangle.h"

#include "Board.h"
#include "IRenderable.h"

struct IState;
namespace Engine
{
	class Camera2D;
}

class IBoardConfiguration
{
public:
	virtual ~IBoardConfiguration() = default;

	virtual void Init() = 0;
	virtual void Update(const std::shared_ptr<IState> &state) = 0;
	void Render(const Engine::Camera2D &camera)
	{
		for (auto &boardTile : m_boardTiles)
			if (boardTile.chessman)
				boardTile.chessman->Render(camera, boardTile.boundingBox.Center());
	}

	Position GetTilePosition(const glm::vec2 &coords) const
	{
		for (auto &boardTile : m_boardTiles)
			if (boardTile.boundingBox.Contains(coords.x, coords.y))
				return boardTile.boardIndexPos;

		return Position();
	}

protected:
	struct BoardTile
	{
		BoardTile(const rectf &boundingBox, const Position &boardIndexPos, const std::shared_ptr<IRenderable> chessman = std::shared_ptr<IRenderable>()) :
			boundingBox(boundingBox),
			chessman(chessman),
			boardIndexPos(boardIndexPos)
		{}

		rectf boundingBox;
		std::shared_ptr<IRenderable> chessman;
		Position boardIndexPos;
	};

	std::vector<BoardTile> m_boardTiles;
};