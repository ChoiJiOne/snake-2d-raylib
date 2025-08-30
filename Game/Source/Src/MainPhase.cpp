#include "ActorManager.h"
#include "Board.h"
#include "Food.h"
#include "GameApp.h"
#include "GameLog.h"
#include "MainPhase.h"
#include "PhaseManager.h"
#include "Snake.h"

void MainPhase::Tick(float deltaSeconds)
{
    for (auto& tickActor : _tickActors)
    {
        tickActor->Tick(deltaSeconds);
    }

    if (_isOver)
    {
        _transitionPhase = PhaseManager::Get().GetRegisteredPhase("TitlePhase");
        SetActionState(EActionState::EXIT_PHASE);
    }

    // 윈도우 닫기 버튼 클릭 시...
    if (WindowShouldClose())
    {
        SetActionState(EActionState::QUIT_LOOP);
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

    Board* board = ActorManager::Get().Create<Board>(
        boardPosition, 
        app->GetConfig()->GetTileSize(), 
        app->GetConfig()->GetRowTileCount(), 
        app->GetConfig()->GetColTileCount()
    );
    Snake* snake = ActorManager::Get().Create<Snake>(
        board, 
        app->GetConfig()->GetStartBodyCount(), 
        static_cast<EDirection>(app->GetConfig()->GetStartDirection())
    );
    Food* food = ActorManager::Get().Create<Food>(board);

    _phaseActors = { board, snake, food, };
    _tickActors = { snake, food, board, };
    _renderActors = { board, snake, food, };
}

void MainPhase::Exit()
{
    for (auto& actor : _phaseActors)
    {
        ActorManager::Get().Destroy(actor);
    }

    _isOver = false;
    _actionState = EActionState::NONE;
}

IPhase* MainPhase::GetTransitionPhase()
{
	return _transitionPhase;
}
