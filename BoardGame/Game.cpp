#include "Game.h"
#include "Engine/Engine.h"
#include "Engine/Logger.h"
#include "Engine/GLSLProgram.h"

#include "IRenderable.h"
#include "AbstractHumanPlayer.h"
#include "IBoardConfiguration.h"

Game::Game() : IGame()
{
}

void Game::onInit()
{
	onInitRendering();

	m_camera.Init(m_screenWidth, m_screenHeight);
	m_judger.InitGame(getPlayer(IState::Winner::FirstPlayer), getPlayer(IState::Winner::SecondPlayer), getStartingState());
	m_boardConfig = getBoardConfiguration();
	m_boardConfig->Init();
}

void Game::onUpdate()
{
	processInput();
	m_camera.Update();

	if (m_judger.HasGameEnded())
	{
		onRoundEnded(m_judger);
		m_judger.InitGame(getPlayer(IState::Winner::FirstPlayer), getPlayer(IState::Winner::SecondPlayer), getStartingState());
	}
	else if (m_bUpdate && getTime() - m_lastTurnTime > m_delayNextTurn)
	{
		if (m_judger.PlayTurn())
		{
			m_boardConfig->Update(m_judger.GetCrtState());
		}

		m_lastTurnTime = getTime();
	}
}

void Game::onRender()
{
	getBoard()->Render(m_camera);
	m_boardConfig->Render(m_camera);
}

void Game::onDestroy()
{
}

void Game::processInput()
{
	const float m_camera_SPEED = 5.0f;
	const float SCALE_SPEED = 0.1f;

	if (m_inputManager.IsKeyDown(Engine::Key::W))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(0.0f, m_camera_SPEED));
	if (m_inputManager.IsKeyDown(Engine::Key::S))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(0.0f, -m_camera_SPEED));
	if (m_inputManager.IsKeyDown(Engine::Key::A))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(-m_camera_SPEED, 0.0f));
	if (m_inputManager.IsKeyDown(Engine::Key::D))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(m_camera_SPEED, -0.0f));
	if (m_inputManager.IsKeyDown(Engine::Key::Q))
		m_camera.SetScale(m_camera.GetScale() + SCALE_SPEED);
	if (m_inputManager.IsKeyDown(Engine::Key::E))
		m_camera.SetScale(m_camera.GetScale() - SCALE_SPEED);

	const uint kDelayIncrement = 25;
	if (m_inputManager.IsKeyDownOnce(Engine::Key::NumpadPlus))
		m_delayNextTurn += kDelayIncrement;
	if (m_inputManager.IsKeyDownOnce(Engine::Key::NumpadMinus))
		if (m_delayNextTurn > kDelayIncrement)
			m_delayNextTurn -= kDelayIncrement;
		else
			m_delayNextTurn = 0;

	if (m_inputManager.IsKeyDownOnce(Engine::Key::Space))
		m_bUpdate = !m_bUpdate;

	if (m_inputManager.IsKeyDownOnce(Engine::Key::LeftMouseButton))
	{
		glm::vec2 mouseCoords = m_inputManager.GetMouseCoords();
		mouseCoords = m_camera.ConvertScreenToWorld(mouseCoords);
		Engine::log("At scale %.2fx coords are: %.2f, %.2f", m_camera.GetScale(), mouseCoords.x, mouseCoords.y);

		// get clicked box

		AbstractHumanPlayer *pPlayer = dynamic_cast<AbstractHumanPlayer*>(m_judger.GetCrtPlayer().get());
		if (pPlayer)
		{
			Position pos = m_boardConfig->GetTilePosition(mouseCoords);
			if(!pos.Invalid())
				pPlayer->BufferAction(pos);
		}
	}
}
