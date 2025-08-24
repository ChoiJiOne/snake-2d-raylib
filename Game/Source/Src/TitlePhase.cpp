#include "ActorManager.h"
#include "Button.h"
#include "GameLog.h"
#include "TitlePhase.h"

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
    Button* startButton = ActorManager::Get().Create<Button>(
        Vector2{ 400.0f, 400.0f },
        Vector2{ 200.0f, 50.0f  },
        [&]() { GAME_LOG_INFO("PRESS_START_BUTTON!"); },
        "START"
    );

    _tickActors =
    {
        startButton,
    };

    _renderActors =
    {
        startButton,
    };
}

void TitlePhase::Exit()
{
}

IPhase* TitlePhase::GetTransitionPhase()
{
	return nullptr;
}
