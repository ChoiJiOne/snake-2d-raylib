#pragma once

#include <string>
#include <map>

#include <yaml-cpp/yaml.h>

#include "GameAssert.h"
#include "GameLog.h"
#include "Macro.h"

class AppConfig
{
public:
	AppConfig() = default;
	virtual ~AppConfig() = default;

	virtual bool TryLoadConfigFile(const std::string& path);

	const std::string& GetWindowTitle() const { return _windowTitle; }
	const int32_t& GetWindowWidth() const { return _windowWidth; }
	const int32_t& GetWindowHeight() const { return _windowHeight; }
	const int32_t& GetFPS() const { return _fps; }
	const int32_t& GetGuiFontSize() const { return _guiFontSize; }

protected:
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

	YAML::Node _rootNode;

private:
	static const int32_t WINDOW_MIN_SIZE = 1;
	static const int32_t DEFAULT_GUI_FONT_SIZE = 10;
	static const int32_t DEFAULT_FPS = 60;

private:
	std::string _windowTitle;
	int32_t _windowWidth = -1;
	int32_t _windowHeight = -1;
	int32_t _fps = 0;
	int32_t _guiFontSize = 0;
};