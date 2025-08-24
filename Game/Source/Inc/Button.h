#pragma once

#include <functional>
#include <string>

#include <raylib.h>

#include "IActor.h"

class Button : public IActor
{
public:
	Button(
		const Vector2& center, 
		const Vector2& size, 
		const std::function<void()>& onButtonClicked, 
		const std::string& text
	);
	virtual ~Button();

	DISALLOW_COPY_AND_ASSIGN(Button);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	Vector2 _center;
	Vector2 _position;
	Vector2 _size;
	Rectangle _area;
	std::string _text;
	std::function<void()> _onButtonClicked;
};