#pragma once

#include "Macro.h"

class IPhase
{
public:
	enum class EState
	{
		IDLE,
		LOOP,
	};

	enum class EActionState
	{
		IDLE  = 0,
		ENTER = 1,
		EXIT  = 2,
	};

public:
	IPhase() = default;
	virtual ~IPhase() = default;

	DISALLOW_COPY_AND_ASSIGN(IPhase);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Render() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual IPhase* GetTransitionPhase() = 0;

	EState GetState() { return _state; }
	EActionState GetActionState() { return _actionState; }

protected:
	EState _state = EState::IDLE;
	EActionState _actionState = EActionState::IDLE;
};