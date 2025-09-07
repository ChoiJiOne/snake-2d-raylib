#include <yaml-cpp/yaml.h>

#include "AppConfig.h"
#include "GameAssert.h"
#include "GameLog.h"

bool AppConfig::TryLoadConfigFile(const std::string& path)
{
	try
	{
		// yaml-cpp�� ���� ������ ���⼭ ���� ó�� ����!
		_rootNode = YAML::LoadFile(path);
	}
	catch (YAML::Exception e)
	{
		GAME_LOG_ERR("FAILED_TO_LOAD_APP_CONFIG_FILE(%s)", path.c_str());

		// ���� �б⿡ �����ϸ� �׳� �⺻ ������ ����!
		_windowTitle = "Game";
		_windowWidth = std::max(_windowWidth, WINDOW_MIN_SIZE);
		_windowHeight = std::max(_windowHeight, WINDOW_MIN_SIZE);
		_fps = DEFAULT_FPS;
		_guiFontSize = DEFAULT_GUI_FONT_SIZE;
		return false;
	}

	YAML::Node appNode = _rootNode["app"];
	if (!appNode.IsDefined() ||
		!TryGetConfigValue<std::string>(appNode, "title", _windowTitle) ||
		!TryGetConfigValue<int32_t>(appNode, "width", _windowWidth) ||
		!TryGetConfigValue<int32_t>(appNode, "height", _windowHeight) ||
		!TryGetConfigValue<int32_t>(appNode, "fps", _fps) ||
		!TryGetConfigValue<int32_t>(appNode, "gui_font_size", _guiFontSize))
	{
		return false;
	}

	return true;
}