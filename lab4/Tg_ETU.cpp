#include "Tg_ETU.h"

Tg_ETU::Tg_ETU(std::string token, std::ostringstream& conn_string) {
    TgBot::Bot bot(token);
    ETUApi etu;
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        std::string greetings;
        greetings += "Команды:\n";
        greetings += "near_lesson GROUP_NUMBER - ближайшее занятие для указанной группы\n";
        greetings += "Пример: near_lesson 1301\n\n";
        greetings += "DAY WEEK_NUMBER GROUP_NUMBER - расписание занятий в указанный день (monday, tuesday, ...)\n";
        greetings += "Пример: friday 2 1301\n\n";
        greetings += "tomorrow GROUP_NUMBER - расписание на следующий день\n";
        greetings += "Пример: tomorrow 1301\n\n";
        greetings += "all WEEK_NUMBER GROUP_NUMBER - расписание на всю неделю\n";
        greetings += "Пример: all 1 1301\n";

        bot.getApi().sendMessage(message->chat->id, greetings);
        });
    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        if (StringTools::startsWith(message->text, "/last_command")) {
            try {
                std::string query;
                pqxx::connection c(conn_string.str());
                pqxx::work w(c);
                query = "";
                query += "Select count(*) ";
                query += "FROM public.\"LastCommand\" as t1 ";
                query += "Where t1.\"UserId\" = '";
                query += message->chat->username;
                query += '\'';
                pqxx::row r = w.exec1(query);
                w.commit();
                if (r[0].as<bool>()) {
                    pqxx::work w(c);
                    query = "";
                    query += "SELECT t1.\"Command\" ";
                    query += "FROM public.\"LastCommand\" as t1 ";
                    query += "Where t1.\"UserId\" = '";
                    query += message->chat->username;
                    query += '\'';
                    pqxx::row r = w.exec1(query);
                    w.commit();
                    bot.getApi().sendMessage(message->chat->id, "Ваша прошлая команда:\n" + r[0].as<std::string>() + '\n');
                    message->text = r[0].as<std::string>();
                } else {
                    bot.getApi().sendMessage(message->chat->id, "Ни одна команда ещё не вводилась\n");
                    return;
                }
            } catch (std::exception const& e) {
                printf("error: %s\n", e.what());
            }
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
                int day_number;
                if (day_week_number.find(mr.str(1)) != day_week_number.end()) {
                    day_number = day_week_number[mr.str(1)];
                } else throw std::exception();
                std::string response = etu.get_day_schedule(group_number, week_number, day_number);
                bot.getApi().sendMessage(message->chat->id, response);
                done = true;
            }
            if (done) {
                std::string query;
                pqxx::connection c(conn_string.str());
                pqxx::work w(c);
                query = "";
                query += "Select count(*) ";
                query += "FROM public.\"LastCommand\" as t1 ";
                query += "Where t1.\"UserId\" = '";
                query += message->chat->username;
                query += '\'';
                pqxx::row r = w.exec1(query);
                w.commit();
                if (r[0].as<bool>()) {
                    pqxx::work w(c);
                    query = "";
                    query += "UPDATE public.\"LastCommand\" ";
                    query += "SET \"Command\" = '";
                    query += message->text;
                    query += "' ";
                    query += "WHERE \"UserId\" = '";
                    query += message->chat->username;
                    query += '\'';
                    w.exec0(query);
                    w.commit();
                } else {
                    pqxx::work w(c);
                    query = "";
                    query += "INSERT INTO public.\"LastCommand\" (\"UserId\", \"Command\") ";
                    query += "VALUES('";
                    query += message->chat->username;
                    query += "', '";
                    query += message->text;
                    query += "')";
                    w.exec0(query);
                    w.commit();
                }
            } else throw std::exception();
        }
        catch (std::exception& e) {
            bot.getApi().sendMessage(message->chat->id, "Неверная команда\n");
            printf("error: %s\n", e.what());
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
