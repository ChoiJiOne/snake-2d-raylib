#pragma once

#include <vector>
#include <map>

#include <raylib.h>

#include "Board.h"
#include "IActor.h"

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
	Snake(Board* board, int32_t startBodyCount, const EDirection& startDirection);
	virtual ~Snake();

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	std::vector<BoardCoord> CreateBodys();

	void SetBodyOnBoard(const ETileState& state);
	BoardCoord CalculateDirectionBoardCoord(const BoardCoord& targetCoord, const EDirection& direction);
	void Move(const BoardCoord& destCoord, bool isEatFood);

private:
	static const int32_t MIN_START_BODY_COUNT = 2;

private:
	Board* _board = nullptr;
	int32_t _startBodyCount = 0;
	std::vector<BoardCoord> _bodys;

	EDirection _startDirection = EDirection::NONE;
	std::map<EDirection, BoardCoord> _startDirectionBias;
	std::map<KeyboardKey, EDirection> _keyCodeDirections;
};