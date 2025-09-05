#include "ActorManager.h"
#include "Board.h"
#include "Food.h"
#include "TextEffect.h"
#include "GameApp.h"
#include "GameLog.h"
#include "GameStatText.h"
#include "GameInfoPopup.h"
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

    Vector2 scorePosition = Vector2{
    static_cast<float>(app->GetConfig()->GetWindowWidth()) / 3.0f,
    static_cast<float>(app->GetConfig()->GetWindowHeight()) / 20.0f
    };
    GameStatText* score = ActorManager::Get().Create<GameStatText>("SCORE: %d", 0, scorePosition, 30, GRAY, GREEN, 0.2f, 1.0f);

    Vector2 levelPosition = Vector2{
        2.0f * static_cast<float>(app->GetConfig()->GetWindowWidth()) / 3.0f,
        static_cast<float>(app->GetConfig()->GetWindowHeight()) / 20.0f
    };
    GameStatText* level = ActorManager::Get().Create<GameStatText>("LEVEL: %d", 1, levelPosition, 30, GRAY, GREEN, 0.2f, 1.0f);

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

    TextEffect* levelEffect = ActorManager::Get().Create<TextEffect>("LEVEL UP!", 35, GRAY, RED, 0.1f, 2.0f, 0.0f);
    Snake* snake = ActorManager::Get().Create<Snake>(
        board,
        levelEffect,
        score,
        level,
        static_cast<EDirection>(app->GetConfig()->GetStartDirection())
    );

    TextEffect* eatEffect = ActorManager::Get().Create<TextEffect>("EAT!", 20, GRAY, GREEN, 0.1f, 0.5f, 20.0f);
    Food* food = ActorManager::Get().Create<Food>(board, eatEffect);

    Vector2 popupPosition = Vector2{
        static_cast<float>(app->GetConfig()->GetWindowWidth()) / 2.0f,
        static_cast<float>(app->GetConfig()->GetWindowHeight()) / 2.0f
    };
    GameInfoPopup* gameInfoPopup = ActorManager::Get().Create<GameInfoPopup>(
        popupPosition, Vector2{300.0f, 200.0f}, snake
    );

    _phaseActors = { board, snake, food, eatEffect, levelEffect, score, level, gameInfoPopup };
    _tickActors = { snake, food, board, eatEffect, levelEffect, score, level, gameInfoPopup };
    _renderActors = { board, snake, food, eatEffect, levelEffect, score, level, gameInfoPopup };
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
