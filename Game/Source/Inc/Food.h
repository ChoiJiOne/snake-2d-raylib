#pragma once

#include <raylib.h>

#include "Board.h"
#include "IActor.h"

class Food : public IActor
{
public:
	Food(Board* board);
	virtual ~Food();

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	bool TrySetRandomCoord();
	
private:
	Board* _board = nullptr;
	BoardCoord _boardCoord;
};