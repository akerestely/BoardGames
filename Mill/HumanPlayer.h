#pragma once
#include "BoardGame\AbstractHumanPlayer.h"

class HumanPlayer : public AbstractHumanPlayer
{
	using AbstractHumanPlayer::AbstractHumanPlayer;
	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState) override;
};