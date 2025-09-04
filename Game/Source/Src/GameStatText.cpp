#include "GameLog.h"
#include "GameStatText.h"

GameStatText::GameStatText(
	const std::string& gameStatTextformat,
	const int32_t& defaultGameStat,
	const Vector2& center,
	int32_t fontSize,
	const Color& defaultColor,
	const Color& effectColor,
	float effectFlashTime,
	float effectTime
) : Text(center, fontSize, defaultColor),
	_defaultColor(defaultColor),
	_effectColor(effectColor),
	_effectFlashTime(effectFlashTime),
	_effectTime(effectTime),
	_gameStatTextformat(gameStatTextformat),
	_gameStat(defaultGameStat)
{
	UpdateStat();
	_isInitialized = true;
}

GameStatText::~GameStatText()
{
	if (_isInitialized)
	{
		Release();
	}
}

void GameStatText::Tick(float deltaSeconds)
{
	if (_isDirty)
	{
		UpdateStat();
		_effectStepTime = 0.0f;
		_effectFlashStepTime = 0.0f;
		_isShowEffect = true;
		_isDirty = false;
	}

	UpdateEffect(deltaSeconds);
}

void GameStatText::Render()
{
	Text::Render();
}

void GameStatText::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_GAME_STAT_TEXT");
		return;
	}

	_isInitialized = true;
}

void GameStatText::SetStat(int stat)
{
	_gameStat = stat;
	_isDirty = true;
}

void GameStatText::UpdateStat()
{
	SetText(TextFormat(_gameStatTextformat.c_str(), _gameStat));
}

void GameStatText::UpdateEffect(float deltaSeconds)
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

Color GameStatText::InterpolateColor(Color startColor, Color endColor, float t)
{
	Color color;
	color.r = (1.0f - t) * startColor.r + t * endColor.r;
	color.g = (1.0f - t) * startColor.g + t * endColor.g;
	color.b = (1.0f - t) * startColor.b + t * endColor.b;
	color.a = (1.0f - t) * startColor.a + t * endColor.a;
	return color;
}