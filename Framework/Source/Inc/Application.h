#pragma once

#include <memory>

#include "Macro.h"
#include "GameConfig.h"
#include "IPhase.h"

class Application
{
public:
	Application() = default;
	virtual ~Application();

	virtual void Startup();
	virtual void Run();
	virtual void Shutdown();

	bool IsInitialized() const { return _isInitialized; }
	bool IsDoneLoop() const { return _isDoneLoop; }

	IPhase* GetEntryPhase() const { return _entryPhase; }
	IPhase* GetCurrentPhase() const { return _currentPhase; }
	GameConfig* GetConfig() { return _gameConfig.get(); }

	void SetEntryPhase(IPhase* entryPhase) { _entryPhase = entryPhase; }

	static Application* GetApp() { return _app; }

protected:
	void ProcessPhaseActionState();
	
private:
	static Application* _app;

private:
	bool _isInitialized = false;
	bool _isDoneLoop = false;

	IPhase* _entryPhase = nullptr;
	IPhase* _currentPhase = nullptr;

	std::unique_ptr<GameConfig> _gameConfig = nullptr;

	class ActorManager* _actorMgr;
	class PhaseManager* _phaseMgr;
};