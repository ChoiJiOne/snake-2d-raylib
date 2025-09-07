#pragma once

#include <memory>

#include "Macro.h"
#include "AppConfig.h"
#include "IPhase.h"

class Application
{
public:
	Application() = default;
	virtual ~Application();

	virtual void Startup();
	virtual void Shutdown();

	void Run();

	bool IsStartupApp() const { return _isStartupApp; }
	bool IsDoneLoop() const { return _isDoneLoop; }

	IPhase* GetEntryPhase() const { return _entryPhase; }
	IPhase* GetCurrentPhase() const { return _currentPhase; }
	AppConfig* GetConfig() { return _config; }

	void SetEntryPhase(IPhase* entryPhase) { _entryPhase = entryPhase; }

	static Application* GetApp() { return _app; }

protected:
	void SetConfig(AppConfig* config);
	void ProcessPhaseActionState();
	
private:
	static Application* _app;

private:
	bool _isStartupApp = false;
	bool _isDoneLoop = false;

	IPhase* _entryPhase = nullptr;
	IPhase* _currentPhase = nullptr;

	AppConfig* _config = nullptr;

	class ActorManager* _actorMgr;
	class PhaseManager* _phaseMgr;
};