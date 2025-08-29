#pragma once

#include "Macro.h"

class IPhase
{
public:
	enum class EActionState
	{
		NONE = 0x00,
		EXIT_PHASE = 0x01, // 페이즈 탈출
		QUIT_LOOP = 0x02, // 루프 종료
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

	const EActionState& GetActionState() const { return _actionState; }

protected:
	EActionState _actionState = EActionState::NONE;
};