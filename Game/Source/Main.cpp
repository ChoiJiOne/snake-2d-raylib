#include <iostream>

#include <raylib.h>

int main(int argc, char* argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Snake");

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();
	return 0;
}