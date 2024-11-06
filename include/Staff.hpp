#pragma once

#include "DatabaseHandler.hpp"

#define SINGLE_REQUEST_SEND 0
#define PACKAGE_REQUEST_SEND 1

class Staff {
public:
    Staff(const std::string& full_name, const std::string& birth_date, const std::string& gender);

    std::string getFullName();
    std::string getBirthDate();
    std::string getGender();

    int calcAge();
    static void saveToDatabase(bool mode, DatabaseHandler& db_handler, const Staff* object = nullptr, const std::vector<std::vector<std::string>>& objects = {}); // mode = SINGLE_REQUEST_SEND  or PACKAGE_REQUEST_SEND
    static std::vector<Staff> getFromDatabase(DatabaseHandler& db_handlere);
    static std::vector<Staff> getUniqueFromDatabase(DatabaseHandler& db_handlere);
    static int callback(void* data, int argc, char** argv, char** azColName);
    std::vector<std::string> convertObjectToVectorString();
    static void optimizeDatabaseOn(DatabaseHandler& db_handler);
    static void optimizeDatabaseOff(DatabaseHandler& db_handler);
private:
    std::string full_name;
    std::string birth_date;
    std::string gender;
};