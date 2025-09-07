#pragma once

#include "Application.h"
#include "ContentConfig.h"

class GameApp : public Application
{
public:
	GameApp() = default;
	virtual ~GameApp() = default;

	virtual void Startup() override;
	virtual void Shutdown() override;

private:
	bool _isInitialized = false;
	std::unique_ptr<ContentConfig> _contentConfig = nullptr;
};