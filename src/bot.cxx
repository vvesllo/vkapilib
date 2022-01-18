#include "include/vkapilib.h"
#include <locale>

int main()
{
    std::locale loc;
    std::string token = "token";
    vkapilib::VKBot bot(-1, token, "5.131", "ru");
    for(;;)
    {
        bot.update();
        if (bot.getMessage() == u8"пинг") {
            bot.call("messages.send", std::map<std::string, std::string> {
                {"peer_id", std::to_string(bot.peerId())},
                {"message", "понг"},
                {"random_id", "0"}
            });
        }
    }
}
