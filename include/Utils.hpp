#pragma once

#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#define WITHOUT_F 0
#define WITH_F 1

class Utils {
   public:
    static std::string getRandomGender();

    static std::string getRandomDate(unsigned int begin_date, unsigned int end_date);
    static std::string getRandomFIO(int length, bool mode);
    static bool isValidName(const std::string& name);
    static bool isValidDate(const std::string& date);
    static bool isValidGender(const std::string& gender);
    static bool checkArgumentsValid(int argc, char* argv[]);

   private:
    static int checkDaysInMonth(int month, int year);
    static std::random_device rd;
    static std::mt19937 engine;
};