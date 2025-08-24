#pragma once

#include "Macro.h"

class IActor
{
public:
	IActor() = default;
	virtual ~IActor() = default;

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	bool IsInitialized() { return _isInitialized; }

protected:
	bool _isInitialized = false;
};