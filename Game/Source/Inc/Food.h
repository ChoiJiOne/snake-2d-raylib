#pragma once

#include <raylib.h>

#include "Board.h"
#include "TextEffect.h"
#include "IActor.h"

class Food : public IActor
{
public:
	Food(Board* board, TextEffect* effect);
	virtual ~Food();

	DISALLOW_COPY_AND_ASSIGN(Food);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

	bool IsEaten();

private:
	bool TrySetRandomCoord();
	void StartEffect();

private:
	Board* _board = nullptr;
	TextEffect* _effect = nullptr;
	BoardCoord _boardCoord;
};