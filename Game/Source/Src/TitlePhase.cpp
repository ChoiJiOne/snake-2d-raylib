#include "ActorManager.h"
#include "Button.h"
#include "GameLog.h"
#include "TitlePhase.h"

#include <raygui.h>

void TitlePhase::Tick(float deltaSeconds)
{
    for (auto& tickActor : _tickActors)
    {
        tickActor->Tick(deltaSeconds);
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
    // 여기, UI 위치 및 크기 하드코딩된 거 외부로 빼는 것 고려해보기.
    Button* startButton = ActorManager::Get().Create<Button>(
        Vector2{ 300.0f, 400.0f },
        Vector2{ 200.0f, 50.0f  },
        [&]() { GAME_LOG_INFO("PRESS_START_BUTTON!"); },
        "START"
    );

    Button* quitButton = ActorManager::Get().Create<Button>(
        Vector2{ 300.0f, 480.0f },
        Vector2{ 200.0f, 50.0f },
        [&]() { GAME_LOG_INFO("PRESS_QUIT_BUTTON!"); },
        "QUIT"
    );

    _tickActors =
    {
        startButton,
        quitButton,
    };

    _renderActors =
    {
        startButton,
        quitButton,
    };
}

void TitlePhase::Exit()
{
}

IPhase* TitlePhase::GetTransitionPhase()
{
	return nullptr;
}
