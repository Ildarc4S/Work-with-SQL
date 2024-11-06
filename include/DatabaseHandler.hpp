#pragma once

#include <sqlite3.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class DatabaseHandler {
public:
    DatabaseHandler(const std::string& db_name);
    ~DatabaseHandler();

    void createTable(const std::string& table_name,
                std::initializer_list<
                    std::pair<std::string, std::string>
                > column_names);
    void insert(const std::string& table_name,
                             const std::initializer_list<
                                    std::pair<std::string, std::string>
                                   >values);

    bool select(const std::string& table_name,
                             const std::initializer_list<std::string> columns = {"*"},
                             const std::string& where_filter = "",
                             const std::string& like_filter = "",
                             const std::string& order_by = "",
                             const int limit = -1,
                             int (*callback)(void*, int, char**, char**) = nullptr,
                             void* data = nullptr);

    bool selectUnique(const std::string& table_name,
                             const std::initializer_list<std::string> columns = {"*"},
                             const std::string& where_filter = "",
                             const std::string& like_filter = "",
                             const std::string& order_by = "",
                             const int limit = -1,
                             int (*callback)(void*, int, char**, char**) = nullptr,
                             void* data = nullptr);

    bool executeQuery(const std::string& query, int (*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);

    bool sendPackage(const std::string& table_name, 
                                  std::initializer_list<std::string> columns,
                                  const std::vector<std::vector<std::string>>& records);
    bool packageQuery();
    bool optimizeDatabaseOn(const std::string& table_name, 
                          std::initializer_list<std::string> column_names);
    bool optimizeDatabaseOff(const std::string& table_name, 
                          std::initializer_list<std::string> column_names);

public:
    sqlite3* data_base = nullptr;
    bool openDatabase(const std::string& db_name);
    void closeDatabase();
};

// class Staff {
// public:
//     Staff(const std::string& full_name, const std::string& birth_date, const std::string& gender);

//     std::string getFullName();
//     std::string getBirthDate();
//     std::string getGender();

//     int calcAgeEmployee();
//     bool saveToDatabase(bool mode); // mode - отправка пакета или только одного запроса

// private:
//     std::string full_name;
//     std::string birth_date;
//     std::string gender;
//     DatabaseHandler& db_handler;
// };