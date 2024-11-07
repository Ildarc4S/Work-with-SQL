#pragma once

#include <sqlite3.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class DatabaseHandler {
   public:
    DatabaseHandler(const std::string& db_name);
    ~DatabaseHandler();

    bool createTable(const std::string& table_name,
                     std::initializer_list<std::pair<std::string, std::string>> column_names);
    bool insert(const std::string& table_name,
                const std::initializer_list<std::pair<std::string, std::string>> values);

    bool select(const std::string& table_name, const std::initializer_list<std::string> columns = {"*"},
                const std::string& where_filter = "", const std::string& like_filter = "",
                const std::string& order_by = "", const int limit = -1,
                int (*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);

    bool selectUnique(const std::string& table_name, const std::initializer_list<std::string> columns = {"*"},
                      const std::string& where_filter = "", const std::string& like_filter = "",
                      const std::string& order_by = "", const int limit = -1,
                      int (*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);

    bool executeQuery(const std::string& query, int (*callback)(void*, int, char**, char**) = nullptr,
                      void* data = nullptr);

    bool sendPackage(const std::string& table_name, std::initializer_list<std::string> columns,
                     const std::vector<std::vector<std::string>>& records);
    bool packageQuery();
    bool optimizeDatabaseOn(const std::string& table_name, std::initializer_list<std::string> column_names);
    bool optimizeDatabaseOff(std::initializer_list<std::string> column_names);

private:
    sqlite3* data_base = nullptr;
    bool openDatabase(const std::string& db_name);
    void closeDatabase();
};