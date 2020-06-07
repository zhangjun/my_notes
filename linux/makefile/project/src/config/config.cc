#include <string>
#include <fstream>
#include <sstream>
#include <google/protobuf/text_format.h>
//#include <workflow/UpstreamManager.h>

#include "src/config/config.h"

namespace ss {

bool Config::Load(const char* config_file) {
	std::ifstream fin;
	fin.open(config_file);
	if(fin.fail()) {
		return false;
	}
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string content = ss.str();
	if(!google::protobuf::TextFormat::ParseFromString(content, &server_config_)) {
		return false;
	}

	LoadUpstream();
	return true;
}

bool Config::LoadUpstream() {
	ss::Backend* cache = server_config_.mutable_cache();
	// load cache
	////UpstreamManager::upstream_create(cache -> upstream_name(), nullptr); 
	//UpstreamManager::upstream_create_consistent_hash(cache -> upstream_name(), nullptr); 
	//for(int i = 0; i < cache -> servers().size(); i ++) {
	//	UpstreamManager::upstream_add_server(cache -> upstream_name(), cache -> servers(i)); 
	//}

	//ss::HintBackend *hint = server_config_.mutable_hint();
	//UpstreamManager::upstream_create_weighted_random(hint->upstream_name(), true);
	//for(int i=0; i<hint->servers().size(); i++)
	//{
	//	UpstreamManager::upstream_add_server(hint->upstream_name(), hint->servers(i));
	//}

	//ss::BlacklistBackend *blacklist = server_config_.mutable_blacklist();
	//UpstreamManager::upstream_create_weighted_random(blacklist->upstream_name(), true);
	//for(int i = 0; i < blacklist->servers().size(); i++)
	//{
	//	UpstreamManager::upstream_add_server(blacklist->upstream_name(), blacklist->servers(i));
	//}

	return true;
}

}
