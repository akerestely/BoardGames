#pragma once
#include "BoardGame/AbstractBoardConfiguration.h"
#include "ChungToiState.h"

class ChungToiBoardConfiguration : public AbstractBoardConfiguration<ChungToiChessmans>
{
public:
	ChungToiBoardConfiguration(std::shared_ptr<IRenderable> whiteCardinal, std::shared_ptr<IRenderable> whiteDiagonal, std::shared_ptr<IRenderable> redCardinal, std::shared_ptr<IRenderable> redDiagonal);
	virtual void Init() override;

private:
	virtual std::shared_ptr<IRenderable>getChessman(ChungToiChessmans type) const override;

private:
	std::shared_ptr<IRenderable> m_whiteCardinal;
	std::shared_ptr<IRenderable> m_whiteDiagonal;
	std::shared_ptr<IRenderable> m_redCardinal;
	std::shared_ptr<IRenderable> m_redDiagonal;
};