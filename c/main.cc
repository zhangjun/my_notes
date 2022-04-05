#include "log.h"

int main() {
  LOG_DEBUG("hello %s\n", "debug");
  LOG_INFO("hello %s\n", "info");
  LOG_WARN("hello %s\n", "warn");
  return 0;
}
