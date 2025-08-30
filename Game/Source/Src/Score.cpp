#include "GameLog.h"
#include "Score.h"

Score::Score(const Vector2& center, int32_t fontSize, const Color& color, Snake* snake)
	: Text(center, fontSize, color)
	, _snake(snake)
{
	UpdateScore(START_SCORE);
	_currentBodyCount = _snake->GetBodyCount();
	_isInitialized = true;
}

Score::~Score()
{
	if (_isInitialized)
	{
		Release();
	}
}

void Score::Tick(float deltaSeconds)
{
	int32_t bodyCount = _snake->GetBodyCount();
	if (_currentBodyCount != bodyCount)
	{
		_currentBodyCount = bodyCount;
		UpdateScore(_score + 1);
	}
}

void Score::Render()
{
	Text::Render();
}

void Score::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_SCORE");
		return;
	}

	_isInitialized = true;
}

void Score::UpdateScore(int32_t score)
{
	_score = score;
	SetText(TextFormat("SCORE: %d", _score));
}