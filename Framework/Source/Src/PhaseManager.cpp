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

void PhaseManager::Register(const std::string& name, IPhase* phase)
{
	auto it = _phaseCacheMap.find(name);
	GAME_ASSERT(it == _phaseCacheMap.end(), "ALREADY_REGISTER_PHASE(name:%s)", name.c_str());

	_phaseCacheMap.insert({ name, phase });
}

void PhaseManager::Unregister(const std::string& name)
{
	auto it = _phaseCacheMap.find(name);
	GAME_ASSERT(it != _phaseCacheMap.end(), "ALREADY_UNREGISTER_PHASE_OR_NOT_REGISTER(name:%s)", name.c_str());

	_phaseCacheMap.erase(it);
}

IPhase* PhaseManager::GetRegisteredPhase(const std::string& name)
{
	auto it = _phaseCacheMap.find(name);
	if (it == _phaseCacheMap.end())
	{
		GAME_LOG_ERR("FAILED_TO_GET_REGISTERED_PHASE(name:%s)", name.c_str());
		return nullptr;
	}

	return it->second;
}