#pragma once

#include <yaml-cpp/yaml.h>

#include "AppConfig.h"

class ContentConfig : public AppConfig
{
public:
	ContentConfig() = default;
	virtual ~ContentConfig() = default;

	virtual bool TryLoadConfigFile(const std::string& path) override;

	const float& GetTileSize() const { return _tileSize; }
	const int32_t& GetRowTileCount() const { return _rowTileCount; }
	const int32_t& GetColTileCount() const { return _colTileCount; }

	const int32_t& GetStartDirection() const { return _startDirection; }
	const int32_t& GetMinLevel() const { return _minLevel; }
	const int32_t& GetMaxLevel() const { return _maxLevel; }
	const std::pair<float, int32_t>& GetSpeedAndBody(int32_t level) const;

private:
	static const int32_t MIN_LEVEL = -1;
	static const int32_t MAX_LEVEL = 1000;

private:
	// board
	float _tileSize = 0.0f;
	int32_t _rowTileCount = 0;
	int32_t _colTileCount = 0;

	// snake
	int32_t _startDirection = 0;
	int32_t _minLevel = MAX_LEVEL;
	int32_t _maxLevel = MIN_LEVEL;
	std::map<int32_t, std::pair<float, int32_t>> _levelMap;
};