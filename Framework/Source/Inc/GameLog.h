#pragma once

#include <raylib.h>

#ifndef GAME_LOG
#define GAME_LOG_TRACE(...) TraceLog(LOG_TRACE, __VA_ARGS__)
#define GAME_LOG_DEBUG(...) TraceLog(LOG_DEBUG, __VA_ARGS__)
#define GAME_LOG_INFO(...) TraceLog(LOG_INFO, __VA_ARGS__)
#define GAME_LOG_WARN(...) TraceLog(LOG_WARNING, __VA_ARGS__)
#define GAME_LOG_ERR(...) TraceLog(LOG_ERROR, __VA_ARGS__)
#define GAME_LOG_FATAL(...) TraceLog(LOG_FATAL, __VA_ARGS__)
#endif