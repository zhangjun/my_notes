#pragma once 

#include "src/common/log.h"

class Logger {
public:
	Logger() {}
	bool Init(ss::log_level_t level) {
		ss::SetLogLevel(level);
		return true;
	}
};
