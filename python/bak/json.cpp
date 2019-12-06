/*******************************************
   Author        : Jun Zhang
   Email         : ewalker.zj@gmail.com
   Last Modified : 2018-12-19 19:38
   Filename      : json.cpp
   Description   : 
*******************************************/
#include<iostream>
#include<jsoncpp/json/json.h>

int main(){
    std::string data = "{\"id\":2,\"success\":\"true\",\"status\":true}";
    Json::Reader reader;
    Json::Value value;
    if(!reader.parse(data.c_str(), value)){
        std::cout << "json parse failed" << std::endl;
        return -1;
    }
    if(!value.isMember("size"))
        std::cout << "no size" << std::endl;

    //std::string size = value["size"].asString();
    //std::cout << "size: " << size << std::endl;
    bool status = value["status"].asBool();
    std::cout << "status: " << status << std::endl;
    Json::Value dv = value;
    value.removeMember("status");
    Json::FastWriter writer;
    std::string res = writer.write(value);
    std::cout << res << std::endl;
    return 0;
}
