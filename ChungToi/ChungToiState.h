#pragma once

#pragma once
#include "BoardGame/State.h"

enum class ChungToiChessmans
{
	None,
	WhiteCardinal,
	WhiteDiagonal,
	RedCardinal,
	RedDiagonal,

	Count
};

class ChungToiState : public State<ChungToiChessmans>
{
public:
	ChungToiState();

	virtual void GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const override;
	bool NextPlayerHasChessmans() const;

private:
	IState::Winner getChessmanPlayer(ChungToiChessmans chessman) const;
	bool canMoveTo(const Position &position) const;

	virtual std::shared_ptr<State<ChungToiChessmans>> Produce(const State<ChungToiChessmans> &fromState) const override;
	virtual uint32_t getChessmanValue(ChungToiChessmans chessman) override;
	virtual void computeEnd() override;
};

