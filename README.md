# VK API Lib

Простая библиотека для работы с API ВК на C++

для создания бота (группа) можно использовать VKBot
```C++
// bot.cxx
#include "include/vkapilib.h"
#include <locale>

int main()
{
    std::locale loc;
    std::string token = "token";
    vkapilib::VKBot bot(194720349, token, "5.131", "ru");
    for(;;)
    {
        bot.update();
        if (bot.getMessage() == u8"пинг") {
            std::string params[][2] = {
                {"peer_id", std::to_string(bot.peerId())},
                {"message", "понг"},
                {"random_id", "0"},
                {"access_token", token}
            };
            bot.call("messages.send", params, sizeof(params)/sizeof(params[0]));
        }
    }
}
```

для отправки запросов на API можно использовать VKAPI
```C++
// getGroupMembers.cxx
#include "include/vkapilib.h"

int main()
{

    vkapilib::VKAPI api("5.131");
	std::string params[][2] = {
		{"count", "5"},
		{"access_token", "token"},
		{"group_id", "194720349"}
	};

    std::string response = api.call("groups.getMembers", params, sizeof(params)/sizeof(params[0]));
	
	std::cout << response << std::endl;

}
```

для создания бота (аккаунт) можно использовать VKUser
```C++
// soon :)
```
