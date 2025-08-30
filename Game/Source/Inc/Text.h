#pragma once

#include <string>

#include <raylib.h>

#include "IActor.h"

class Text : public IActor
{
public:
	Text(
		const Vector2& center, 
		const std::string& text, 
		int32_t fontSize, 
		const Color& color
	);
	Text(
		const Vector2& center,
		int32_t fontSize,
		const Color& color
	);
	virtual ~Text();

	DISALLOW_COPY_AND_ASSIGN(Text);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

	const Vector2& GetCenter() const { return _center; }
	const Vector2& GetPosition() const { return _position; }
	const Vector2& GetSize() const { return _size; }
	const std::string& GetText() const { return _text; }
	const int32_t& GetFontSize() const { return _fontSize; }
	const Color& GetColor() const { return _color; }
	
	void SetCenter(const Vector2& center);
	void SetText(const std::string& text);
	void SetFontSize(int32_t fontSize);
	void SetColor(const Color& color);
	void SetProperties(const Vector2& center, const std::string& text, int32_t fontSize, const Color& color);
	
protected:
	// Setter 호출 시 요 함수 호출해주어야 함.
	void CalculateTextProperties();
	
protected:
	Vector2 _center;
	Vector2 _position; // 좌측 상단 좌표. (외부 설정 X)
	Vector2 _size; // 외부 설정 X
	std::string _text;
	int32_t _fontSize = 0;
	Color _color;
};