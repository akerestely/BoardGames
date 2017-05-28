#pragma once
#include "BoardGame\AbstractHumanPlayer.h"

class HumanPlayer : public AbstractHumanPlayer
{
public:
	using AbstractHumanPlayer::AbstractHumanPlayer;

	virtual std::shared_ptr<IState> TakeAction(const std::shared_ptr<IState> &crtState) override;

	void BufferRotation();

private:
	bool m_bBufferedRotation = false;
};
