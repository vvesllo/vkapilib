/*
   :::     :::                               :::::::       :::::::: 
  :+:     :+:                              :+:   :+:     :+:    :+: 
 +:+     +:+                              +:+   +:+           +:+   
+#+     +:+         +#++:++#++:++        +#+   +:+         +#+      
+#+   +#+                               +#+   +#+       +#+         
#+#+#+#                                #+#   #+# #+#  #+#           
 ###                                   #######  ### ##########      


*/

#include "include/vkapilib.h"

std::string replace(std::string text)
{
	std::string symbols = " +\n\t";
	std::string symbols_2[] = {"%20", "%2B", "%0A", "%09"};
	std::string new_text = "";
	for (size_t i = 0; i < strlen(text.c_str()); i++) {
		size_t pos = symbols.find(text[i]);
		if (pos != std::string::npos)
			new_text += symbols_2[pos];
		else
			new_text += text[i];
	}
	return new_text;
}

vkapilib::VKBot::VKBot(int group_id, std::string token, std::string v, std::string lang) : token(token), v(v), group_id(group_id), lang(lang)
{
	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_DEFAULT);
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_URL, ("https://api.vk.com/method/groups.getLongPollServer?group_id="+ std::to_string(group_id) +"&access_token=" + token + "&v="+v+"&lang="+lang).c_str());
	}
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cout << "Error! No.1" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}
	root.Parse(content.c_str());

	longpoll_key = root["response"]["key"].GetString();
	server = root["response"]["server"].GetString();
	ts = root["response"]["ts"].GetString();
}

vkapilib::VKBot::~VKBot() { curl_easy_cleanup(curl); }

int vkapilib::VKBot::peerId() { return std::stoi(peer_id); }

int vkapilib::VKBot::fromId() { return std::stoi(from_id); }

std::string vkapilib::VKBot::getMessage() { return message; }

std::string vkapilib::VKBot::call(std::string method_name, std::string (*params)[2], size_t len)
{
	content.clear();
	std::stringstream params_str;
	for (size_t i = 0; i < len; i++) {
		std::string str = params[i][1];
		str = replace(str);
		params_str << "&" << params[i][0] << "=" << str;
	}
	std::string url = "https://api.vk.com/method/"+method_name+"?"+params_str.str()+"&v="+v;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cout << "Error! No.2" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}
	// std::cout << content << std::endl;
	return content;
}

std::string vkapilib::VKBot::update()
{
	message.clear();
	content.clear();

	std::stringstream ss;
	ss << server << "?act=a_check&key=" << longpoll_key << "&ts=" << ts << "&wait=25";
	curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());
	res = curl_easy_perform(curl);

	root.Parse(content.c_str());
	if (res != CURLE_OK)
	{
		std::cout << "Error! No.3" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
		content.clear();
		curl_easy_setopt(curl, CURLOPT_URL, ("https://api.vk.com/method/groups.getLongPollServer?group_id="+ std::to_string(group_id) +"&access_token=" + token + "&v="+v+"&lang="+lang).c_str());
		res = curl_easy_perform(curl);
		root.Parse(content.c_str());
		std::cout << content << '\n';

		longpoll_key = root["response"]["key"].GetString();
		server = root["response"]["server"].GetString();
		ts = root["response"]["ts"].GetString();
		content.clear();
	}


	bool failed_find = root.FindMember("failed") == root.MemberEnd() ? false : true;
	if (!failed_find)
	{
		const rapidjson::Value& updates = root["updates"];
		ts = root["ts"].GetString();
		if (!updates.Empty())
		{
			std::string type = updates[0]["type"].GetString();
			if (type == "message_new")
			{
				message = updates[0]["object"]["message"]["text"].GetString();
				peer_id = std::to_string(updates[0]["object"]["message"]["peer_id"].GetInt());
				from_id = std::to_string(updates[0]["object"]["message"]["from_id"].GetInt());
			}
		}
	}
	else
	{
		std::cout << "failed\n";
		switch (root["failed"].GetInt())
		{
			case 1:
				{
					std::cout << "Failed No 1" << std::endl;
					ts = root["response"]["ts"].GetString();
					break;
				}
			case 2:
				{
					content.clear();
					std::cout << "Failed No 2" << std::endl;
					curl_easy_setopt(curl, CURLOPT_URL, ("https://api.vk.com/method/groups.getLongPollServer?group_id="+ std::to_string(group_id) +"&access_token=" + token + "&v="+v+"&lang="+lang).c_str());
					res = curl_easy_perform(curl);
					root.Parse(content.c_str());
					std::cout << content << '\n';

					longpoll_key = root["response"]["key"].GetString();
					server = root["response"]["server"].GetString();
					ts = root["response"]["ts"].GetString();
					content.clear();
					break;
				}
			case 3:
				{
					std::cout << "Failed No 3" << std::endl;
					break;
				}
		}
	}
	return content;
}

size_t vkapilib::VKBot::write_data(char* ptr, size_t size, size_t nmemb, std::string* data)
{
	if (data)
	{
		data->append(ptr, size * nmemb);
		return size * nmemb;
	}
	return 0;
}


// -------------------------------------------------------------------------------------------------------------------------
// VKAPI Functions
// -------------------------------------------------------------------------------------------------------------------------


vkapilib::VKAPI::VKAPI(std::string v) : v(v)
{
	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_DEFAULT);
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
	}
}

vkapilib::VKAPI::~VKAPI() { curl_easy_cleanup(curl); }

std::string vkapilib::VKAPI::call(std::string method_name, std::string (*params)[2], size_t len)
{
	content.clear();
	std::stringstream params_str;
	for (size_t i = 0; i < len; i++) {
		std::string str = params[i][1];
		str = replace(str);
		params_str << "&" << params[i][0] << "=" << str;
	}
	std::string url = "https://api.vk.com/method/"+method_name+"?"+params_str.str()+"&v="+v;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cout << "Error! No.2" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}

	return content;
}

size_t vkapilib::VKAPI::write_data(char* ptr, size_t size, size_t nmemb, std::string* data)
{
	if (data)
	{
		data->append(ptr, size * nmemb);
		return size * nmemb;
	}
	return 0;
}


