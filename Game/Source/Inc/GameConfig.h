#pragma once

#include <string>
#include <unordered_map>

#include "Macro.h"

class GameConfig
{
public:
	GameConfig() = default;
	virtual ~GameConfig() = default;

	void Load(const std::string& path);

	const std::string& GetWindowTitle() const { return _windowTitle; }
	const int32_t& GetWindowWidth() const { return _windowWidth; }
	const int32_t& GetWindowHeight() const { return _windowHeight; }
	const int32_t& GetFPS() const { return _fps; }

private:
	static const int32_t WINDOW_MIN_SIZE = 1;
	static const int32_t DEFAULT_FPS = 60;

private:
	std::string _windowTitle;
	int32_t _windowWidth = -1;
	int32_t _windowHeight = -1;
	int32_t _fps = 0;
};