#pragma once 

#include "src/proto/server_config.pb.h"

namespace ss {


class Config {
public:
	static Config* instance() {
		static Config cfg;
		return &cfg;
	}

	bool Load(const char* config_file);
	bool LoadUpstream();

public:
	ss::ServerConfig server_config_;
};

}
