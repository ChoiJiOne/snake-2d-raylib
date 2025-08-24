#include <raylib.h>

#include "TitlePhase.h"

void TitlePhase::Tick(float deltaSeconds)
{
}

void TitlePhase::Render()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
    }
    EndDrawing();
}

void TitlePhase::Enter()
{
}

void TitlePhase::Exit()
{
}

IPhase* TitlePhase::GetTransitionPhase()
{
	return nullptr;
}
