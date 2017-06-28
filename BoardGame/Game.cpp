#include "Game.h"

#include <fstream>

#include "Engine/Engine.h"
#include "Engine/GLSLProgram.h"
#include "Engine/SpriteBatch.h"
#include "Engine/SpriteFont.h"

#include "Graph.h"
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
	initShaders();
	loadSettings();

	m_hudFontSpriteBatchPtr = std::make_unique<Engine::SpriteBatch>();
	m_hudFontSpriteBatchPtr->Init();

	uint kFontRealSize = 64;
	m_defaultFontScale = { 2.f / m_screenSize.width, 1.5f / m_screenSize.height };
	m_fontsSpritePtr = std::make_unique<Engine::SpriteFont>("../Resources/Fonts/fast99.ttf", kFontRealSize);

	m_hudCamera.SetScale({ m_screenSize.width/2, m_screenSize.height/2 });
	m_hudCamera.Init(m_screenSize.width, m_screenSize.height);

	m_graph = std::make_unique<Graph>();
	m_graph->Init(m_simpleProgramPtr);

	if (m_bShowGraph)
		m_camera.SetPosition({ -m_screenSize.width / 4.f,0 });
	else
		m_camera.SetPosition({});

	updateHud();

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
				m_boardConfig->Update(m_judger.GetCurrentState());
				onTurnEnding(crtPlayer, crtState);
			}

			if (m_judger.HasGameEnded())
			{
				// if a player is human, pause to show results
				if (dynamic_cast<AbstractHumanPlayer*>(getPlayer(IState::Winner::FirstPlayer).get()) ||
					dynamic_cast<AbstractHumanPlayer*>(getPlayer(IState::Winner::SecondPlayer).get()))
					m_bPausedAutomatically = true;

				if (m_judger.GetWinner() == IState::Winner::FirstPlayer)
					++m_gamesWon;
				else if(m_judger.GetWinner() == IState::Winner::SecondPlayer)
					++m_gamesLost;

				++m_gamesTotal;

				m_winingsStats.emplace_back(float(m_gamesWon) / m_gamesTotal);
				m_loosingStats.emplace_back(float(m_gamesLost) / m_gamesTotal);
				updateHud();

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

void Game::initShaders()
{
	m_simpleProgramPtr = std::make_shared<Engine::GLSLProgram>();
	m_simpleProgramPtr->CompileShaders("../Resources/Shaders/simpleShading.vert", "../Resources/Shaders/simpleShading.frag");
	m_simpleProgramPtr->AddAttribute("vertexPosition");
	m_simpleProgramPtr->LinkShader();

	m_textureProgramPtr = std::make_shared<Engine::GLSLProgram>();
	m_textureProgramPtr->CompileShaders("../Resources/../Resources/Shaders/textureShading.vert", "../Resources/../Resources/Shaders/textureShading.frag");
	m_textureProgramPtr->AddAttribute("vertexPosition");
	m_textureProgramPtr->AddAttribute("vertexColor");
	m_textureProgramPtr->AddAttribute("vertexUV");
	m_textureProgramPtr->LinkShader();
}

void Game::loadSettings()
{
	std::ifstream in("settings.ini");
	char key[100], equal[100], value[100];
	while (!in.fail() && !in.eof())
	{
		in >> key >> equal >> value;
		if (strlen(key) == 0 || strchr(key, '#') || equal[0] != '=')
			continue;
		m_settings[key] = value;
	}
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

	m_hudFontSpriteBatchPtr->Begin();
	m_fontsSpritePtr->Draw(*m_hudFontSpriteBatchPtr, m_windowTitle.c_str(), glm::vec2(0, 0.7f), m_defaultFontScale, 0.f, Engine::ColorRGBA8(85, 47, 107), Engine::Justification::Middle);
	glm::vec2 scorePos(0, m_scoreHeight);
	if (m_bShowGraph)
	{
		scorePos.x = 0.5;
		sprintf(buffer, "%d%%", 100);
		m_fontsSpritePtr->Draw(*m_hudFontSpriteBatchPtr, buffer, glm::vec2(-0.8, 0.45f), m_defaultFontScale * 0.5f, 0.f, Engine::ColorRGBA8(0, 0, 0), Engine::Justification::Middle);
		sprintf(buffer, "%d", 0);
		m_fontsSpritePtr->Draw(*m_hudFontSpriteBatchPtr, buffer, glm::vec2(-0.8, -0.55f), m_defaultFontScale * 0.5f, 0.f, Engine::ColorRGBA8(0, 0, 0), Engine::Justification::Right);
		sprintf(buffer, "%d", m_gamesTotal);
		m_fontsSpritePtr->Draw(*m_hudFontSpriteBatchPtr, buffer, glm::vec2(-0.075, -0.55f), m_defaultFontScale * 0.5f, 0.f, Engine::ColorRGBA8(0, 0, 0), Engine::Justification::Right);
	}
	sprintf(buffer, "%d : %d", m_gamesWon, m_gamesLost);
	m_fontsSpritePtr->Draw(*m_hudFontSpriteBatchPtr, buffer, scorePos, m_defaultFontScale, 0.f, Engine::ColorRGBA8(255, 127, 80), Engine::Justification::Middle);
	m_hudFontSpriteBatchPtr->End();

	if(m_bShowGraph)
		m_graph->SetValues(m_winingsStats, m_loosingStats);
}

void Game::renderHud()
{
	m_textureProgramPtr->Use();
	m_textureProgramPtr->UploadUniform("mySampler", 0);
	m_textureProgramPtr->UploadUniform("P", m_hudCamera.GetCameraMatrix());
	m_hudFontSpriteBatchPtr->RenderBatches();
	m_textureProgramPtr->UnUse();

	if (m_bShowGraph)
		m_graph->Render(m_hudCamera);
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
	{
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
					if (!m_clickedTilePosIndex.Invalid())
						pPlayer->BufferAction(m_clickedTilePosIndex, releaseTilePosIndex);
			}

			m_clickedTilePosIndex = Position();
		}
	}
		break;
	case Engine::Key::F2:
		m_bShowGraph = !m_bShowGraph;
		if (m_bShowGraph)
			m_camera.SetPosition({ -m_screenSize.width / 4.f,0 });
		else
			m_camera.SetPosition({});
		updateHud();
		break;
	}
}

void Game::onMouseMove(void *pVec2i)
{
	glm::ivec2 &mouseCoord = *static_cast<glm::ivec2*>(pVec2i);
	glm::vec2 wordPos = m_camera.ConvertScreenToWorld(mouseCoord);
}
