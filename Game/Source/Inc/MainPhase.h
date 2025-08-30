#pragma once

#include <vector>

#include "IActor.h"
#include "IPhase.h"

class MainPhase : public IPhase
{
public:
	MainPhase() = default;
	virtual ~MainPhase() = default;

	DISALLOW_COPY_AND_ASSIGN(MainPhase);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual IPhase* GetTransitionPhase() override;

	void SetGameOver(bool isOver) { _isOver = isOver; }

private:
	bool _isOver;
	IPhase* _transitionPhase = nullptr;

	std::vector<IActor*> _phaseActors;
	std::vector<IActor*> _tickActors;
	std::vector<IActor*> _renderActors;
};