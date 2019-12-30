#pragma once

#pragma once
#include "BoardGame/State.h"

enum class MillChessmans : char
{
	Skip = -1,
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

	Winner getCurrentPlayer() const;
	uint8_t& getPlacedPieces(Winner player);
	uint8_t getLostPieces(Winner player) const;
	uint8_t& getLostPieces(Winner player);

	const std::vector<Winner>& getMills() const;
	bool needToRemoveEnemyPiece(const IState::Winner crtPlayer) const;

	virtual std::shared_ptr<State<MillChessmans>> Produce(const State<MillChessmans> &fromState) const override;
	virtual std::shared_ptr<State<MillChessmans>> GetNextState(const Position &pos, MillChessmans chessman) const override;
	virtual std::shared_ptr<State<MillChessmans>> GetNextState(const Move &move) const override;
	virtual uint32_t getChessmanValue(MillChessmans chessman) override;
	virtual void computeEnd() override;

private:
	uint8_t m_pPlacedPieces[2];
	uint8_t m_pLostPieces[2];
	bool m_bNeedToRemovePiece;

	std::vector<Winner> m_mills;
	std::vector<Winner> m_previousStateMills;
};

