#include "GameLog.h"
#include "Level.h"

Level::Level(
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
	UpdateLevel(START_LEVEL);
	_currentLevel = _snake->GetLevel();
	_isInitialized = true;
}

Level::~Level()
{
	if (_isInitialized)
	{
		Release();
	}
}

void Level::Tick(float deltaSeconds)
{
	if (IsDirty())
	{
		_currentLevel = _snake->GetLevel();
		UpdateLevel(_currentLevel);

		_effectStepTime = 0.0f;
		_effectFlashStepTime = 0.0f;
		_isShowEffect = true;
	}

	UpdateEffect(deltaSeconds);
}

void Level::Render()
{
	Text::Render();
}

void Level::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_LEVEL");
		return;
	}

	_isInitialized = true;
}

bool Level::IsDirty()
{
	return _currentLevel != _snake->GetLevel();
}

void Level::UpdateLevel(int32_t level)
{
	_level = level;
	SetText(TextFormat("LEVEL: %d", _level));
}

void Level::UpdateEffect(float deltaSeconds)
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

Color Level::InterpolateColor(Color startColor, Color endColor, float t)
{
	Color color;
	color.r = (1.0f - t) * startColor.r + t * endColor.r;
	color.g = (1.0f - t) * startColor.g + t * endColor.g;
	color.b = (1.0f - t) * startColor.b + t * endColor.b;
	color.a = (1.0f - t) * startColor.a + t * endColor.a;
	return color;
}