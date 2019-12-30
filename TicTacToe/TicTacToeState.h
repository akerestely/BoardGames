#pragma once
#include "BoardGame/State.h"

enum class TicTacToeChessmans
{
	None,
	Cross,
	Nought,

	Count
};

class TicTacToeState : public State<TicTacToeChessmans>
{
public:
	TicTacToeState();
	virtual void GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const override;

private:
	virtual std::shared_ptr<State<TicTacToeChessmans>> Produce(const State<TicTacToeChessmans> &fromState) const override;
	virtual uint32_t getChessmanValue(TicTacToeChessmans chessman) override;
	virtual void computeEnd() override;
};

