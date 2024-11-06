#pragma once

#include <string>
#include <random>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <sstream>

#define WITHOUT_F 0
#define WITH_F 1

class Utils {
public:
    static std::string getRandomGender();

    static std::string getRandomDate(unsigned int begin_date, unsigned int end_date);
    static std::string getRandomFIO(int length, bool mode);

private:
    static int checkDaysInMonth(int month, int year);
    static std::random_device rd;
    static std::mt19937 engine;
};

bool isValidName(const std::string& name);
bool isValidDate(const std::string& date);
bool isValidGender(const std::string& gender);