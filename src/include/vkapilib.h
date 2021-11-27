#pragma once

#include <sstream>
#include <string>
#include <iostream>
#include <curl\curl.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;

namespace vkapilib
{
    class VKBot
    {
    public:
        VKBot(int group_id, std::string token, std::string v, std::string lang);
    	~VKBot();
        void update();
    	int peerId();
    	int fromId();
        void send(int peer_id, std::string text);
    	std::string getMessage();

    private:
        static size_t write_data(char* ptr, size_t size, size_t nmemb, std::string* data);

        CURL* curl;
        CURLcode res;
        Document root;

        std::string content;
        std::string token;
        std::string message;
        std::string longpoll_key;
        std::string server;
        std::string ts;
        std::string peer_id;
        std::string from_id;

    };
    class VKUser
    {
        //soon :)
    };
}
