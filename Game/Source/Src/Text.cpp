#include "GameAssert.h"
#include "GameLog.h"
#include "Text.h"

Text::Text(const Vector2& center, const std::string& text, int32_t fontSize, const Color& color)
	: _center(center)
	, _text(text)
	, _fontSize(fontSize)
	, _color(color)
{
	GAME_ASSERT(_fontSize > 0, "INVALID_TEXT_FONT_SIZE(fontSize:%d)", fontSize);

	CalculateTextProperties();
	_isInitialized = true;
}

Text::~Text()
{
	if (_isInitialized)
	{
		Release();
	}
}

void Text::Tick(float deltaSeconds)
{
}

void Text::Render()
{
	DrawText(_text.c_str(), _position.x, _position.y, _fontSize, _color);
}

void Text::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_TEXT");
		return;
	}

	_isInitialized = false;
}

void Text::SetCenter(const Vector2& center)
{
	_center = center;
	CalculateTextProperties();
}

void Text::SetText(const std::string& text)
{
	_text = text;
	CalculateTextProperties();
}

void Text::SetFontSize(int32_t fontSize)
{
	_fontSize = fontSize;
	CalculateTextProperties();
}

void Text::SetColor(const Color& color)
{
	_color = color;
}

void Text::SetProperties(const Vector2& center, const std::string& text, int32_t fontSize, const Color& color)
{
	_center = center;
	_text = text;
	_fontSize = fontSize;
	_color = color;
	CalculateTextProperties();
}

void Text::CalculateTextProperties()
{
	float textWidth = static_cast<float>(MeasureText(_text.c_str(), _fontSize));
	float textHeight = static_cast<float>(_fontSize);

	_size = Vector2{ textWidth, textHeight };
	_position = Vector2{ _center.x - _size.x * 0.5f, _center.y - _size.y * 0.5f };
}