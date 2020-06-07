#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

namespace ss {

static const unsigned MAX_LOGBUF_LEN = 63 << 10;

typedef int func(char*, ...);

typedef enum LOG_LEVEL {
	TRACE = 1,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL
} LOG_LEVEL_T;

typedef LOG_LEVEL_T log_level_t;

extern log_level_t log_level_;

void Log(log_level_t level, const char* fmt, ...);

void SetLogLevel(log_level_t level);

}

#define LOG_TRACE(...) Log(ss::TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) Log(ss::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) Log(ss::INFO, __VA_ARGS__)
#define LOG_WARN(...) Log(ss::WARN, __VA_ARGS__)
#define LOG_ERROR(...) Log(ss::ERROR, __VA_ARGS__)
#define LOG_FATAL(...) Log(ss::FATAL, __VA_ARGS__)
