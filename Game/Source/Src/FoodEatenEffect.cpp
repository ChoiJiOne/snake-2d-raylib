#include "FoodEatenEffect.h"
#include "GameLog.h"

FoodEatenEffect::FoodEatenEffect(
	int32_t fontSize, 
	const Color& defaultColor,
	const Color& effectColor, 
	float effectFlashTime, 
	float effectTime,
	float moveLength
) : Text(Vector2{0.0f, 0.0f}, "EAT!", fontSize, defaultColor),
	_defaultColor(defaultColor), 
	_effectColor(effectColor), 
	_effectFlashTime(effectFlashTime), 
	_effectTime(effectTime),
	_moveLength(moveLength)
{
	_isInitialized = true;
}

FoodEatenEffect::~FoodEatenEffect()
{
	if (_isInitialized)
	{
		Release();
	}
}

void FoodEatenEffect::Tick(float deltaSeconds)
{
	if (!_isStart)
	{
		return;
	}

	_effectStepTime += deltaSeconds;
	if (_effectStepTime >= _effectTime)
	{
		_isStart = false;
		return;
	}

	_effectFlashStepTime += deltaSeconds;
	if (_effectFlashStepTime >= _effectFlashTime)
	{
		_effectFlashStepTime -= _effectFlashTime;
	}

	float t = _effectFlashStepTime / _effectFlashTime;
	_color = InterpolateColor(_defaultColor, _effectColor, t);
	_center.y -= deltaSeconds * _moveLength;
	
	SetCenter(_center);
}

void FoodEatenEffect::Render()
{
	if (!_isStart)
	{
		return;
	}

	Text::Render();
}

void FoodEatenEffect::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_FOOD_EATEN_EFFECT");
		return;
	}

	_isInitialized = true;
}

void FoodEatenEffect::StartEffect(const Vector2& center)
{
	_isStart = true;
	_effectStepTime = 0.0f;
	_effectFlashStepTime = 0.0f;

	SetCenter(center);
}

Color FoodEatenEffect::InterpolateColor(Color startColor, Color endColor, float t)
{
	Color color;
	color.r = (1.0f - t) * startColor.r + t * endColor.r;
	color.g = (1.0f - t) * startColor.g + t * endColor.g;
	color.b = (1.0f - t) * startColor.b + t * endColor.b;
	color.a = (1.0f - t) * startColor.a + t * endColor.a;
	return color;
}