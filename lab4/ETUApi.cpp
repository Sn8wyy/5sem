#include "ETUApi.h"

ETUApi::NowDay ETUApi::get_now_day() {
	NowDay now_day;
	Client сlient;
	nlohmann::json j{};

	const std::string URL = MAIN_API + "semester";
	j = nlohmann::json::parse(сlient.get_request("", URL));
	std::string d = j["startDate"];

	struct std::tm a = { atoi(d.substr(17, 2).c_str()),atoi(d.substr(14, 2).c_str()),atoi(d.substr(11, 2).c_str()),
		atoi(d.substr(8, 2).c_str()), atoi(d.substr(5, 2).c_str()) - 1, atoi(d.substr(0, 4).c_str()) - 1900 };
	std::time_t start_semester = std::mktime(&a);
	std::time_t date_today = std::time(0);   // get time now
	
	double intpart;
	now_day.day = floor(7 * modf((std::difftime(date_today, start_semester)) / (60 * 60 * 24 * 7), &intpart));
	now_day.week = 1 + (int)floor(std::difftime(date_today, start_semester) / (60 * 60 * 24 * 7)) % 2;

	return now_day;
}

std::string ETUApi::get_near_lesson(int group_number) {
	NowDay now_day = get_now_day();
	SYSTEMTIME st;
	GetLocalTime(&st);

	Client сlient;
	std::string URL = this->MAIN_API + "schedule?groupNumber=" + std::to_string(group_number);
	std::string json = сlient.get_request("", URL);

	nlohmann::json j = nlohmann::json::parse(json);
	std::string info;

	bool find_lesson = false;
	if (!j[std::to_string(group_number)]["days"][std::to_string(now_day.day)]["lessons"].empty()) {
		for (auto& lesson : j[std::to_string(group_number)]["days"][std::to_string(now_day.day)]["lessons"]) {
			if (!find_lesson && lesson["end_time_seconds"] >= st.wHour * 60 * 60 + st.wMinute * 60 + st.wSecond 
				&& lesson["week"] == std::to_string(now_day.week)) {
				find_lesson = true;
				info += get_lesson(lesson);
			}
		}
	}
	if (!find_lesson) {
		++now_day.day;
		while (!find_lesson && now_day.day <= 6) {
			for (auto& lesson : j[std::to_string(group_number)]["days"][std::to_string(now_day.day)]["lessons"]) {
				if (!find_lesson && lesson["week"] == std::to_string(now_day.week)) { 
					find_lesson = true;
					info += get_lesson(lesson);
				}
			}
			++now_day.day;
		}
	}
	if (!find_lesson) {
		now_day.day = 0;
		now_day.week = now_day.week == 2 ? 1 : 2;
		while (!find_lesson && now_day.day < st.wDay) {
			for (auto& lesson : j[std::to_string(group_number)]["days"][std::to_string(now_day.day)]["lessons"]) {
				if (!find_lesson && lesson["week"] == std::to_string(now_day.week)) {
					find_lesson = true;
					info += get_lesson(lesson);
				}
			}
			++now_day.day;
		}
	}
	return info;
}

std::string ETUApi::get_tomorrow_schedule(int group_number) {
	NowDay now_day = get_now_day();
	if (now_day.day == 6) {
		now_day.day = 0;
		now_day.week = now_day.week == 2 ? 1 : 2;
	}
	else ++now_day.day;

	return get_day_schedule(group_number, now_day.week, now_day.day);
}

std::string ETUApi::get_week_schedule(int group_number, int week_number) {
	Client сlient;
	std::string URL = this->MAIN_API + "schedule?groupNumber=" + std::to_string(group_number);
	std::string json = сlient.get_request("", URL);

	nlohmann::json j = nlohmann::json::parse(json);
	std::string info;

	for (int i = 0; i < 7; ++i) {
		info += day_week[i] + '\n' + '\n';
		info += get_day(j[std::to_string(group_number)]["days"][std::to_string(i)]["lessons"], week_number);
	}
	return info;
}

std::string ETUApi::get_day_schedule(int group_number, int week_number, int day_number) {
	Client сlient;
	std::string URL = this->MAIN_API + "schedule?groupNumber=" + std::to_string(group_number);
	std::string json = сlient.get_request("", URL);

	nlohmann::json j = nlohmann::json::parse(json);
	std::string info;

	info += day_week[day_number] + '\n' + '\n';
	info += get_day(j[std::to_string(group_number)]["days"][std::to_string(day_number)]["lessons"], week_number);

	return info;
}

std::string ETUApi::get_day(nlohmann::json day, int week_number) {
	std::string info;
	for (auto& lesson : day) {
		if (lesson["week"] == std::to_string(week_number)) { 
			info += get_lesson(lesson); 
		}
	}
	return info;
}

std::string ETUApi::get_lesson(nlohmann::json j) {
	std::string info;
	info += "Время: " + j["start_time"].dump().substr(1, j["start_time"].dump().size() - 2) + '\n';
	info += "Дисциплина: " + j["name"].dump().substr(1, j["name"].dump().size() - 2) + '\n';
	info += "Тип: " + lesson_type.at(j["subjectType"].dump().substr(1, j["subjectType"].dump().size() - 2)) + '\n';
	info += "Преподаватель: " + j["teacher"].dump().substr(1, j["teacher"].dump().size() - 2) + '\n';
	if(j["second_teacher"].dump() != "\"\"") 
		info += "Преподаватель: " + j["second_teacher"].dump().substr(1, j["second_teacher"].dump().size() - 2) + '\n';
	info += "Аудитория: " + j["room"].dump().substr(1, j["room"].dump().size() - 2) + '\n';
	info += "Номер недели: " + j["week"].dump().substr(1, j["week"].dump().size() - 2) + '\n';
	info += '\n';
	return info;
}