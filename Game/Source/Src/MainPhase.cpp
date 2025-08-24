#include "ActorManager.h"
#include "Board.h"
#include "Food.h"
#include "GameApp.h"
#include "GameLog.h"
#include "MainPhase.h"
#include "Snake.h"

void MainPhase::Tick(float deltaSeconds)
{
    for (auto& tickActor : _tickActors)
    {
        tickActor->Tick(deltaSeconds);
    }
}

void MainPhase::Render()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        for (auto& renderActor : _renderActors)
        {
            renderActor->Render();
        }
    }
    EndDrawing();
}

void MainPhase::Enter()
{
    GameApp* app = GameApp::GetAppPtr();

    Vector2 boardPosition = Vector2{ 
        static_cast<float>(app->GetConfig()->GetWindowWidth())  / 2.0f, 
        static_cast<float>(app->GetConfig()->GetWindowHeight()) / 2.0f
    };

    Board* board = ActorManager::Get().Create<Board>(boardPosition, app->GetConfig()->GetTileSize(), app->GetConfig()->GetRowTileCount(), app->GetConfig()->GetColTileCount());
    Snake* snake = ActorManager::Get().Create<Snake>(board, app->GetConfig()->GetStartBodyCount(), static_cast<EDirection>(app->GetConfig()->GetStartDirection()));
    Food* food = ActorManager::Get().Create<Food>(board);

    _tickActors = 
    {
        snake,
        food,
        board,
    };

    _renderActors =
    {
        board,
        snake,
        food,
    };
}

void MainPhase::Exit()
{
}

IPhase* MainPhase::GetTransitionPhase()
{
	return nullptr;
}
