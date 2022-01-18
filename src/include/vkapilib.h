#pragma once

#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include "rapidjson/document.h"

namespace vkapilib
{
    class VKBot
    {
    public:
        VKBot(int group_id, std::string token, std::string v, std::string lang);
    	~VKBot();
    	int peerId();
    	int fromId();
        std::string update();
        std::string call(std::string method_name, std::map<std::string, std::string> params);
    	std::string getMessage();

    private:
        static size_t write_data(char* ptr, size_t size, size_t nmemb, std::string* data);

        CURL* curl;
        CURLcode res;
        rapidjson::Document root;
        std::string content;
        std::string token;
        std::string message;
        std::string longpoll_key;
        std::string server;
        std::string ts;
        std::string v;
        int group_id;
        std::string lang;
        std::string peer_id;
        std::string from_id;

    };
    class VKUser
    {
        //soon :)
    };
    class VKAPI
    {
    public:
    	VKAPI(std::string v);
    	~VKAPI();
        std::string call(std::string method_name, std::map<std::string, std::string> params);
    private:
        static size_t write_data(char* ptr, size_t size, size_t nmemb, std::string* data);

        std::string content;
        std::string v;
        CURL* curl;
        CURLcode res;
    };
}
