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
