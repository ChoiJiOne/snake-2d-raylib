#pragma once

#include <raylib.h>

#include "Board.h"
#include "FoodEatenEffect.h"
#include "IActor.h"

class Food : public IActor
{
public:
	Food(Board* board, FoodEatenEffect* effect);
	virtual ~Food();

	DISALLOW_COPY_AND_ASSIGN(Food);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

	bool IsEaten();

private:
	bool TrySetRandomCoord();
	void StartEatenEffect();

private:
	Board* _board = nullptr;
	FoodEatenEffect* _effect = nullptr;
	BoardCoord _boardCoord;
};