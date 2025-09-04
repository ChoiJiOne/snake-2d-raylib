#pragma once

#include <string>
#include <map>

#include "Macro.h"

class GameConfig
{
public:
	GameConfig() = default;
	virtual ~GameConfig() = default;

	void Load(const std::string& path);

	// app (GameConfig.yaml)
	const std::string& GetWindowTitle() const { return _windowTitle; }
	const int32_t& GetWindowWidth() const { return _windowWidth; }
	const int32_t& GetWindowHeight() const { return _windowHeight; }
	const int32_t& GetFPS() const { return _fps; }
	const int32_t& GetGuiFontSize() const { return _guiFontSize; }
	
	// board (GameConfig.yaml)
	const float& GetTileSize() const { return _tileSize; }
	const int32_t& GetRowTileCount() const { return _rowTileCount; }
	const int32_t& GetColTileCount() const { return _colTileCount; }

	// snake (GameConfig.yaml)
	const int32_t& GetStartDirection() const { return _startDirection; }
	const int32_t& GetMinLevel() const { return _minLevel; }
	const int32_t& GetMaxLevel() const { return _maxLevel; }
	const std::pair<float, int32_t>& GetSpeedAndBody(int32_t level) const;

private:
	static const int32_t WINDOW_MIN_SIZE = 1;
	static const int32_t DEFAULT_GUI_FONT_SIZE = 10;
	static const int32_t DEFAULT_FPS = 60;
	static const int32_t MIN_LEVEL = -1;
	static const int32_t MAX_LEVEL = 1000;

private:
	// app (GameConfig.yaml)
	std::string _windowTitle;
	int32_t _windowWidth = -1;
	int32_t _windowHeight = -1;
	int32_t _fps = 0;
	int32_t _guiFontSize = 0;

	// board (GameConfig.yaml)
	float _tileSize = 0.0f;
	int32_t _rowTileCount = 0;
	int32_t _colTileCount = 0;

	// snake (GameConfig.yaml)
	int32_t _startDirection = 0;
	int32_t _minLevel = MAX_LEVEL;
	int32_t _maxLevel = MIN_LEVEL;
	std::map<int32_t, std::pair<float, int32_t>> _levelMap;
};