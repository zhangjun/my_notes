#include <getopt.h>
#include "src/logger.h"
#include "src/config/config.h"
#include "src/kernel/kernel.h"
#include <iostream>


int main(int argc, char* argv[]) {

	std::string config_file = "conf/config.cfg";
	ss::log_level_t log_level = ss::INFO;

	char ch;
	while((ch = getopt(argc, argv, "hdc:")) != -1) {
		switch(ch) {
		case 'c':
			config_file = optarg;
			break;
		case 'd':
			log_level = ss::DEBUG;
			break;
		case 'h':
			std::cout << "usage: " << argv[0] << " -c " << config_file << std::endl;
			return 0;
		default:
			break;
		}
	}

	ss::Config* cfg = ss::Config::instance();
	bool status = cfg -> Load(config_file.c_str());
	if(!status) {
		std::cout << "parse config fail" << std::endl;
		return -1;
	}
	ss::ServerConfig* sc = &cfg -> server_config_;

	Logger* logger = new Logger();
	logger -> Init(ss::INFO);

	ss::Kernel* k = new ss::Kernel();
	k -> stat();
	
	delete logger;
	return 0;
}
