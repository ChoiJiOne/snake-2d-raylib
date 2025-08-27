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
};