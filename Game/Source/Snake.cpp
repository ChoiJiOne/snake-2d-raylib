#include "GameAssert.h"
#include "GameLog.h"
#include "Snake.h"

Snake::Snake(Board* board, int32_t startBodyCount, const EDirection& startDirection)
{
	GAME_CHECK(board != nullptr);
	_board = board;
	_startBodyCount = std::max(MIN_START_BODY_COUNT, startBodyCount);

	if (startDirection == EDirection::NONE)
	{
		EDirection newStartDirection = static_cast<EDirection>(GetRandomValue(static_cast<int32_t>(EDirection::LEFT), static_cast<int32_t>(EDirection::DOWN)));
		_startDirection = newStartDirection;
	}
	else
	{
		_startDirection = startDirection;
	}

	_startDirectionBias =
	{
		{ EDirection::LEFT,  BoardCoord{ +1,  0 } },
		{ EDirection::RIGHT, BoardCoord{ -1,  0 } },
		{ EDirection::UP,    BoardCoord{  0, +1 } },
		{ EDirection::DOWN,  BoardCoord{  0, -1 } },
	};

	_bodys = CreateBodys();
	SetBodyOnBoard(ETileState::BODY);

	_isInitialized = true;
}

Snake::~Snake()
{
	if (_isInitialized)
	{
		Release();
	}
}

void Snake::Tick(float deltaSeconds)
{
    EDirection direction = EDirection::NONE;
    bool canMove = false;

    const BoardCoord& head = _bodys.front();
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
        newOffsetX = (newOffsetX <= 0) ? _board->GetColTileCount() - 1 : newOffsetX - 1;
        break;

    case EDirection::RIGHT:
        newOffsetX = (newOffsetX + 1) % _board->GetColTileCount();
        break;

    case EDirection::UP:
        newOffsetY = (newOffsetY <= 0) ? _board->GetRowTileCount() - 1 : newOffsetY - 1;
        break;

    case EDirection::DOWN:
        newOffsetY = (newOffsetY + 1) % _board->GetRowTileCount();
        break;

    default:
        break;
    }

    bool isEatFood = false;
    if (direction != EDirection::NONE && _board->GetTileState(BoardCoord{ newOffsetX, newOffsetY }) != ETileState::BODY)
    {
        canMove = true;
        if (_board->GetTileState(BoardCoord{ newOffsetX, newOffsetY }) == ETileState::FOOD)
        {
            isEatFood = true;
            _bodys.emplace_back(_bodys.back());
        }
    }

    if (canMove)
    {
        for (const auto& body : _bodys)
        {
            _board->SetTileState(body, ETileState::NONE);
        }

        size_t startIdx = _bodys.size() - 1;
        if (isEatFood)
        {
            startIdx = _bodys.size() - 2;
        }

        for (size_t idx = startIdx; idx >= 1; --idx)
        {
            _bodys[idx].offsetX = _bodys[idx - 1].offsetX;
            _bodys[idx].offsetY = _bodys[idx - 1].offsetY;
        }

        _bodys[0].offsetX = newOffsetX;
        _bodys[0].offsetY = newOffsetY;

        for (const auto& body : _bodys)
        {
            _board->SetTileState(body, ETileState::BODY);
        }
    }
}

void Snake::Render()
{
}

void Snake::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_SNAKE");
		return;
	}

	_isInitialized = false;
}

std::vector<BoardCoord> Snake::CreateBodys()
{
	std::vector<BoardCoord> bodys(_startBodyCount);
	for (size_t idx = 0; idx < bodys.size(); ++idx)
	{
		if (idx == 0) // 머리 부분
		{
			bodys[idx].offsetX = _board->GetColTileCount() / 2;
			bodys[idx].offsetY = _board->GetRowTileCount() / 2;
		}
		else
		{
			auto it = _startDirectionBias.find(_startDirection);
			GAME_ASSERT(it != _startDirectionBias.end(), "NOT_IMPLEMENT_MOVE_DIRECTION_IN_SNAKE(%d)", _startDirection);

			bodys[idx].offsetX = bodys[idx - 1].offsetX + it->second.offsetX;
			bodys[idx].offsetY = bodys[idx - 1].offsetY + it->second.offsetY;
		}
	}

	return bodys;
}

void Snake::SetBodyOnBoard(const ETileState& state)
{
	for (const auto& body : _bodys)
	{
		_board->SetTileState(body, state);
	}
}