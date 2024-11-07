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
    if (mode == WITHOUT_F) {
        result += static_cast<char>('A' + dist(engine));
    } else if (mode == WITH_F) {
        result += 'F';
    } else {
        mode_valid = false;
        std::cerr << "Not found mode!" << std::endl;
    }
    for (int i = 1; i < length && mode_valid; i++) {
        result += static_cast<char>('a' + dist(engine));
    }

    for (int i = 0; i < 2; i++) {
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
        case 4:
        case 6:
        case 9:
        case 11:
            result = 30;
            break;
    }
    return result;
}

bool Utils::isValidName(const std::string& name) {
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

bool Utils::isValidDate(const std::string& date) {
    bool result = true;
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        result = false;
    }

    for (size_t i = 0; i < date.size(); i++) {
        if ((i != 4 && i != 7) && !isdigit(date[i])) {
            result = false;
        }
    }

    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (month < 1 || month > 12 || day < 1 || day > 31) {
        result = false;
    }

    switch (month) {
        case 2:
            if (day > 29) result = false;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (day > 30) result = false;
            break;
    }

    return result;
}

bool Utils::isValidGender(const std::string& gender) { return gender == "Male" || gender == "Female"; }


bool Utils::checkArgumentsValid(int argc, char* argv[]) {
    
    if (argc < 2 || argc > 5) {
        std::cout << "Invalid number of arguments!" << std::endl;
        return false;
    }

    int mode = atoi(argv[1]);
    if (mode < 1 || mode > 6) {
        std::cout << "Invalid mode. Please use mode between 1 and 6." << std::endl;
        return false;
    }

    if (mode == 2 && argc != 5) {
        std::cout << "Usage for mode 2: myApp 2 \"full_name\" \"birth_date\" \"gender\"" << std::endl;
        return false;
    }

    if (mode == 2) {
        std::string fullName = argv[2];
        if (!Utils::isValidName(fullName)) {
            std::cout << "Invalid full name format. Use \"last_name first_name middle_name\"." << std::endl;
            return false;
        }

        std::string birthDate = argv[3];
        if (!Utils::isValidDate(birthDate)) {
            std::cout << "Invalid birth date format. Use \"YYYY-MM-DD\"." << std::endl;
            return false;
        }

        std::string gender = argv[4];
        if (!Utils::isValidGender(gender)) {
            std::cout << "Gender must be \"Male\" or \"Female\"." << std::endl;
            return false;
        }
    }
    return true;
}