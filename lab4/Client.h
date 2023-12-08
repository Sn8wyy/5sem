#pragma once

#define CURL_STATICLIB

#include "curl/curl.h"

#include <iostream>
#include <vector>
#include <string>

class Client {
public:
	static std::string get_request(std::string input, std::string MAIN_URL);

private:
	static size_t get_response_tostring(void* buffer, size_t size, size_t nmemb, void* param);
};

