#include <vector>

#include <raylib.h>

#include "ActorManager.h"
#include "Board.h"
#include "Food.h"
#include "GameAssert.h"
#include "Snake.h"

const int screenWidth = 800;
const int screenHeight = 800;

BoardCoord food;

int main(int argc, char* argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);

    ActorManager::Get().Startup();

    Board* board = ActorManager::Get().Create<Board>(Vector2{ static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f }, 20.0f, 20, 20);
    Snake* snake = ActorManager::Get().Create<Snake>(board, 4, EDirection::RIGHT);
    Food* food = ActorManager::Get().Create<Food>(board);

    while (!WindowShouldClose()) 
    {
        snake->Tick(0.0f);
        food->Tick(0.0f);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            board->Render();
        }
        EndDrawing();
    }

    ActorManager::Get().Shutdown();
    CloseWindow();
	return 0;
}