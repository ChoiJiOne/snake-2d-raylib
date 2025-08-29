#include "ActorManager.h"
#include "Button.h"
#include "GameAssert.h"
#include "GameLog.h"
#include "PhaseManager.h"
#include "Text.h"
#include "TitlePhase.h"

#include <raygui.h>

void TitlePhase::Tick(float deltaSeconds)
{
    for (auto& tickActor : _tickActors)
    {
        tickActor->Tick(deltaSeconds);
    }

    // ������ �ݱ� ��ư Ŭ�� ��...
    if (WindowShouldClose())
    {
        SetActionState(EActionState::QUIT_LOOP);
    }
}

void TitlePhase::Render()
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

void TitlePhase::Enter()
{
    if (!_isActorsLoaded)
    {
        LoadActors();
    }
}

void TitlePhase::Exit()
{
    _actionState = EActionState::NONE;
}

IPhase* TitlePhase::GetTransitionPhase()
{
	return _transitionPhase;
}

void TitlePhase::LoadActors()
{
    GAME_CHECK(!_isActorsLoaded);

    // ����, UI ��ġ �� ũ�� �ϵ��ڵ��� �� �ܺη� ���� �� ����غ���.
    Text* titleText = ActorManager::Get().Create<Text>(
        Vector2{ 300.0f, 200.0f },
        "SNAKE",
        100,
        GRAY
    );

    Button* startButton = ActorManager::Get().Create<Button>(
        Vector2{ 300.0f, 350.0f },
        Vector2{ 200.0f, 50.0f },
        [&]() 
        {
            _transitionPhase = PhaseManager::Get().GetRegisteredPhase("MainPhase");
            SetActionState(EActionState::EXIT_PHASE);
        },
        "START"
    );

    Button* quitButton = ActorManager::Get().Create<Button>(
        Vector2{ 300.0f, 430.0f },
        Vector2{ 200.0f, 50.0f },
        [&]() 
        {
            SetActionState(EActionState::QUIT_LOOP);
        },
        "QUIT"
    );

    _tickActors =
    {
        startButton,
        quitButton,
        titleText,
    };

    _renderActors =
    {
        titleText,
        startButton,
        quitButton,
    };

    _isActorsLoaded = true;
}
