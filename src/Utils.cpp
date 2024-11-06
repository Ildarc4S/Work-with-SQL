#include "Utils.hpp"

std::random_device Utils::rd;
std::mt19937 Utils::engine(Utils::rd());

std::string Utils::getRandomGender() {
    std::uniform_int_distribution<int> distGender(0, 1);
    return distGender(engine) ? "Male" : "Female";
}

std::string Utils::getRandomDate(unsigned int begin_date, unsigned int end_date) {
    std::uniform_int_distribution<int> distYear(begin_date, end_date);
    int year = distYear(engine);

    std::uniform_int_distribution<int> distMonth(1, 12);
    int month = distMonth(engine);

    int days_in_month = checkDaysInMonth(month, year);
    std::uniform_int_distribution<int> distDay(1, days_in_month);
    int day = distDay(engine);

    char result[11];
    snprintf(result, sizeof(result), "%04d-%02d-%02d", year, month, day);
    return std::string(result);
}

std::string Utils::getRandomFIO(int length, bool mode) {
    bool mode_valid = true;
    std::uniform_int_distribution<int> dist(0, 25);

    std::string result;
    if(mode == WITHOUT_F) {
        result += static_cast<char>('A' + dist(engine));
    } else if(mode == WITH_F) {
        result += 'F';
    } else {
        mode_valid = false;
        std::cerr << "Not found mode!" << std::endl;
    }
    for (int i = 1; i < length && mode_valid; i++) {
        result += static_cast<char>('a' + dist(engine));
    }

    for(int i = 0; i < 2; i++) {
        result += ' ';
        result += static_cast<char>('A' + dist(engine));
        for (int i = 1; i < length && mode_valid; i++) {
            result += static_cast<char>('a' + dist(engine));
        }
    }
    return result;
}

int Utils::checkDaysInMonth(int month, int year) {
    int result = 31;
    switch (month) {
        case 2:
            result = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            result = 30;
            break;
    }
    return result;
}

bool isValidName(const std::string& name) {
    std::istringstream stream(name);
    std::string part;
    int count = 0;

    while (stream >> part) {
        for (char c : part) {
            if (!isalpha(c) && c != '-') {
                return false; 
            }
        }
        count++;
    }
    return count == 3;
}

bool isValidDate(const std::string& date) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (int i = 0; i < date.length(); ++i) {
        if ((i != 4 && i != 7) && !isdigit(date[i])) {
            return false;
        }
    }

    return true;
}

bool isValidGender(const std::string& gender) {
    return gender == "Male" || gender == "Female";
}
