#define RAYGUI_IMPLEMENTATION
#include <raylib.h>
#include <raygui.h>

#include "ActorManager.h"
#include "Application.h"
#include "GameAssert.h"
#include "GameLog.h"
#include "PhaseManager.h"

Application* Application::_app = nullptr;

Application::~Application()
{
	if (_isInitialized)
	{
		Shutdown();
	}
}

void Application::Startup()
{
	if (_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_STARTUP_APPLICATION");
		return;
	}

	_gameConfig = std::make_unique<GameConfig>();
	_gameConfig->Load("Config/GameConfig.yaml");

	InitWindow(_gameConfig->GetWindowWidth(), _gameConfig->GetWindowHeight(), _gameConfig->GetWindowTitle().c_str());
	SetTargetFPS(_gameConfig->GetFPS());
	GuiSetStyle(DEFAULT, TEXT_SIZE, _gameConfig->GetGuiFontSize());
	
	_actorMgr = ActorManager::Get().GetPtr();
	_phaseMgr = PhaseManager::Get().GetPtr();
	
	_actorMgr->Startup();
	_phaseMgr->Startup();

	_app = this;
	_isDoneLoop = false;
	_isInitialized = true;
}

void Application::Run()
{
	GAME_CHECK(_entryPhase != nullptr);

	_currentPhase = _entryPhase;
	_currentPhase->Enter();

	while (!_isDoneLoop)
	{
		float deltaSeconds = GetFrameTime();
		_currentPhase->Tick(deltaSeconds);
		_currentPhase->Render();

		ProcessPhaseActionState();
	}
}

void Application::Shutdown()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_SHUTDOWN_OR_FAILED_TO_STARTUP_GAMEAPP");
		return;
	}

	_phaseMgr->Shutdown();
	_actorMgr->Shutdown();
	CloseWindow();

	_app = nullptr;
	_isInitialized = false;
}

void Application::ProcessPhaseActionState()
{
	GAME_CHECK(_currentPhase != nullptr);

	switch (_currentPhase->GetActionState())
	{
	case IPhase::EActionState::EXIT_PHASE:
		_currentPhase->Exit();
		_currentPhase = _currentPhase->GetTransitionPhase();
		_currentPhase->Enter();
		break;

	case IPhase::EActionState::QUIT_LOOP:
		_currentPhase->Exit();
		_isDoneLoop = true;
		break;

	default:
		// 아무 것도 안함...
		break;
	}
}