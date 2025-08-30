#pragma once

#include "Snake.h"
#include "Text.h"

class Score : public Text
{
public:
	Score(const Vector2& center, int32_t fontSize, const Color& color, Snake* snake);
	virtual ~Score();

	DISALLOW_COPY_AND_ASSIGN(Score);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	void UpdateScore(int32_t score);

private:
	static const int32_t START_SCORE = 0;

private:
	Snake* _snake = nullptr;

	int32_t _currentBodyCount = 0;
	int32_t _score = 0;
};