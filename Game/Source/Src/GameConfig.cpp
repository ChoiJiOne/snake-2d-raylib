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
		// yaml-cpp�� ���� ������ ���⼭ ���� ó�� ����!
		rootConfig = YAML::LoadFile(path);
	}
	catch (YAML::Exception e)
	{
		GAME_LOG_ERR("FAILED_TO_LOAD_GAME_CONFIG_FILE(%s)", path.c_str());

		// ���� �б⿡ �����ϸ� �׳� �⺻ ������ ����!
		_windowTitle = "Game";
		_windowWidth = std::max(_windowWidth, WINDOW_MIN_SIZE);
		_windowHeight = std::max(_windowHeight, WINDOW_MIN_SIZE);
		_fps = DEFAULT_FPS;
		return;
	}

	YAML::Node windowNode = rootConfig["app"];
	GAME_ASSERT(windowNode.IsDefined(), "UNDEFINED_APP_CONFIG_OBJECT(%s)", "window");

	GAME_CHECK(TryGetConfigValue<std::string>(windowNode, "title", _windowTitle));
	GAME_CHECK(TryGetConfigValue<int>(windowNode, "width", _windowWidth));
	GAME_CHECK(TryGetConfigValue<int>(windowNode, "height", _windowHeight));
	GAME_CHECK(TryGetConfigValue<int>(windowNode, "fps", _fps));
}