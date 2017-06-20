#include "Engine/BaseTypes.h"
#include "HumanPlayer.h"

#include "ChungToiState.h"

ChungToiChessmans rotatedChessman(ChungToiChessmans chessman)
{
	switch (chessman)
	{
	case ChungToiChessmans::WhiteCardinal:
		return ChungToiChessmans::WhiteDiagonal;
	case ChungToiChessmans::WhiteDiagonal:
		return ChungToiChessmans::WhiteCardinal;
	case ChungToiChessmans::RedCardinal:
		return ChungToiChessmans::RedDiagonal;
	case ChungToiChessmans::RedDiagonal:
		return ChungToiChessmans::RedCardinal;
	}

	return ChungToiChessmans::None;
}

std::shared_ptr<IState> HumanPlayer::TakeAction(const std::shared_ptr<IState> &crtState)
{
	ChungToiChessmans chessman = ChungToiChessmans::None;
	ChungToiState *pCrtState = static_cast<ChungToiState*>(crtState.get());

	std::shared_ptr<State<ChungToiChessmans>> nextState;
	if (m_bufferedPositioning)
	{
		if (pCrtState->NextPlayerHasChessmans())
		{
			// set default
			if (m_symbol == IState::Winner::FirstPlayer)
				chessman = ChungToiChessmans::WhiteCardinal;
			else
				chessman = ChungToiChessmans::RedCardinal;

			// rotate if needed
			if (m_bBufferedRotation)
				chessman = rotatedChessman(chessman);
		}
		else
			// rotate the current selected
			chessman = rotatedChessman(pCrtState->GetBoard()[*m_bufferedPositioning]);

		// generate state
		nextState = static_cast<ChungToiState*>(crtState.get())->GetNextState(*m_bufferedPositioning, chessman);
	}
	else if (m_bufferedMove)
	{
		if(!m_bBufferedRotation)
		// simple move
			nextState = static_cast<ChungToiState*>(crtState.get())->GetNextState(*m_bufferedMove);
		else
		{
			// move and rotate based on dragged
			chessman = rotatedChessman(pCrtState->GetBoard()[m_bufferedMove->from]);
			nextState = static_cast<ChungToiState*>(crtState.get())->GetNextState(*m_bufferedMove, chessman);
		};
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
		for(const auto &state : possibleNextStates)
			if (state->GetHash() == nextState->GetHash())
			{
				bFound = true;
				break;
			}

		if(!bFound)
			nextState.reset();
	}

	// set buffered actions as consumed
	m_bufferedPositioning.reset();
	m_bufferedMove.reset();
	m_bBufferedRotation = false;

	return nextState;
}

void HumanPlayer::BufferRotation()
{
	m_bBufferedRotation = !m_bBufferedRotation;
}
