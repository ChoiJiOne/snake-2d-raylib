#include "GameAssert.h"
#include "GameLog.h"
#include "PhaseManager.h"

void PhaseManager::Startup()
{
	if (_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_INITIALIZE_PHASE_MANAGER");
		return;
	}

	_isInitialized = true;
}

void PhaseManager::Shutdown()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("NOT_INITIALIZE_PHASE_MANAGER");
		return;
	}

	_isInitialized = false;
}