#include <yaml-cpp/yaml.h>

#include "ContentConfig.h"
#include "GameAssert.h"

bool ContentConfig::TryLoadConfigFile(const std::string& path)
{
	if (!AppConfig::TryLoadConfigFile(path))
	{
		GAME_LOG_ERR("FAILED_TO_LOAD_CONTENT_CONFIG_FILE(%s)", path.c_str());
		return false;
	}

	YAML::Node boardNode = _rootNode["board"];
	GAME_ASSERT(boardNode.IsDefined(), "UNDEFINED_APP_CONFIG_OBJECT(%s)", "board");

	GAME_CHECK(TryGetConfigValue<float>(boardNode, "tile_size", _tileSize));
	GAME_CHECK(TryGetConfigValue<int32_t>(boardNode, "row_tile_count", _rowTileCount));
	GAME_CHECK(TryGetConfigValue<int32_t>(boardNode, "col_tile_count", _colTileCount));

	YAML::Node snakeNode = _rootNode["snake"];
	GAME_ASSERT(boardNode.IsDefined(), "UNDEFINED_APP_CONFIG_OBJECT(%s)", "snake");

	GAME_CHECK(TryGetConfigValue<int32_t>(snakeNode, "start_direction", _startDirection));

	YAML::Node levelNodeList = snakeNode["levels"];
	for (const auto& levelNode : levelNodeList)
	{
		int32_t level = 0;
		GAME_CHECK(TryGetConfigValue<int>(levelNode, "level", level));

		float speed = 0.0f;
		GAME_CHECK(TryGetConfigValue<float>(levelNode, "speed", speed));

		int32_t body = 0;
		GAME_CHECK(TryGetConfigValue<int>(levelNode, "body", body));

		if (level <= _minLevel)
		{
			_minLevel = level;
		}

		if (level >= _maxLevel)
		{
			_maxLevel = level;
		}

		_levelMap.insert({ level, std::pair<float, int32_t>(speed, body) });
	}
}

const std::pair<float, int32_t>& ContentConfig::GetSpeedAndBody(int32_t level) const
{
	auto it = _levelMap.find(level);
	GAME_ASSERT(it != _levelMap.end(), "INVALID_GAME_LEVEL_TO_SPEED(level:%d)", level);
	return it->second;
}