#include "Engine/BaseTypes.h"
#include "HumanPlayer.h"
#include "ChungToiState.h"

std::shared_ptr<IState> HumanPlayer::TakeAction(const std::shared_ptr<IState> &crtState)
{
	if (!m_bufferedAction)
		return std::shared_ptr<IState>();

	ChungToiChessmans chessman;
	if (m_symbol == IState::Winner::FirstPlayer)
		chessman = ChungToiChessmans::WhiteCardinal;
	else
		chessman = ChungToiChessmans::RedDiagonal;

	auto nextState = static_cast<ChungToiState*>(crtState.get())->GetNextState(*m_bufferedAction, chessman);
	m_bufferedAction.reset();

	return nextState;
}
