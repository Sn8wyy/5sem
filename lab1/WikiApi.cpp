#include "WikiApi.h"

void WikiApi::сreate_request(std::string browser_path) {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	try {
		input_request();

		Client сlient;

		json = сlient.get_request(input, MAIN_URL);

		remove_substrs(json, "<span class=\\\"searchmatch\\\">");
		remove_substrs(json, "</span>");

		read_json(json, { {"\"title\":", ",\"pageid\":"}, {"\"pageid\":", ",\"size\":"} });
		
		selection_menu(browser_path);
	} catch (std::string& error) {
		setlocale(LC_ALL, "ru");
		std::cout << error << '\n';
		std::cout << "Нажмите на любую кнопку для выхода\n";
		_getch();
	}

}

void WikiApi::input_request() {
	std::cout << "Введите запрос\n";
	while (std::getline(std::cin, this->input) && this->input.length() == 0) {
		std::cout << "Пустая строка. Введите снова\n";
	}
}

void WikiApi::selection_menu(std::string browser_path) {
	setlocale(LC_ALL, "ru");
	if (sroffset.size() == 0) {
		std::cout << "По вашему запросу ничего не найдено\n";
		std::cout << "Нажмите на любую кнопку для выхода\n";
		_getch();
		return;
	}
	
	std::cout << "Выберете какую страницу открыть\n";
	std::cout << "0 - выход\n";

	setlocale(LC_ALL, "ru_RU.UTF-8");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	for (int i = 1; i <= sroffset.size(); ++i) {
		std::cout << i << " - " << sroffset[i - 1][0] << '\n';
	}

	int choise = -1;
	std::string selection;
	setlocale(LC_ALL, "ru");
	while (choise == -1) {
		std::getline(std::cin, selection);
		if (selection.length() == 0)  std::cout << "Попытка ввести пустую строку. Введите номер\n";
		else try {
			choise = std::stoi(selection);
			if (choise < 0 || choise > sroffset.size() || (choise == 0 && selection.length() != 1) ||
				(choise != 0 && std::ceil(std::log10(choise + 1)) != selection.length())) {
				std::cout << "Неверный номер. Введите снова\n";
				choise = -1;
			}
		} catch (std::invalid_argument e) {
			std::cout << "Ошибка ввода. Введите номер\n";
		}
	}
	if (choise != 0) {
		std::string op = "start " + browser_path + " -new " + RESULT_PAGE_URL + sroffset[choise - 1][1];
		system(op.c_str());
		exit(0);
	}
}

void WikiApi::remove_substrs(std::string& s, std::string p) {
	std::string::size_type n = p.length();
	for (std::string::size_type i = s.find(p); i != std::string::npos; i = s.find(p)) s.erase(i, n);
}

void WikiApi::read_json(std::string& s, std::vector<std::pair<std::string, std::string>> feilds) {
	// поиск подстрок начиная с места после feilds[].first до места начала feilds[].second не включая его
	
	this->sroffset = {};
	std::string::size_type i = 0, j = 0;
	for (i = s.find(feilds.begin()->first, i); i != std::string::npos; i = s.find(feilds.begin()->first, i)) {
		j = s.find(feilds.begin()->second, j);

		sroffset.push_back(std::vector<std::string>(feilds.size()));
		(*sroffset.rbegin())[0] = s.substr(i + feilds.begin()->first.size(), j - (i + feilds.begin()->first.size()));

		for (int k = 1; k < feilds.size(); ++k) {
			i = s.find(feilds[k].first, i);
			j = s.find(feilds[k].second, j);
			(*sroffset.rbegin())[k] = s.substr(i + feilds[k].first.size(), j - (i + feilds[k].first.size()));
		}
	}
	++i; ++j;
}

	
