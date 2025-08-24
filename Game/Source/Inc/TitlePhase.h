#pragma once

#include <vector>

#include "IActor.h"
#include "IPhase.h"

class TitlePhase : public IPhase
{
public:
	TitlePhase() = default;
	virtual ~TitlePhase() = default;

	DISALLOW_COPY_AND_ASSIGN(TitlePhase);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual IPhase* GetTransitionPhase() override;
};