#include "TextEffect.h"
#include "GameLog.h"

TextEffect::TextEffect(
	const std::string& text,
	int32_t fontSize, 
	const Color& defaultColor,
	const Color& effectColor, 
	float effectFlashTime, 
	float effectTime,
	float moveLength
) : Text(Vector2{0.0f, 0.0f}, text, fontSize, defaultColor),
	_defaultColor(defaultColor), 
	_effectColor(effectColor), 
	_effectFlashTime(effectFlashTime), 
	_effectTime(effectTime),
	_moveLength(moveLength)
{
	_isInitialized = true;
}

TextEffect::~TextEffect()
{
	if (_isInitialized)
	{
		Release();
	}
}

void TextEffect::Tick(float deltaSeconds)
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

void TextEffect::Render()
{
	if (!_isStart)
	{
		return;
	}

	Text::Render();
}

void TextEffect::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_TEXT_EFFECT");
		return;
	}

	_isInitialized = true;
}

void TextEffect::StartEffect(const Vector2& center)
{
	_isStart = true;
	_effectStepTime = 0.0f;
	_effectFlashStepTime = 0.0f;

	SetCenter(center);
}

Color TextEffect::InterpolateColor(Color startColor, Color endColor, float t)
{
	Color color;
	color.r = (1.0f - t) * startColor.r + t * endColor.r;
	color.g = (1.0f - t) * startColor.g + t * endColor.g;
	color.b = (1.0f - t) * startColor.b + t * endColor.b;
	color.a = (1.0f - t) * startColor.a + t * endColor.a;
	return color;
}