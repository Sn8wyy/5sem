#pragma once

#pragma execution_character_set("utf-8")
#include "Client.h"
#include "nlohmann/json.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class ETUApi {
public:
	std::string get_near_lesson(int group_number);
	std::string get_tomorrow_schedule(int group_number);
	std::string get_week_schedule(int group_number, int week_number);
	std::string get_day_schedule(int group_number, int week_number, int day_number);
	
private:
	const std::string MAIN_API = "https://digital.etu.ru/api/mobile/";
	const std::unordered_map<std::string, std::string> lesson_type{
		{"Лек", "Лекция"}, {"Пр", "Практика"}, {"Лаб", "Лабораторная"} };
	const std::vector <std::string> day_week = {"Понедельник", "Вторник", 
		"Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
	struct NowDay {
		int day; // 0..6
		int week; // 1..2
	};
	NowDay get_now_day();
	std::string get_day(nlohmann::json day, int week_number);
	std::string get_lesson(nlohmann::json j);
};

