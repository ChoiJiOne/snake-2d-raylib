#pragma once

#include "Macro.h"

template <typename T>
class IManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(IManager);

	static T& Get() { return _instance; }
	static T* GetPtr() { return &_instance; }

	bool IsInitialized() { return _isInitialized; }

	virtual void Shutdown() = 0;

protected:
	IManager() = default;
	virtual ~IManager() = default;

protected:
	bool _isInitialized = false;

private:
	static T _instance;
};

#include "IManager.inl"