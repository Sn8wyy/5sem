#pragma execution_character_set("utf-8")

#include "Tg_ETU.h"

int main() {
    std::ostringstream conn_string("");
    conn_string << "host=" << "("YOUR HOST"
        << " user=" << "YOUR USER"
        << " password=" << "YOUR PASSWORD"
        << " dbname=" << "YOUR BDNAME";
    Tg_ETU("YOUR TOKEN", conn_string);

    return 0;
}