#include <vector>

#include <raylib.h>

#include "GameAssert.h"

const int screenWidth = 800;
const int screenHeight = 800;

Vector2 center{ static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f };
float size = 20.0f;
int count = 20;

enum class EState
{
    NONE = 0x00,
    BODY = 0x01,
    FOOD = 0x02,
};

struct Body
{
    int offsetX;
    int offsetY;
};

struct Food
{
    int offsetX;
    int offsetY;
};

struct Snake
{
    std::vector<Body> bodys;
};

struct Tile
{
    Vector2 position;
    Vector2 size;
    EState state;
    int index;
};

std::vector<Tile> board;
Snake snake;
Food food;

void DrawTileMapGrid()
{
    float x = center.x - size * count * 0.5f;
    float y = center.y - size * count * 0.5f;

    for (int yi = 0; yi <= count; ++yi)
    {
        DrawLineV(Vector2{ x, y + yi * size }, Vector2{ x + size * count, y + yi * size }, BLACK);
    }

    for (int xi = 0; xi <= count; ++xi)
    {
        DrawLineV(Vector2{ x + xi * size, y }, Vector2{ x + xi * size, y + size * count }, BLACK);
    }
}

void DrawTileMap()
{
    for (const auto& tile : board)
    {
        if (tile.state == EState::BODY)
        {
            DrawRectangleV(tile.position, tile.size, RED);
        }
        else if (tile.state == EState::FOOD)
        {
            DrawRectangleV(tile.position, tile.size, BLUE);
        }
    }
}

int main(int argc, char* argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);

    float x = center.x - size * count * 0.5f;
    float y = center.y - size * count * 0.5f;
    for (int yi = 0; yi < count; ++yi)
    {
        for (int xi = 0; xi < count; ++xi)
        {
            Tile tile;
            tile.position = Vector2{ x + xi * size, y + yi * size };
            tile.size = Vector2{ size, size };
            tile.state = EState::NONE;
            tile.index = xi + yi * count;
            board.emplace_back(tile);
        }
    }

    Body body0;
    body0.offsetX = count / 2;
    body0.offsetY = count / 2;
    Body body1;
    body1.offsetX = body0.offsetX - 1;
    body1.offsetY = body0.offsetY;
    Body body2;
    body2.offsetX = body1.offsetX - 1;
    body2.offsetY = body1.offsetY;
    Body body3;
    body3.offsetX = body2.offsetX - 1;
    body3.offsetY = body2.offsetY;
    snake.bodys.emplace_back(body0);
    snake.bodys.emplace_back(body1);
    snake.bodys.emplace_back(body2);
    snake.bodys.emplace_back(body3);

    for (const auto& body : snake.bodys)
    {
        int offset = body.offsetX + body.offsetY * count; 
        board[offset].state = EState::BODY;
    }

    while (true)
    {
        food.offsetX = GetRandomValue(0, count - 1);
        food.offsetY = GetRandomValue(0, count - 1);

        int offset = food.offsetX + food.offsetY * count;
        if (board[offset].state == EState::NONE)
        {
            board[offset].state = EState::FOOD;
            break;
        }
    }
    
    while (!WindowShouldClose()) 
    {
        bool canMove = false;
        const Body& head = snake.bodys.front();
        int newOffsetX = head.offsetX;
        int newOffsetY = head.offsetY;
        if (IsKeyPressed(KEY_RIGHT))
        {
            newOffsetX = (newOffsetX + 1) % count;
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            if (newOffsetX <= 0)
            {
                newOffsetX = count - 1;
            }
            else
            {
                newOffsetX = newOffsetX - 1;
            }
        }
        if (IsKeyPressed(KEY_UP))
        {
            if (newOffsetY <= 0)
            {
                newOffsetY = count - 1;
            }
            else
            {
                newOffsetY = newOffsetY - 1;
            }
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            newOffsetY = (newOffsetY + 1) % count;
        }

        int offset = newOffsetX + newOffsetY * count;
        bool isEatFood = false;
        if (board[offset].state != EState::BODY)
        {
            canMove = true;
            if (board[offset].state == EState::FOOD)
            {
                isEatFood = true;
                snake.bodys.emplace_back(snake.bodys.back());
            }
        }
        
        if (canMove)
        {
            for (const auto& body : snake.bodys)
            {
                int offset = body.offsetX + body.offsetY * count;
                board[offset].state = EState::NONE;
            }

            size_t startIdx = snake.bodys.size() - 1;
            if (isEatFood)
            {
                startIdx = snake.bodys.size() - 2;
            }

            for (size_t idx = startIdx; idx >= 1; --idx)
            {
                snake.bodys[idx].offsetX = snake.bodys[idx - 1].offsetX;
                snake.bodys[idx].offsetY = snake.bodys[idx - 1].offsetY;
            }

            snake.bodys[0].offsetX = newOffsetX;
            snake.bodys[0].offsetY = newOffsetY;

            for (const auto& body : snake.bodys)
            {
                int offset = body.offsetX + body.offsetY * count;
                board[offset].state = EState::BODY;
            }
        }

        if (isEatFood)
        {
            while (true)
            {
                food.offsetX = GetRandomValue(0, count - 1);
                food.offsetY = GetRandomValue(0, count - 1);

                int offset = food.offsetX + food.offsetY * count;
                if (board[offset].state == EState::NONE)
                {
                    board[offset].state = EState::FOOD;
                    break;
                }
            }
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            DrawTileMap();
            DrawTileMapGrid();
        }
        EndDrawing();
    }

    CloseWindow();
	return 0;
}