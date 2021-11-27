#include "include/vkapilib.h"
#include <locale>

int main()
{
    std::locale loc;
    vkapilib::VKBot bot(1, "token", "5.131", "ru");
    for(;;)
    {
        bot.update();
        if (bot.getMessage() == u8"пинг") {
            bot.send(bot.peerId(), "pong");
        }
    }
    system("pause");
}
