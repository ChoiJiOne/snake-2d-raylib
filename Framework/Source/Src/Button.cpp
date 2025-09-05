#include <raygui.h>

#include "Button.h"
#include "GameLog.h"

Button::Button(
	const Vector2& center, 
	const Vector2& size, 
	const std::function<void()>& onButtonClicked,
	const std::string& text
)
	: _center(center)
	, _size(size)
	, _onButtonClicked(onButtonClicked)
	, _text(text)
{
	_position = Vector2{ _center.x - _size.x * 0.5f, _center.y - _size.y * 0.5f };
	_area = Rectangle{ _position.x, _position.y, _size.x, _size.y };
	_isInitialized = true;
}

Button::~Button()
{
	if (_isInitialized)
	{
		Release();
	}
}

void Button::Tick(float deltaSeconds)
{
}

void Button::Render()
{
	if (GuiButton(_area, _text.c_str()) && _onButtonClicked != nullptr)
	{
		_onButtonClicked();
	}
}

void Button::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_BUTTON");
		return;
	}

	_isInitialized = false;
}