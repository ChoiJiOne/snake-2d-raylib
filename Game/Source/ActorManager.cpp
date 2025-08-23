#include "ActorManager.h"
#include "GameAssert.h"

void ActorManager::Startup()
{
	if (_isInitialized)
	{
		// [TODO] 여기에 로그 출력 필요.
		return;
	}

	_isInitialized = true;
}

void ActorManager::Shutdown()
{
	if (!_isInitialized)
	{
		// [TODO] 여기에 로그 출력 필요.
		return;
	}

	for (size_t idx = 0; idx < _actorPool.size(); ++idx)
	{
		if (_actorPool[idx].actor)
		{
			if (_actorPool[idx].actor->IsInitialized())
			{
				_actorPool[idx].actor->Release();
			}

			_actorPool[idx].actor.reset();
			_actorPool[idx].isOccupied = false;
		}
	}

	_isInitialized = false;
}

void ActorManager::Destroy(const IActor* actor)
{
	int32_t actorID = -1;
	for (size_t idx = 0; idx < _actorPool.size(); ++idx)
	{
		if (actor == _actorPool[idx].actor.get())
		{
			actorID = static_cast<int32_t>(idx);
			break;
		}
	}

	if (actorID != -1 && _actorPool[actorID].actor)
	{
		if (_actorPool[actorID].actor->IsInitialized())
		{
			_actorPool[actorID].actor->Release();
		}

		_actorPool[actorID].actor.reset();
		_actorPool[actorID].isOccupied = false;
	}
}