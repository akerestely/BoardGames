#include "Engine/BaseTypes.h"
#include "HumanPlayer.h"
#include "TicTacToeState.h"

std::shared_ptr<IState> HumanPlayer::TakeAction(const std::shared_ptr<IState> &crtState)
{
	if (!m_bufferedPositioning)
		return std::shared_ptr<IState>();

	TicTacToeChessmans chessman;
	if (m_symbol == IState::Winner::FirstPlayer)
		chessman = TicTacToeChessmans::Cross;
	else
		chessman = TicTacToeChessmans::Nought;

	auto nextState = static_cast<TicTacToeState*>(crtState.get())->GetNextState(*m_bufferedPositioning, chessman);
	m_bufferedPositioning.reset();

	return nextState;
}
