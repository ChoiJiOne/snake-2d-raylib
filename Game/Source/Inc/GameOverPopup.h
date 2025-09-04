#pragma once

#include "IActor.h"
#include "Snake.h"

class GameOverPopup : public IActor
{
public:
	GameOverPopup(Vector2 center, Vector2 size, Snake* snake);
	virtual ~GameOverPopup();

	DISALLOW_COPY_AND_ASSIGN(GameOverPopup);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	Rectangle _round;
	Snake* _snake = nullptr;

	bool _isShowMessageBox = false;
};