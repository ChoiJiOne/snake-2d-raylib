#pragma once

#include <memory>

#include <raylib.h>

#include "Macro.h"
#include "GameConfig.h"
#include "IPhase.h"

class GameApp
{
public:
	GameApp() = default;
	virtual ~GameApp();

	void Startup();
	void Run();
	void Shutdown();

	GameConfig* GetConfig() { return _gameConfig.get(); }

	static GameApp* GetAppPtr() { return _gameAppPtr; }

private:
	void ProcessPhaseActionState();

private:
	static GameApp* _gameAppPtr;

private:
	bool _isInitialized = false;
	bool _isDoneLoop = false;

	std::unique_ptr<GameConfig> _gameConfig = nullptr;

	IPhase* _entryPhase = nullptr;
	IPhase* _currentPhase = nullptr;
};