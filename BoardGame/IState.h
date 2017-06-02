#pragma once
#include <vector>
#include <memory>
//#include "InfInt.h"

struct IState
{
	using THash = std::uint64_t;
	//using THash = InfInt;

	enum class Winner : char
	{
		None,
		FirstPlayer,	// player who makes the first move
		SecondPlayer	// player who makes the second move
	};

	virtual ~IState() = default;
	virtual THash GetHash() = 0;
	virtual bool IsEnd() = 0;
	virtual Winner GetWinner() = 0;
	virtual Winner GetNextPlayer() = 0;

	virtual void GetPossibleNextStates(std::vector<std::shared_ptr<IState>> &states) const = 0;
};