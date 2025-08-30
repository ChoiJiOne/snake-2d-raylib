#include "GameLog.h"
#include "Score.h"

Score::Score(
	const Vector2& center, 
	int32_t fontSize, 
	const Color& defaultColor, 
	const Color& effectColor, 
	float effectFlashTime,
	float effectTime,
	Snake* snake
) : Text(center, fontSize, defaultColor), 
	_defaultColor(defaultColor), 
	_effectColor(effectColor), 
	_effectFlashTime(effectFlashTime),
	_effectTime(effectTime),
	_snake(snake)
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
	if (HasGainedScore())
	{
		_currentBodyCount = _snake->GetBodyCount();
		UpdateScore(_score + 1);

		_effectStepTime = 0.0f;
		_isShowEffect = true;
	}

	UpdateEffect(deltaSeconds);
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

bool Score::HasGainedScore()
{
	return _currentBodyCount != _snake->GetBodyCount();
}

void Score::UpdateScore(int32_t score)
{
	_score = score;
	SetText(TextFormat("SCORE: %d", _score));
}

void Score::UpdateEffect(float deltaSeconds)
{
	if (!_isShowEffect)
	{
		return;
	}

	_effectStepTime += deltaSeconds;
	if (_effectStepTime >= _effectTime)
	{
		_isShowEffect = false;
		SetColor(_defaultColor);
		return;
	}

	_effectFlashStepTime += deltaSeconds;
	if (_effectFlashStepTime >= _effectFlashTime)
	{
		_effectFlashStepTime -= _effectFlashTime;
	}

	float t = _effectFlashStepTime / _effectFlashTime;
	Color color = InterpolateColor(_defaultColor, _effectColor, t);
	SetColor(color);
}

Color Score::InterpolateColor(Color startColor, Color endColor, float t)
{
	Color color;
	color.r = (1.0f - t) * startColor.r + t * endColor.r;
	color.g = (1.0f - t) * startColor.g + t * endColor.g;
	color.b = (1.0f - t) * startColor.b + t * endColor.b;
	color.a = (1.0f - t) * startColor.a + t * endColor.a;
	return color;
}