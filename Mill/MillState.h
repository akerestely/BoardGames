#pragma once

#pragma once
#include "BoardGame/State.h"

enum class MillChessmans
{
	None,
	White,
	Red,

	Count
};

class MillState : public State<MillChessmans>
{
public:
	MillState();

	virtual void GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const override;
	bool NextPlayerHasChessmans() const;

private:
	IState::Winner getChessmanPlayer(MillChessmans chessman) const;
	bool canMoveTo(const Position &position) const;

	virtual std::shared_ptr<State<MillChessmans>> Produce(const State<MillChessmans> &fromState) const override;
	virtual uint getChessmanValue(MillChessmans chessman) override;
	virtual void computeEnd() override;
};

