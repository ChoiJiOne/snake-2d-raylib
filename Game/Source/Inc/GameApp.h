#pragma once

#include <raylib.h>

#include "Macro.h"

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
};