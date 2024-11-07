#include "DatabaseHandler.hpp"

DatabaseHandler::DatabaseHandler(const std::string& db_name) { openDatabase(db_name); }
DatabaseHandler::~DatabaseHandler() { closeDatabase(); }

bool DatabaseHandler::createTable(const std::string& table_name,
                                  std::initializer_list<std::pair<std::string, std::string>> column_names) {
    std::ostringstream query;
    query << "CREATE TABLE " << table_name << " (";
    query << "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, ";
    for (auto iter = column_names.begin(); iter != column_names.end(); iter++) {
        query << iter->first << " " << iter->second;
        if (iter + 1 != column_names.end()) {
            query << ", ";
        }
    }
    query << ");";
    return executeQuery(query.str());
}

bool DatabaseHandler::insert(const std::string& table_name,
                             const std::initializer_list<std::pair<std::string, std::string>> values) {
    std::ostringstream query;
    query << "INSERT INTO " << table_name << " (";

    for (auto iter = values.begin(); iter != values.end(); iter++) {
        query << iter->first;
        if (iter + 1 != values.end()) {
            query << ", ";
        }
    }
    query << ") VALUES(";
    for (auto iter = values.begin(); iter != values.end(); iter++) {
        query << "'" << iter->second << "'";
        if (iter + 1 != values.end()) {
            query << ", ";
        }
    }

    query << ");";
    return executeQuery(query.str());
}

bool DatabaseHandler::select(const std::string& table_name, const std::initializer_list<std::string> columns,
                             const std::string& where_filter, const std::string& like_filter,
                             const std::string& order_by, const int limit,
                             int (*callback)(void*, int, char**, char**), void* data) {
    std::ostringstream query;
    query << "SELECT ";

    for (auto iter = columns.begin(); iter != columns.end(); ++iter) {
        query << *iter;
        if (std::next(iter) != columns.end()) {
            query << ", ";
        }
    }

    query << " FROM " << table_name;

    if (!where_filter.empty()) {
        query << " WHERE " << where_filter;
    }

    if (!like_filter.empty()) {
        if (where_filter.empty()) {
            query << " WHERE " << like_filter;
        } else {
            query << " AND " << like_filter;
        }
    }

    if (!order_by.empty()) {
        query << " ORDER BY " << order_by;
    }

    if (limit > -1) {
        query << " LIMIT " << limit;
    }

    query << ";";

    return executeQuery(query.str(), callback, data);
}

bool DatabaseHandler::selectUnique(const std::string& table_name,
                                   const std::initializer_list<std::string> columns,
                                   const std::string& where_filter, const std::string& like_filter,
                                   const std::string& order_by, const int limit,
                                   int (*callback)(void*, int, char**, char**), void* data) {
    std::ostringstream query;
    query << "SELECT DISTINCT ";

    for (auto iter = columns.begin(); iter != columns.end(); ++iter) {
        query << *iter;
        if (std::next(iter) != columns.end()) {
            query << ", ";
        }
    }

    query << " FROM " << table_name;

    if (!where_filter.empty()) {
        query << " WHERE " << where_filter;
    }

    if (!like_filter.empty()) {
        if (where_filter.empty()) {
            query << " WHERE " << like_filter;
        } else {
            query << " AND " << like_filter;
        }
    }

    if (!order_by.empty()) {
        query << " ORDER BY " << order_by;
    }

    if (limit > -1) {
        query << " LIMIT " << limit;
    }

    query << ";";

    return executeQuery(query.str(), callback, data);
}

bool DatabaseHandler::executeQuery(const std::string& query, int (*callback)(void*, int, char**, char**),
                                   void* data) {
    std::cout << query << std::endl;
    bool result = true;
    char* error_message = nullptr;
    if (sqlite3_exec(data_base, query.c_str(), callback, data, &error_message) != SQLITE_OK) {
        std::cerr << "ERROR: " << error_message << std::endl;
        sqlite3_free(error_message);
        result = false;
    }

    return result;
}

bool DatabaseHandler::sendPackage(const std::string& table_name, std::initializer_list<std::string> columns,
                                  const std::vector<std::vector<std::string>>& records) {
    if (!executeQuery("BEGIN TRANSACTION;")) {
        std::cerr << "Failed to start transaction.\n";
        return false;
    }

    sqlite3_stmt* stmt = nullptr;

    std::ostringstream query;
    query << "INSERT INTO " << table_name << " (";
    for (auto iter = columns.begin(); iter != columns.end(); ++iter) {
        query << *iter;
        if (iter + 1 != columns.end()) {
            query << ", ";
        }
    }
    query << ") VALUES (";
    for (size_t i = 0; i < columns.size(); ++i) {
        query << "?";
        if (i + 1 != columns.size()) {
            query << ", ";
        }
    }
    query << ");";

    if (sqlite3_prepare_v2(data_base, query.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement!\n";
        return false;
    }

    for (const auto& record : records) {
        int index = 1;
        for (const auto& value : record) {
            sqlite3_bind_text(stmt, index++, value.c_str(), -1, SQLITE_STATIC);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "SQLITE step error! " << sqlite3_errmsg(data_base) << "\n";
            sqlite3_finalize(stmt);
            executeQuery("ROLLBACK;");
            return false;
        }

        sqlite3_reset(stmt);
    }
    executeQuery("COMMIT;");
    sqlite3_finalize(stmt);

    return true;
}

bool DatabaseHandler::optimizeDatabaseOn(const std::string& table_name,
                                         std::initializer_list<std::string> column_names) {
    std::ostringstream query;
    query << "CREATE INDEX idx_";
    for (auto iter = column_names.begin(); iter != column_names.end(); iter++) {
        query << *iter;
        if (iter + 1 != column_names.end()) {
            query << "_";
        }
    }

    query << " ON " << table_name << " (";

    for (auto it = column_names.begin(); it != column_names.end(); ++it) {
        query << *it;
        if (std::next(it) != column_names.end()) {
            query << ", ";
        }
    }
    query << ");";
    return executeQuery(query.str());
}

bool DatabaseHandler::optimizeDatabaseOff(std::initializer_list<std::string> column_names) {
    std::ostringstream query;
    query << "DROP INDEX idx_";
    for (auto iter = column_names.begin(); iter != column_names.end(); iter++) {
        query << *iter;
        if (iter + 1 != column_names.end()) {
            query << "_";
        }
    }

    query << ";";
    return executeQuery(query.str());
}

bool DatabaseHandler::openDatabase(const std::string& db_name) {
    bool result = true;
    if (sqlite3_open(db_name.c_str(), &data_base) == SQLITE_OK) {
        std::cout << "Database is open!" << std::endl;
    } else {
        result = false;
        std::cerr << "Failed to open database!" << std::endl;
    }
    return result;
}

void DatabaseHandler::closeDatabase() {
    if (data_base != nullptr) {
        sqlite3_close(data_base);
        data_base = nullptr;
        std::cout << "Database is closed!" << std::endl;
    }
}