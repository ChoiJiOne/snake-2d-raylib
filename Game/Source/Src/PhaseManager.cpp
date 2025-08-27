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

	for (size_t idx = 0; idx < _phasePool.size(); ++idx)
	{
		if (_phasePool[idx].phase)
		{
			_phasePool[idx].phase.reset();
			_phasePool[idx].isOccupied = false;
		}
	}

	_isInitialized = false;
}

void PhaseManager::Destroy(const IPhase* phase)
{
	int32_t phaseID = -1;
	for (size_t idx = 0; idx < _phasePool.size(); ++idx)
	{
		if (phase == _phasePool[idx].phase.get())
		{
			phaseID = static_cast<int32_t>(idx);
			break;
		}
	}

	if (phaseID != -1 && _phasePool[phaseID].phase)
	{
		_phasePool[phaseID].phase.reset();
		_phasePool[phaseID].isOccupied = false;
	}
}