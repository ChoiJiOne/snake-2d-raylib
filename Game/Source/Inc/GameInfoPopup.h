#pragma once

#include "IActor.h"
#include "Snake.h"

class GameInfoPopup : public IActor
{
public:
	GameInfoPopup(Vector2 center, Vector2 size, Snake* snake);
	virtual ~GameInfoPopup();

	DISALLOW_COPY_AND_ASSIGN(GameInfoPopup);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	Rectangle _round;
	Snake* _snake = nullptr;

	bool _isShowMessageBox = false;
};