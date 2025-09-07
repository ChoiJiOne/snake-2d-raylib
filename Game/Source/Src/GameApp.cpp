#include "ActorManager.h"
#include "GameApp.h"
#include "GameAssert.h"
#include "GameLog.h"
#include "MainPhase.h"
#include "PhaseManager.h"
#include "TitlePhase.h"

void GameApp::Startup()
{
    _contentConfig = std::make_unique<ContentConfig>();
    _contentConfig->TryLoadConfigFile("Config/ContentConfig.yaml");

    Application::SetConfig(_contentConfig.get());
    Application::Startup();

    if (_isInitialized)
    {
        GAME_LOG_ERR("ALREADY_STARTUP_GAME_APP");
        return;
    }

    TitlePhase* titlePhase = PhaseManager::Get().Create<TitlePhase>();
    PhaseManager::Get().Register("TitlePhase", titlePhase);

    MainPhase* mainPhase = PhaseManager::Get().Create<MainPhase>();
    PhaseManager::Get().Register("MainPhase", mainPhase);

    SetEntryPhase(titlePhase);

    _isInitialized = true;
}

void GameApp::Shutdown()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_SHUTDOWN_OR_FAILED_TO_STARTUP_GAME_APP");
		return;
	}

    PhaseManager::Get().Unregister("MainPhase");
    PhaseManager::Get().Unregister("TitlePhase");

    Application::Shutdown();
}