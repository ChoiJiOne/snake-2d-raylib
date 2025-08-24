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
    _lastDirection = _startDirection;

	_startDirectionBias =
	{
		{ EDirection::LEFT,  BoardCoord{ +1,  0 } },
		{ EDirection::RIGHT, BoardCoord{ -1,  0 } },
		{ EDirection::UP,    BoardCoord{  0, +1 } },
		{ EDirection::DOWN,  BoardCoord{  0, -1 } },
	};

    _keyCodeDirections =
    {
        { KEY_LEFT,  EDirection::LEFT  },
        { KEY_RIGHT, EDirection::RIGHT },
        { KEY_UP,    EDirection::UP    },
        { KEY_DOWN,  EDirection::DOWN  },
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
    // 움직일 수 없다면 아무 것도 하지 않음.
    if (_isStopped)
    {
        return;
    }

    _stepTime += deltaSeconds;

    const BoardCoord& head = _bodys.front();
    EDirection direction = EDirection::NONE;
    for (const auto& keyCodeDirection : _keyCodeDirections)
    {
        if (IsKeyPressed(keyCodeDirection.first))
        {
            direction = keyCodeDirection.second;
        }
    }

    if (direction != EDirection::NONE)
    {
        _lastDirection = direction;
        if (!TryMove(head, direction))
        {
            _isStopped = true;
        }
        return;
    }

    if (_stepTime < _moveStepTime)
    {
        return;
    }

    if (!TryMove(head, _lastDirection))
    {
        _isStopped = true;
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

BoardCoord Snake::CalculateDirectionBoardCoord(const BoardCoord& targetCoord, const EDirection& direction)
{
    int newOffsetX = targetCoord.offsetX;
    int newOffsetY = targetCoord.offsetY;
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

    return BoardCoord{ newOffsetX, newOffsetY };
}

bool Snake::TryMove(const BoardCoord& head, const EDirection& direction)
{
    if (!CanMove(head, direction))
    {
        return false;
    }

    MoveDirection(head, direction);
    return true;
}

void Snake::Move(const BoardCoord& destCoord, bool isEatFood)
{
    SetBodyOnBoard(ETileState::NONE);

    // 먹이를 먹었으면, 꼬리는 움직이지 않음.
    size_t startIdx = isEatFood ? _bodys.size() - 2 : _bodys.size() - 1;
    for (size_t idx = startIdx; idx >= 1; --idx)
    {
        _bodys[idx].offsetX = _bodys[idx - 1].offsetX;
        _bodys[idx].offsetY = _bodys[idx - 1].offsetY;
    }
    _bodys.front() = destCoord;

    SetBodyOnBoard(ETileState::BODY);
}

void Snake::MoveDirection(const BoardCoord& head, const EDirection& direction)
{
    BoardCoord moveBoardCoord = CalculateDirectionBoardCoord(head, direction);

    bool isEatFood = (_board->GetTileState(moveBoardCoord) == ETileState::FOOD);
    if (isEatFood)
    {
        _bodys.emplace_back(_bodys.back());
    }

    bool canMove = (_board->GetTileState(moveBoardCoord) != ETileState::BODY);
    if (canMove)
    {
        Move(moveBoardCoord, isEatFood);
        _stepTime = 0.0f;
    }
}

bool Snake::CanMove(const BoardCoord& head, const EDirection& direction)
{
    BoardCoord moveBoardCoord = CalculateDirectionBoardCoord(head, direction);
    return (_board->GetTileState(moveBoardCoord) != ETileState::BODY);
}