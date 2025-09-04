#pragma once

#include "Text.h"

class GameStatText : public Text
{
public:
	GameStatText(
		const std::string& gameStatTextformat,
		const int32_t& defaultGameStat,
		const Vector2& center,
		int32_t fontSize,
		const Color& defaultColor,
		const Color& effectColor,
		float effectFlashTime,
		float effectTime
	);
	virtual ~GameStatText();

	DISALLOW_COPY_AND_ASSIGN(GameStatText);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	void UpdateStat();
	void UpdateEffect(float deltaSeconds);
	Color InterpolateColor(Color startColor, Color endColor, float t);
	
private:
	Color _defaultColor;
	Color _effectColor;
	bool _isShowEffect = false;
	float _effectStepTime = 0.0f;
	float _effectTime = 0.0f;
	float _effectFlashStepTime = 0.0f;
	float _effectFlashTime = 0.0f;

	bool _isDirty = false;
	std::string _gameStatTextformat;
	int32_t _gameStat = 0;
};