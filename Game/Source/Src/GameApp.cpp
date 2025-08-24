#include "ActorManager.h"
#include "Board.h"
#include "Food.h"
#include "GameApp.h"
#include "GameLog.h"
#include "Snake.h"

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
    
	_isInitialized = true;
}

void GameApp::Run()
{
    Board* board = ActorManager::Get().Create<Board>(
        Vector2{ static_cast<float>(_gameConfig->GetWindowWidth()) / 2.0f, static_cast<float>(_gameConfig->GetWindowHeight()) / 2.0f },
        20.0f, 
        20, 
        20
    );
    Snake* snake = ActorManager::Get().Create<Snake>(board, 4, EDirection::RIGHT);
    Food* food = ActorManager::Get().Create<Food>(board);

    std::vector<IActor*> updateActors =
    {
        snake,
        food,
        board,
    };

    std::vector<IActor*> renderActors =
    {
        board,
        snake,
        food,
    };

    while (!WindowShouldClose())
    {
        float deltaSeconds = GetFrameTime();
        for (auto& updateActor : updateActors)
        {
            updateActor->Tick(deltaSeconds);
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            for (auto& renderActor : renderActors)
            {
                renderActor->Render();
            }
        }
        EndDrawing();
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

	_isInitialized = false;
}