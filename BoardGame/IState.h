#pragma once
#include "Engine/BaseTypes.h"

struct IState
{
	using THash = uint;
	enum class Winner
	{
		None,
		FirstPlayer,	// player who makes the first move
		SecondPlayer	// player who makes the second move
	};

	virtual ~IState() = default;
	virtual THash GetHash() = 0;
	virtual bool IsEnd() = 0;
	virtual Winner GetWinner() = 0;
};