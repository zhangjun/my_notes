#pragma once

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

namespace log {

static const unsigned MAX_LOGBUF_LEN = 63 << 10;
#define COL_CYANB "\033[1;36m"
#define COL_END "\033[0m"

typedef enum LOG_LEVEL {
  TRACE = 1,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL
} LOG_LEVEL_T;

typedef LOG_LEVEL_T log_level_t;

static __thread char buffer[MAX_LOGBUF_LEN];

static log_level_t log_level_ = INFO;

static const char *LevelStr(log_level_t level) {
  switch (level) {
  case TRACE:
    return "TRACE";
  case DEBUG:
    return "DEBUG";
  case INFO:
    return "INFO";
  case WARN:
    return "WARN";
  case ERROR:
    return "ERROR";
  case FATAL:
    return "FATAL";
  default:
    return "UNKOWN";
  }
}

static bool CheckLevel(log_level_t level) {
  if (level >= log_level_)
    return true;
  else
    return false;
}

// https://github.com/0xmalloc/c-log/blob/master/src/log.c
// https://github.com/LeechanX/Ring-Log/blob/master/src/rlog.cc
inline int LogPrefix(char *buffer, log_level_t level) {
  time_t now;
  now = time(&now) + 8 * 3600;
  struct tm t;
  localtime_r(&now, &t);
  return snprintf(buffer, MAX_LOGBUF_LEN,
                  COL_CYANB "[%s]" COL_END "[%02d%02d%02d %02d:%02d:%02d]",
                  LevelStr(level), t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                  t.tm_hour, t.tm_min, t.tm_sec);
}

static void SetLogLevel(log_level_t level) { log_level_ = level; }

static void Log(log_level_t level, const char *fmt, ...) {
  if (!CheckLevel(level))
    return;

  int size = 0;
  va_list ap;
  va_start(ap, fmt);
  size += LogPrefix(buffer, level);
  size += vsnprintf(buffer + size, sizeof(buffer) - size, fmt, ap);
  va_end(ap);
  fprintf(stderr, "%s", buffer);
}

} // namespace log

#define LOG_TRACE(...) Log(log::TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) Log(log::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) Log(log::INFO, __VA_ARGS__)
#define LOG_WARN(...) Log(log::WARN, __VA_ARGS__)
#define LOG_ERROR(...) Log(log::ERROR, __VA_ARGS__)
#define LOG_FATAL(...) Log(log::FATAL, __VA_ARGS__)
