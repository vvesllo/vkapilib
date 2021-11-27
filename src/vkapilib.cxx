#include "include/vkapilib.h"

vkapilib::VKBot::VKBot(int group_id, std::string token, std::string v, std::string lang) : token(token)
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
		std::cout << "Error!" << std::endl;
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

void vkapilib::VKBot::send(int peer_id, std::string text)
{
	std::string url = "https://api.vk.com/method/messages.send?peer_id="+std::to_string(peer_id)+"&message="+text+"&random_id=0&access_token="+token+"&v=5.131";
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	res = curl_easy_perform(curl);

	if (res != CURLE_OK)
	{
		std::cout << "Error!" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}
}

void vkapilib::VKBot::update()
{
	message.clear();
	std::stringstream ss;
	ss << server << "?act=a_check&key=" << longpoll_key << "&ts=" << ts << "&wait=1";
	curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());
	res = curl_easy_perform(curl);
	root.Parse(content.c_str());
	if (res != CURLE_OK)
	{
		std::cout << "Error!" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}
	const Value& updates = root["updates"];
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
size_t vkapilib::VKBot::write_data(char* ptr, size_t size, size_t nmemb, std::string* data)
{
	if (data)
	{
		data->clear();
		data->append(ptr, size * nmemb);
		return size * nmemb;
	}
	return 0;
}
