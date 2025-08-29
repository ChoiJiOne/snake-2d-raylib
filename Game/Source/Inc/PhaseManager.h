#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "IPhase.h"
#include "IManager.h"

class PhaseManager : public IManager<PhaseManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(PhaseManager);

	void Startup();
	virtual void Shutdown() override;

    template <typename TPhase, typename... Args>
    TPhase* Create(Args&&... args)
    {
        int32_t phaseID = -1;
        for (size_t idx = 0; idx < _phasePool.size(); ++idx)
        {
            if (!_phasePool[idx].phase && !_phasePool[idx].isOccupied)
            {
                phaseID = idx;
                break;
            }
        }

        if (phaseID == -1)
        {
            _phasePool.emplace_back();
            phaseID = static_cast<int32_t>(_phasePool.size()) - 1;
        }

        _phasePool[phaseID].isOccupied = true;
        _phasePool[phaseID].phase = std::make_unique<TPhase>(args...);

        return static_cast<TPhase*>(_phasePool[phaseID].phase.get());
    }

    void Destroy(const IPhase* phase);

    void Register(const std::string& name, IPhase* phase);
    void Unregister(const std::string& name);
    IPhase* GetRegisteredPhase(const std::string& name);

private:
	friend class IManager<PhaseManager>;

	PhaseManager() = default;
	virtual ~PhaseManager() = default;

	struct PhasePoolSlot
	{
		std::unique_ptr<IPhase> phase;
		bool isOccupied = false;
	};

private:
	std::vector<PhasePoolSlot> _phasePool;
    std::map<std::string, IPhase*> _phaseCacheMap;
};