#include "ActorManager.h"
#include "GameApp.h"
#include "GameLog.h"
#include "MainPhase.h"
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
    
    _gameAppPtr = this;
	_isInitialized = true;
}

void GameApp::Run()
{
    std::unique_ptr<TitlePhase> titlePhase = std::make_unique<TitlePhase>();
    std::unique_ptr<MainPhase> mainPhase = std::make_unique<MainPhase>();

    IPhase* currentPhase = titlePhase.get();
    currentPhase->Enter();
    while (!WindowShouldClose())
    {
        float deltaSeconds = GetFrameTime();
        currentPhase->Tick(deltaSeconds);
        currentPhase->Render();

        if (currentPhase->GetActionState() == IPhase::EActionState::EXIT)
        {
            currentPhase->Exit();
            currentPhase = currentPhase->GetTransitionPhase();
            currentPhase->Enter();
        }
    }
}

void GameApp::Shutdown()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_SHUTDOWN_OR_FAILED_TO_STARTUP_GAMEAPP");
		return;
	}

    ActorManager::Get().Shutdown();
    CloseWindow();

    _gameAppPtr = nullptr;
	_isInitialized = false;
}