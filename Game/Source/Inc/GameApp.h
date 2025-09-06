#pragma once

#include "Application.h"

class GameApp : public Application
{
public:
	GameApp() = default;
	virtual ~GameApp() = default;

	virtual void Startup() override;
	virtual void Shutdown() override;

private:
	bool _isInitialized = false;
};