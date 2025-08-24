#pragma once

#include <raylib.h>

#if defined(DEBUG_MODE) || defined(RELWITHDEBINFO_MODE) || defined(RELEASE_MODE)
#ifndef GAME_CHECK
#define GAME_CHECK(EXP)\
{\
	if (!(bool)(EXP))\
	{\
		TraceLog(LOG_ERROR, "Assertion check point failed!\n> File: %s\n> Line: %d\n> Function: %s\n> Expression: %s", __FILE__, __LINE__, __func__, #EXP);\
		__debugbreak();\
	}\
}
#endif
#ifndef GAME_ASSERT
#define GAME_ASSERT(EXP, ...)\
{\
	if (!(bool)(EXP))\
	{\
		TraceLog(LOG_ERROR, "Assertion check point failed!\n> File: %s\n> Line: %d\n> Function: %s\n> Expression: %s", __FILE__, __LINE__, __func__, #EXP);\
		TraceLog(LOG_ERROR, __VA_ARGS__);\
		__debugbreak();\
	}\
}
#endif
#else // defined(MINSIZEREL_MODE)
#ifndef GAME_CHECK
#define GAME_CHECK(EXP) ((void)(EXP))
#endif
#ifndef GAME_ASSERT
#define GAME_ASSERT(EXP, ...) ((void)(EXP))
#endif
#endif