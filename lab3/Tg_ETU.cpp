#include "Tg_ETU.h"

Tg_ETU::Tg_ETU(std::string token) {
    TgBot::Bot bot(token);
    ETUApi etu;
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        std::string greetings;
        greetings += "Команды:\n";
        greetings += "near_lesson GROUP_NUMBER - ближайшее занятие для указанной группы\n";
        greetings += "Пример: near_lesson 1301\n\n";
        greetings += "DAY WEEK_NUMBER GROUP_NUMBER - расписание занятий в указанный день (monday, thuesday, ...)\n";
        greetings += "Пример: friday 2 1301\n\n";
        greetings += "tomorrow GROUP_NUMBER - расписание на следующий день\n";
        greetings += "Пример: tomorrow 1301\n\n";
        greetings += "all WEEK_NUMBER GROUP_NUMBER - расписание на всю неделю\n";
        greetings += "Пример: all 1 1301\n";
        bot.getApi().sendMessage(message->chat->id, greetings);
        });
    bot.getEvents().onAnyMessage([this, &etu, &bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        try {
            bool done = false;
            std::transform(message->text.begin(),message->text.end(),message->text.begin(),::tolower); 
            if (message->text.find("near_lesson") == 0) {
                int group_number = atoi(message->text.substr(message->text.substr().size() - 4, message->text.substr().size() - 1).c_str());
                std::string response = etu.get_near_lesson(group_number);
                bot.getApi().sendMessage(message->chat->id, response);
                done = true;
            }
            if (message->text.find("tomorrow") == 0) {
                int group_number = atoi(message->text.substr(message->text.substr().size() - 4, message->text.substr().size() - 1).c_str());
                std::string response = etu.get_tomorrow_schedule(group_number);
                bot.getApi().sendMessage(message->chat->id, response);
                done = true;
            }
            if (message->text.find("all") == 0) {
                int group_number = atoi(message->text.substr(message->text.substr().size() - 4, message->text.substr().size() - 1).c_str());
                int week_number = atoi(message->text.substr(message->text.substr().size() - 6, 1).c_str());
                std::string response = etu.get_week_schedule(group_number, week_number);
                bot.getApi().sendMessage(message->chat->id, response);
                done = true;
            }
            std::regex rx("([a-z]+) ([0-9]) ([0-9]+)");
            std::match_results< std::string::const_iterator> mr;
            if (std::regex_search(message->text, mr, rx) && mr.str(1) != "all") {
                int group_number = atoi(mr.str(3).c_str());
                int week_number = atoi(mr.str(2).c_str());
                int day_number = day_week_number[mr.str(1)];
                std::string response = etu.get_day_schedule(group_number, week_number, day_number);
                bot.getApi().sendMessage(message->chat->id, response);
                done = true;
            }
            if (!done) throw std::exception();
        }
        catch (std::exception& e) {
            bot.getApi().sendMessage(message->chat->id, "Неверная команда\n");
        }
        });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
}
