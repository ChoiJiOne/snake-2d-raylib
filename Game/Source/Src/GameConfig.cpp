#include <yaml-cpp/yaml.h>

#include "GameConfig.h"
#include "GameAssert.h"
#include "GameLog.h"

template <typename T>
bool TryGetConfigValue(const YAML::Node& node, const std::string& key, T& outValue)
{
	if (!node[key].IsDefined())
	{
		GAME_LOG_ERR("UNDEFINED_APP_CONFIG_OBJECT(%s)", key.c_str());
		return false;
	}
	
	outValue = node[key].as<T>();
	return true;
}

void GameConfig::Load(const std::string& path)
{
	YAML::Node rootConfig;

	try
	{
		// yaml-cpp가 예외 던져서 여기서 예외 처리 수행!
		rootConfig = YAML::LoadFile(path);
	}
	catch (YAML::Exception e)
	{
		GAME_LOG_ERR("FAILED_TO_LOAD_GAME_CONFIG_FILE(%s)", path.c_str());

		// 파일 읽기에 실패하면 그냥 기본 값으로 설정!
		_windowTitle = "Game";
		_windowWidth = std::max(_windowWidth, WINDOW_MIN_SIZE);
		_windowHeight = std::max(_windowHeight, WINDOW_MIN_SIZE);
		_fps = DEFAULT_FPS;
		_guiFontSize = DEFAULT_GUI_FONT_SIZE;
		return;
	}

	// app (GameConfig.yaml)
	YAML::Node appNode = rootConfig["app"];
	GAME_ASSERT(appNode.IsDefined(), "UNDEFINED_APP_CONFIG_OBJECT(%s)", "app");

	GAME_CHECK(TryGetConfigValue<std::string>(appNode, "title", _windowTitle));
	GAME_CHECK(TryGetConfigValue<int32_t>(appNode, "width", _windowWidth));
	GAME_CHECK(TryGetConfigValue<int32_t>(appNode, "height", _windowHeight));
	GAME_CHECK(TryGetConfigValue<int32_t>(appNode, "fps", _fps));
	GAME_CHECK(TryGetConfigValue<int32_t>(appNode, "gui_font_size", _guiFontSize));

	// board (GameConfig.yaml)
	YAML::Node boardNode = rootConfig["board"];
	GAME_ASSERT(boardNode.IsDefined(), "UNDEFINED_APP_CONFIG_OBJECT(%s)", "board");

	GAME_CHECK(TryGetConfigValue<float>(boardNode, "tile_size", _tileSize));
	GAME_CHECK(TryGetConfigValue<int32_t>(boardNode, "row_tile_count", _rowTileCount));
	GAME_CHECK(TryGetConfigValue<int32_t>(boardNode, "col_tile_count", _colTileCount));

	// snake (GameConfig.yaml)
	YAML::Node snakeNode = rootConfig["snake"];
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

const std::pair<float, int32_t>& GameConfig::GetSpeedAndBody(int32_t level) const
{
	auto it = _levelMap.find(level);
	GAME_ASSERT(it != _levelMap.end(), "INVALID_GAME_LEVEL_TO_SPEED(level:%d)", level);
	return it->second;
}