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

	_isInitialized = true;
}

void GameApp::Run()
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);

    ActorManager::Get().Startup();

    Board* board = ActorManager::Get().Create<Board>(Vector2{ static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f }, 20.0f, 20, 20);
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

    ActorManager::Get().Shutdown();
    CloseWindow();
}

void GameApp::Shutdown()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_SHUTDOWN_OR_FAILED_TO_STARTUP_GAMEAPP");
		return;
	}

	_isInitialized = false;
}