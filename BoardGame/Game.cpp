#include "Game.h"
#include "Engine/Engine.h"
#include "Engine/GLSLProgram.h"
#include "Engine/SpriteBatch.h"
#include "Engine/SpriteFont.h"

#include "IRenderable.h"
#include "AbstractHumanPlayer.h"
#include "IBoardConfiguration.h"
#include "State.h"

Game::Game() : IGame()
{
}

Game::~Game()
{

}

void Game::onInit()
{
	m_fontSpriteBatchPtr = std::make_unique<Engine::SpriteBatch>();
	m_fontSpriteBatchPtr->Init();

	uint kFontRealSize = 64;
	m_defaultFontScale = { 2.f / m_screenSize.width, 1.5f / m_screenSize.height };
	m_fontsSpritePtr = std::make_unique<Engine::SpriteFont>("../Resources/Fonts/fast99.ttf", kFontRealSize);

	m_textureProgramPtr = std::make_shared<Engine::GLSLProgram>();
	m_textureProgramPtr->CompileShaders("../Resources/../Resources/Shaders/textureShading.vert", "../Resources/../Resources/Shaders/textureShading.frag");
	m_textureProgramPtr->AddAttribute("vertexPosition");
	m_textureProgramPtr->AddAttribute("vertexColor");
	m_textureProgramPtr->AddAttribute("vertexUV");
	m_textureProgramPtr->LinkShader();

	m_hudCamera.SetScale({ m_screenSize.width/2, m_screenSize.height/2 });
	m_hudCamera.Init(m_screenSize.width, m_screenSize.height);


	onInitRendering();

	using namespace std::placeholders;

 	m_inputManager.Register(Engine::EventType::ButtonDown, std::bind(&Game::onKeyDown, this, _1));
 	m_inputManager.Register(Engine::EventType::ButtonUp, std::bind(&Game::onKeyUp, this, _1));
 	m_inputManager.Register(Engine::EventType::MouseMotion, std::bind(&Game::onMouseMove, this, _1));

	m_camera.Init(m_screenSize.width, m_screenSize.height);
	m_judger.InitGame(getPlayer(IState::Winner::FirstPlayer), getPlayer(IState::Winner::SecondPlayer), getStartingState());
	m_boardConfig = getBoardConfiguration();
	m_boardConfig->Init();
}

void Game::onUpdate()
{
	processInput();
	m_camera.Update();

	if (canUpdate() && getTime() - m_lastTurnTime > m_delayNextTurn)
	{
		if (m_judger.HasGameEnded())
		{
			m_judger.InitGame(getPlayer(IState::Winner::FirstPlayer), getPlayer(IState::Winner::SecondPlayer), getStartingState());
			m_boardConfig->Update(m_judger.GetCurrentState());
		}
		else
		{
			auto crtPlayer = m_judger.GetCurrentPlayer();
			auto crtState = m_judger.GetCurrentState();

			onTurnBegining(crtPlayer, crtState);
			if (m_judger.PlayTurn())
			{
				updateHud();
				m_boardConfig->Update(m_judger.GetCurrentState());
				onTurnEnding(crtPlayer, crtState);
			}

			if (m_judger.HasGameEnded())
			{
				// if a player is human, pause to show results
				if (dynamic_cast<AbstractHumanPlayer*>(getPlayer(IState::Winner::FirstPlayer).get()) ||
					dynamic_cast<AbstractHumanPlayer*>(getPlayer(IState::Winner::SecondPlayer).get()))
					m_bPausedAutomatically = true;

				onRoundEnded(m_judger);
			}
		}

		m_lastTurnTime = getTime();
	}
}

void Game::onRender()
{
	getBoard()->Render(m_camera);
	m_boardConfig->Render(m_camera);

	renderHud();
}

void Game::onDestroy()
{
}

void Game::processInput()
{
	const float kCameraSpeed = 5.0f;
	const float kScaleSpeed = 0.1f;

	if (m_inputManager.IsKeyDown(Engine::Key::W))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(0.0f, kCameraSpeed));
	if (m_inputManager.IsKeyDown(Engine::Key::S))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(0.0f, -kCameraSpeed));
	if (m_inputManager.IsKeyDown(Engine::Key::A))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(-kCameraSpeed, 0.0f));
	if (m_inputManager.IsKeyDown(Engine::Key::D))
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(kCameraSpeed, -0.0f));
	if (m_inputManager.IsKeyDown(Engine::Key::Q))
		m_camera.SetScale(m_camera.GetScale() + kScaleSpeed);
	if (m_inputManager.IsKeyDown(Engine::Key::E))
		m_camera.SetScale(m_camera.GetScale() - kScaleSpeed);
}

bool Game::canUpdate()
{
	return !m_bPausedManually && !m_bPausedAutomatically;
}

void Game::updateHud()
{
	char buffer[256];
	static int counter = 0;
	//sprintf(buffer, "Turn: %d", counter++);

	m_fontSpriteBatchPtr->Begin();
	m_fontsSpritePtr->Draw(*m_fontSpriteBatchPtr, m_windowTitle.c_str(), glm::vec2(0, 0.7), m_defaultFontScale, 0, Engine::ColorRGBA8(85, 107, 47,255), Engine::Justification::Middle);
	m_fontSpriteBatchPtr->End();
}

void Game::renderHud()
{
	m_textureProgramPtr->Use();
	m_textureProgramPtr->UploadUniform("mySampler", 0);
	m_textureProgramPtr->UploadUniform("P", m_hudCamera.GetCameraMatrix());

	m_fontSpriteBatchPtr->RenderBatches();

	m_textureProgramPtr->UnUse();
}

void Game::onKeyDown(void *pkey)
{
	Engine::Key key = *static_cast<Engine::Key*>(pkey);

	// any key can stop automatic pause
	if (m_bPausedAutomatically)
	{
		m_bPausedAutomatically = false;
		return;
	}

	const uint kDelayIncrement = 25;

	switch (key)
	{
	case Engine::Key::NumpadPlus:
		m_delayNextTurn += kDelayIncrement;
		break;
	case Engine::Key::NumpadMinus:
		if (m_delayNextTurn > kDelayIncrement)
			m_delayNextTurn -= kDelayIncrement;
		else
			m_delayNextTurn = 0;
		break;
	case Engine::Key::Space:
		m_bPausedManually = !m_bPausedManually;
		break;
	case Engine::Key::LeftMouseButton:
		glm::vec2 wordPos = m_camera.ConvertScreenToWorld(m_inputManager.GetMouseCoords());

		// get clicked box
		AbstractHumanPlayer *pPlayer = dynamic_cast<AbstractHumanPlayer*>(m_judger.GetCurrentPlayer().get());
		if (pPlayer)
			m_clickedTilePosIndex = m_boardConfig->GetTilePosition(wordPos);

		break;
	}
}

void Game::onKeyUp(void *pkey)
{
	Engine::Key key = *static_cast<Engine::Key*>(pkey);

	switch (key)
	{
	case Engine::Key::LeftMouseButton:
		glm::vec2 wordPos = m_camera.ConvertScreenToWorld(m_inputManager.GetMouseCoords());

		AbstractHumanPlayer *pPlayer = dynamic_cast<AbstractHumanPlayer*>(m_judger.GetCurrentPlayer().get());
		if (pPlayer)
		{
			Position releaseTilePosIndex = m_boardConfig->GetTilePosition(wordPos);
			if (!releaseTilePosIndex.Invalid())
			{
				if (m_clickedTilePosIndex == releaseTilePosIndex)
					pPlayer->BufferAction(releaseTilePosIndex);
				else
					if(!m_clickedTilePosIndex.Invalid())
						pPlayer->BufferAction(m_clickedTilePosIndex, releaseTilePosIndex);
			}

			m_clickedTilePosIndex = Position();
		}
	}
}

void Game::onMouseMove(void *pVec2i)
{
	glm::ivec2 &mouseCoord = *static_cast<glm::ivec2*>(pVec2i);
	glm::vec2 wordPos = m_camera.ConvertScreenToWorld(mouseCoord);
}
