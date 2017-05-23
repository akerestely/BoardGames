#pragma once
#include <memory>

struct IState;

struct IPlayer
{
	virtual ~IPlayer() = default;
	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState) = 0;
};