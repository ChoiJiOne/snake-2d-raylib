#include "GameAssert.h"
#include "GameApp.h"
#include "GameLog.h"
#include "Snake.h"
#include "PhaseManager.h"

Snake::Snake(
    Board * board, 
    TextEffect* effect, 
    GameStatText* scoreStat,
    GameStatText* levelStat,
    const EDirection& startDirection
)
{
	GAME_CHECK(board != nullptr);
	_board = board;

    GAME_CHECK(effect != nullptr);
    _effect = effect;

    GAME_CHECK(scoreStat != nullptr);
    _scoreStat = scoreStat;

    GAME_CHECK(levelStat != nullptr);
    _levelStat = levelStat;

    GameConfig* config = GameApp::GetAppPtr()->GetConfig();
    GAME_CHECK(config != nullptr);

    _level = config->GetMinLevel();
    _maxLevel = config->GetMaxLevel();
    auto levelConfig = config->GetSpeedAndBody(_level);
    _moveStepTime = levelConfig.first;
    _startBodyCount = levelConfig.second;

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

    _counterDirections =
    {
        { EDirection::LEFT,  EDirection::RIGHT, },
        { EDirection::RIGHT, EDirection::LEFT,  },
        { EDirection::UP,    EDirection::DOWN,  },
        { EDirection::DOWN,  EDirection::UP,    },
    };

	_bodys = CreateBodys();
	SetBodyOnBoard(ETileState::BODY);

    _state = EState::ALIVE;
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
    if (!IsAlive())
    {
        return;
    }

    _stepTime += deltaSeconds;

    EDirection direction = GetDirectionFromInput();
    if (direction != EDirection::NONE)
    {
        _lastDirection = direction;
        if (!TryMove(_bodys.front(), direction))
        {
            GameOver();
        }
        return;
    }

    if (_stepTime < _moveStepTime)
    {
        return;
    }

    if (!TryMove(_bodys.front(), _lastDirection))
    {
        GameOver();
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

void Snake::Stop()
{
    _state = EState::STOPPED;
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

EDirection Snake::GetDirectionFromInput()
{
    EDirection direction = EDirection::NONE;
    for (const auto& keyCodeDirection : _keyCodeDirections)
    {
        if (IsKeyPressed(keyCodeDirection.first))
        {
            direction = keyCodeDirection.second;
        }
    }

    // 방향이 없거나, 현재 움직이고 있는 방향의 반대 방향을 입력하면 이동 처리를 하지 않음.
    if (direction == EDirection::NONE || _counterDirections[_lastDirection] == direction)
    {
        return EDirection::NONE;
    }
    
    return direction;
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

void Snake::GameOver()
{
    if (!IsAlive())
    {
        // 이미 중지 상태라면 아무 것도 하지 않음.
        return;
    }

    _state = EState::DEAD;
}

void Snake::StartEffect()
{
    GameConfig* config = GameApp::GetAppPtr()->GetConfig();

    Vector2 center = Vector2{
        static_cast<float>(config->GetWindowWidth()) / 2.0f,
        19.0f * static_cast<float>(config->GetWindowHeight()) / 20.0f
    };

    _effect->StartEffect(center);
}

void Snake::MoveDirection(const BoardCoord& head, const EDirection& direction)
{
    BoardCoord moveBoardCoord = CalculateDirectionBoardCoord(head, direction);

    bool isEatFood = (_board->GetTileState(moveBoardCoord) == ETileState::FOOD);
    if (isEatFood)
    {
        if (_level < _maxLevel)
        {
            int32_t currentBodyCount = static_cast<int32_t>(_bodys.size());
            int32_t nextLevel = _level + 1;

            GameConfig* config = GameApp::GetAppPtr()->GetConfig();
            const auto& levelConfig = config->GetSpeedAndBody(nextLevel);
            float nextMoveStepTime = levelConfig.first;
            int32_t nextBodyCount = levelConfig.second;
            if (currentBodyCount + 1 == nextBodyCount)
            {
                _level = nextLevel;
                _moveStepTime = nextMoveStepTime;
                _levelStat->SetStat(_level);

                StartEffect();
            }
        }

        _score++;
        _scoreStat->SetStat(_score);
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