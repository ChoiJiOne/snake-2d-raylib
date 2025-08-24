#pragma once

#include <memory>

#include <raylib.h>

#include "Macro.h"
#include "GameConfig.h"

class GameApp
{
public:
	GameApp() = default;
	virtual ~GameApp();

	void Startup();
	void Run();
	void Shutdown();

private:
	bool _isInitialized = false;
	std::unique_ptr<GameConfig> _gameConfig = nullptr;
};