#pragma once
#include "BoardGame/AbstractBoardConfiguration.h"
#include "TicTacToeState.h"

class TicTacToeBoardConfig : public AbstractBoardConfiguration<TicTacToeChessmans>
{
public:
	TicTacToeBoardConfig(std::shared_ptr<IRenderable> cross, std::shared_ptr<IRenderable> nought);

	virtual void Init() override;

	virtual std::shared_ptr<IRenderable>getChessman(TicTacToeChessmans type) const override;

private:
	std::shared_ptr<IRenderable> m_cross;
	std::shared_ptr<IRenderable> m_nought;
};
