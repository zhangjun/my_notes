/*************************************************************************
	> File Name: topic_util.h
	> Author: Jun Zhang
	> Mail: ewalker.zj@gmail.com 
	> Created Time: Fri 10 Nov 2017 04:31:18 PM CST
 ************************************************************************/
#ifndef _TOPIC_UTIL_H
#define _TOPIC_UTIL_H

#include<string>
#include<vector>
#include<ctime>
#include<sstream>


void trim(std::string &s){
	if(s.empty())
		return;

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
}

//void split(std::string s, const char* delimiter, std::vector<std::string>& tokens){
//	tokens.clear();
//	char *p;
//	char *buf = const_cast<char*>(s.c_str());
//	p = strtok(buf, delimiter);
//	while(p != NULL){
//		tokens.push_back(p);
//		p = strtok(NULL, delimiter);
//	}
//}

void split(const std::string s, const char* delimiter, std::vector<std::string>& tokens){
    tokens.clear();
    size_t last = 0;
    size_t index = s.find_first_of(delimiter, last);
    while(index != std::string::npos){
        tokens.push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(delimiter, last);
    }

    if(index - last > 0)
        tokens.push_back(s.substr(last, index - last));
}

std::string int2str(unsigned long val){
    std::stringstream ss;
    ss << val;
    return ss.str();
}

uint64_t str2int(std::string s){
    uint64_t val;
    std::stringstream ss(s);
    ss >> val;
    return val;
}


time_t getTimestamp(const char timestamp[]){
    struct tm t;
    memset(&t, 0, sizeof(tm));

    sscanf(timestamp, "%d-%d-%d %d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
    
    t.tm_year -= 1900;
    t.tm_mon --;
    
    return mktime(&t);
}




#endif

