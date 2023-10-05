#pragma once
#include "Client.h"

#include <conio.h>
#include <iostream>
#include <vector>
#include <string>

class WikiApi {
public:
	void ñreate_request(std::string path);

private:
	void selection_menu(std::string path);
	void input_request();
	void remove_substrs(std::string& s, std::string p);
	void read_json(std::string& s, std::vector<std::pair<std::string, std::string>> feilds);
	const std::string MAIN_URL = 
		"https://ru.wikipedia.org/w/api.php?action=query&list=search&utf8=&format=json&srsearch=";
	const std::string RESULT_PAGE_URL = 
		"https://ru.wikipedia.org/w/index.php?curid=";

	std::vector<std::vector<std::string>> sroffset;
	std::string input;
	std::string json;
};

