#include "Client.h"

std::string Client::get_request(std::string input, std::string MAIN_URL) {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	std::string result;

	CURL* curl;
	CURLcode response;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {
		char* percent_encoding = curl_easy_escape(curl, input.c_str(), strlen(input.c_str()));
		if (!percent_encoding) {
			curl_easy_cleanup(curl);
			throw std::string("Ошибка преобразования url");
		}
		MAIN_URL += percent_encoding;
		curl_easy_setopt(curl, CURLOPT_URL, MAIN_URL.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_response_tostring);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		response = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		curl_free(percent_encoding);

		if (CURLE_OK != response) {
			throw std::string("Ошибка выполнения запроса");
		}
	}

	curl_global_cleanup();
	return result;
}

size_t Client::get_response_tostring(void* buffer, size_t size, size_t nmemb, void* param) {
	std::string& text = *static_cast<std::string*>(param);
	size_t totalsize = size * nmemb;
	text.append(static_cast<char*>(buffer), totalsize);
	return totalsize;
}
