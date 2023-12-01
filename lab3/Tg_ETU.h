#pragma once
#include "ETUApi.h"

#include <stdio.h>
#include <tgbot/tgbot.h>
#include <string>
#include <regex>
#include <unordered_map>

class Tg_ETU {
public:
    Tg_ETU(std::string token);
private:
    std::unordered_map<std::string, int> day_week_number {
        {"monday", 0},{"tuesday", 1},{"wednesday", 2},
        {"thursday", 3}, {"friday", 4},{"saturday", 5},{"sunday", 6}, };
};

