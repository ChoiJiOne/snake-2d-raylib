#pragma once

#include "IActor.h"
#include "Snake.h"

class GameInfoPopup : public IActor
{
public:
	enum class EState
	{
		PLAY = 0x00,
		PAUSE = 0x01,
		GAME_OVER = 0x02,
	};

public:
	GameInfoPopup(Vector2 center, Vector2 size, Snake* snake);
	virtual ~GameInfoPopup();

	DISALLOW_COPY_AND_ASSIGN(GameInfoPopup);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;
		
private:
	Rectangle _round;
	EState _state = EState::PLAY;
	Snake* _snake = nullptr;

	std::string _title;
	std::string _info;
	std::string _buttons;
};