#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "ActorManager.h"
#include "GameApp.h"
#include "GameAssert.h"
#include "GameLog.h"
#include "MainPhase.h"
#include "PhaseManager.h"
#include "TitlePhase.h"

GameApp* GameApp::_gameAppPtr = nullptr;

GameApp::~GameApp()
{
	if (_isInitialized)
	{
		Shutdown();
	}
}

void GameApp::Startup()
{
	if (_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_STARTUP_GAMEAPP");
		return;
	}

    _gameConfig = std::make_unique<GameConfig>();
    _gameConfig->Load("Config/GameConfig.yaml");

    InitWindow(_gameConfig->GetWindowWidth(), _gameConfig->GetWindowHeight(), _gameConfig->GetWindowTitle().c_str());
    SetTargetFPS(_gameConfig->GetFPS());
    
    ActorManager::Get().Startup();
    PhaseManager::Get().Startup();

    TitlePhase* titlePhase = PhaseManager::Get().Create<TitlePhase>();
    PhaseManager::Get().Register("TitlePhase", titlePhase);

    MainPhase* mainPhase = PhaseManager::Get().Create<MainPhase>();
    PhaseManager::Get().Register("MainPhase", mainPhase);

    _entryPhase = titlePhase;
    _gameAppPtr = this;
    _isDoneLoop = false;
	_isInitialized = true;
}

void GameApp::Run()
{
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

void GameApp::Shutdown()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_SHUTDOWN_OR_FAILED_TO_STARTUP_GAMEAPP");
		return;
	}

    PhaseManager::Get().Unregister("MainPhase");
    PhaseManager::Get().Unregister("TitlePhase");

    PhaseManager::Get().Shutdown();
    ActorManager::Get().Shutdown();
    CloseWindow();

    _gameAppPtr = nullptr;
	_isInitialized = false;
}

void GameApp::ProcessPhaseActionState()
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