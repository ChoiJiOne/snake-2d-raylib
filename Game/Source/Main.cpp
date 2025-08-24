#include <vector>

#include <raylib.h>

#include "ActorManager.h"
#include "Board.h"
#include "GameAssert.h"
#include "GameLog.h"

const int screenWidth = 800;
const int screenHeight = 800;

enum class EDirection
{
    NONE  = 0x00,
    LEFT  = 0x01,
    RIGHT = 0x02,
    UP    = 0x03,
    DOWN  = 0x04,
};

struct Snake
{
    std::vector<BoardCoord> bodys;
};

Snake snake;
BoardCoord food;

int main(int argc, char* argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);

    ActorManager::Get().Startup();

    Board* board = ActorManager::Get().Create<Board>(Vector2{ static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f }, 20.0f, 20, 20);

    BoardCoord body0;
    body0.offsetX = board->GetColTileCount() / 2;
    body0.offsetY = board->GetRowTileCount() / 2;
    BoardCoord body1;
    body1.offsetX = body0.offsetX - 1;
    body1.offsetY = body0.offsetY;
    BoardCoord body2;
    body2.offsetX = body1.offsetX - 1;
    body2.offsetY = body1.offsetY;
    BoardCoord body3;
    body3.offsetX = body2.offsetX - 1;
    body3.offsetY = body2.offsetY;
    snake.bodys.emplace_back(body0);
    snake.bodys.emplace_back(body1);
    snake.bodys.emplace_back(body2);
    snake.bodys.emplace_back(body3);

    for (const auto& body : snake.bodys)
    {
        board->SetTileState(body, ETileState::BODY);
    }

    while (true)
    {
        food.offsetX = GetRandomValue(0, board->GetColTileCount() - 1);
        food.offsetY = GetRandomValue(0, board->GetRowTileCount() - 1);

        if (board->GetTileState(food) == ETileState::NONE)
        {
            board->SetTileState(food, ETileState::FOOD);
            break;
        }
    }
    
    while (!WindowShouldClose()) 
    {
        EDirection direction = EDirection::NONE;
        bool canMove = false;
        const BoardCoord& head = snake.bodys.front();
        if (IsKeyPressed(KEY_RIGHT))
        {
            direction = EDirection::RIGHT;
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            direction = EDirection::LEFT;
        }
        if (IsKeyPressed(KEY_UP))
        {
            direction = EDirection::UP;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            direction = EDirection::DOWN;
        }

        int newOffsetX = head.offsetX;
        int newOffsetY = head.offsetY;
        switch (direction)
        {
        case EDirection::LEFT:
            newOffsetX = (newOffsetX <= 0) ? board->GetColTileCount() - 1 : newOffsetX - 1;
            break;

        case EDirection::RIGHT:
            newOffsetX = (newOffsetX + 1) % board->GetColTileCount();
            break;

        case EDirection::UP:
            newOffsetY = (newOffsetY <= 0) ? board->GetRowTileCount() - 1 : newOffsetY - 1;
            break;

        case EDirection::DOWN:
            newOffsetY = (newOffsetY + 1) % board->GetRowTileCount();
            break;

        default:
            break;
        }

        bool isEatFood = false;
        if (direction != EDirection::NONE && board->GetTileState(BoardCoord{ newOffsetX, newOffsetY }) != ETileState::BODY)
        {
            canMove = true;
            if (board->GetTileState(BoardCoord{ newOffsetX, newOffsetY }) == ETileState::FOOD)
            {
                isEatFood = true;
                snake.bodys.emplace_back(snake.bodys.back());
            }
        }
        
        if (canMove)
        {
            for (const auto& body : snake.bodys)
            {
                board->SetTileState(body, ETileState::NONE);
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
                board->SetTileState(body, ETileState::BODY);
            }
        }

        if (isEatFood)
        {
            while (true)
            {
                food.offsetX = GetRandomValue(0, board->GetColTileCount() - 1);
                food.offsetY = GetRandomValue(0, board->GetRowTileCount() - 1);

                if (board->GetTileState(food) == ETileState::NONE)
                {
                    board->SetTileState(food, ETileState::FOOD);
                    break;
                }
            }
        }

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