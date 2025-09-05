#pragma once

#include <vector>
#include <map>

#include <raylib.h>

#include "Board.h"
#include "GameStatText.h"
#include "IActor.h"
#include "TextEffect.h"

enum class EDirection
{
	NONE  = 0x00,
	LEFT  = 0x01,
	RIGHT = 0x02,
	UP    = 0x03,
	DOWN  = 0x04,
};

class Snake : public IActor
{
public:
	enum class EState
	{
		NONE = 0x00,
		ALIVE = 0x01,
		STOPPED = 0x02,
		DEAD = 0x03,
	};

public:
	Snake(
		Board* board, 
		TextEffect* effect, 
		GameStatText* scoreStat,
		GameStatText* levelStat,
		const EDirection& startDirection
	);
	virtual ~Snake();

	DISALLOW_COPY_AND_ASSIGN(Snake);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

	void WakeUp();
	void Stop();

	bool IsAlive() const { return _state == EState::ALIVE; }
	bool IsStopped() const { return _state == EState::STOPPED; }
	bool IsDead() const { return _state == EState::DEAD; }
	int32_t GetBodyCount() const { return static_cast<int32_t>(_bodys.size()); }
	int32_t GetScore() const { return _score; }
	int32_t GetLevel() const { return _level; }

private:
	std::vector<BoardCoord> CreateBodys();
	EDirection GetDirectionFromInput();
	void SetBodyOnBoard(const ETileState& state);
	BoardCoord CalculateDirectionBoardCoord(const BoardCoord& targetCoord, const EDirection& direction);
	bool TryMove(const BoardCoord& head, const EDirection& direction);
	void MoveDirection(const BoardCoord& head, const EDirection& direction);
	bool CanMove(const BoardCoord& head, const EDirection& direction);
	void Move(const BoardCoord& destCoord, bool isEatFood);
	void GameOver();
	void StartEffect();

private:
	static const int32_t MIN_START_BODY_COUNT = 2;

private:
	Board* _board = nullptr;
	TextEffect* _effect = nullptr;
	int32_t _startBodyCount = 0;
	std::vector<BoardCoord> _bodys;

	EDirection _startDirection = EDirection::NONE;
	EDirection _lastDirection = EDirection::NONE;
	std::map<EDirection, BoardCoord> _startDirectionBias;
	std::map<KeyboardKey, EDirection> _keyCodeDirections;
	std::map<EDirection, EDirection> _counterDirections;

	int32_t _level = 0;
	int32_t _score = 0;
	int32_t _maxLevel = 0;
	float _moveStepTime = 0.0f;
	float _stepTime = 0.0f;

	EState _state = EState::NONE;

	GameStatText* _scoreStat = nullptr;
	GameStatText* _levelStat = nullptr;
};