#pragma once
#include "BoardGame/AbstractBoardConfiguration.h"
#include "MillState.h"

class MillBoardConfiguration : public AbstractBoardConfiguration<MillChessmans>
{
public:
	MillBoardConfiguration(std::shared_ptr<IRenderable> white, std::shared_ptr<IRenderable> red);
	virtual void Init() override;

private:
	virtual std::shared_ptr<IRenderable>getChessman(MillChessmans type) const override;

private:
	std::shared_ptr<IRenderable> m_white;
	std::shared_ptr<IRenderable> m_red;
};
