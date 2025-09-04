#pragma once

#include "Snake.h"
#include "Text.h"

class Level : public Text
{
public:
	Level(
		const Vector2& center,
		int32_t fontSize,
		const Color& defaultColor,
		const Color& effectColor,
		float effectFlashTime,
		float effectTime,
		Snake* snake
	);
	virtual ~Level();

	DISALLOW_COPY_AND_ASSIGN(Level);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	bool IsDirty();
	void UpdateLevel(int32_t score);
	void UpdateEffect(float deltaSeconds);
	Color InterpolateColor(Color startColor, Color endColor, float t);

private:
	static const int32_t START_LEVEL = 1;

private:
	Snake* _snake = nullptr;

	Color _defaultColor;
	Color _effectColor;
	bool _isShowEffect = false;
	float _effectStepTime = 0.0f;
	float _effectTime = 0.0f;
	float _effectFlashStepTime = 0.0f;
	float _effectFlashTime = 0.0f;

	int32_t _currentLevel = 0;
	int32_t _level = 0;
};