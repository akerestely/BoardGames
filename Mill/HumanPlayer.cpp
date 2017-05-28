#include "Engine/BaseTypes.h"
#include "HumanPlayer.h"

std::shared_ptr<IState> HumanPlayer::TakeAction(const std::shared_ptr<IState> &crtState)
{
	//if (!m_bufferedAction)
		return std::shared_ptr<IState>();

	
	//m_bufferedAction.reset();

	//return nextState;
}
