#include "Engine/BaseTypes.h"
#include "HumanPlayer.h"
#include "MillState.h"

std::shared_ptr<IState> HumanPlayer::TakeAction(const std::shared_ptr<IState> &crtState)
{
	std::shared_ptr<State<MillChessmans>> nextState;
	if (m_bufferedPositioning)
	{
		MillChessmans chessman;
		if (m_symbol == IState::Winner::FirstPlayer)
			chessman = MillChessmans::White;
		else
			chessman = MillChessmans::Red;

		// generate state
		nextState = static_cast<State<MillChessmans>*>(crtState.get())->GetNextState(*m_bufferedPositioning, chessman);
	}
	else if (m_bufferedMove)
	{
		// simple move
		nextState = static_cast<State<MillChessmans>*>(crtState.get())->GetNextState(*m_bufferedMove);
	}
	else
		// no buffered move
		return std::shared_ptr<IState>();

	// validate
	if (nextState)
	{
		std::vector<std::shared_ptr<IState>> possibleNextStates;
		crtState->GetPossibleNextStates(possibleNextStates);
		bool bFound = false;
		for (const auto &state : possibleNextStates)
			if (static_cast<State<MillChessmans>*>(state.get())->GetBoard() == nextState->GetBoard())
			{
				bFound = true;
				break;
			}

		if (!bFound)
			nextState.reset();
	}

	// set buffered actions as consumed
	m_bufferedPositioning.reset();
	m_bufferedMove.reset();

	return nextState;
}
