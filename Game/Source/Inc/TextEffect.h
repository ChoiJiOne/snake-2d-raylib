#pragma once

#include "Text.h"

class TextEffect : public Text
{
public:
	TextEffect(
		const std::string& text,
		int32_t fontSize,
		const Color& defaultColor,
		const Color& effectColor,
		float effectFlashTime,
		float effectTime,
		float moveLength
	);
	virtual ~TextEffect();

	DISALLOW_COPY_AND_ASSIGN(TextEffect);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

	void StartEffect(const Vector2& center);

private:
	Color InterpolateColor(Color startColor, Color endColor, float t);
	
private:
	bool _isStart = false;

	Color _defaultColor;
	Color _effectColor;
	float _effectStepTime = 0.0f;
	float _effectTime = 0.0f;
	float _effectFlashStepTime = 0.0f;
	float _effectFlashTime = 0.0f;

	float _moveLength = 0.0f;
};