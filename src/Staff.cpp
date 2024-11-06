#include "Staff.hpp"
#include <chrono>

Staff::Staff(const std::string& full_name, const std::string& birth_date, const std::string& gender)
    : full_name{full_name}
    , birth_date{birth_date}
    , gender{gender}
    {}

std::string Staff::getFullName() { return full_name; }
std::string Staff::getBirthDate() { return birth_date; }
std::string Staff::getGender() { return gender; }

int Staff::calcAge() {
    unsigned int day, month;
    int year;
    char dash_one, dash_two;
    std::istringstream date(birth_date);

    date >> year >> dash_one >> month >>  dash_two >> day; 
    std::chrono::year_month_day birth_date{ 
        std::chrono::year{year}, 
        std::chrono::month{month}, 
        std::chrono::day{day} 
    };

    std::chrono::year_month_day current_date = {
        std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now()
            )
    };

    int age = (current_date.year() - birth_date.year()).count();
    if (current_date < std::chrono::year_month_day{birth_date.year(), birth_date.month(), birth_date.day()}) {
        age--;
    }

    return age;
}

void Staff::saveToDatabase(bool mode, DatabaseHandler& db_handler, const Staff* object, const std::vector<std::vector<std::string>>& objects) {
    if(mode == SINGLE_REQUEST_SEND && object != nullptr) {
        db_handler.insert("staff", {
            {"full_name", object->full_name}, {"birth_date", object->birth_date}, {"gender", object->gender}
        });
    }
    else if (mode == PACKAGE_REQUEST_SEND && objects.size() != 0) {
        db_handler.sendPackage("staff", {"full_name", "birth_date", "gender"}, objects);
    } else {
        std::cerr << "Incorrect parametrs!" << std::endl;
    }
}

int Staff::callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<Staff>* names = static_cast<std::vector<Staff>*>(data);
    names->emplace_back(Staff(argv[0], argv[1], argv[2])); 
    return 0;
}

std::vector<Staff> Staff::getUniqueFromDatabase(DatabaseHandler& db_handlere) {
    std::vector<Staff> staff;
    db_handlere.select("Staff", {"full_name", "birth_date", "gender"}, "", "", "full_name" , -1, callback, &staff);
    return staff;
}

std::vector<Staff> Staff::getFromDatabase(DatabaseHandler& db_handlere) {
    std::vector<Staff> staff;
    db_handlere.select("Staff", {"full_name", "birth_date", "gender"}, "gender = 'Male'", "full_name LIKE 'F%'", "", -1, callback, &staff);
    return staff;
}

void Staff::optimizeDatabaseOn(DatabaseHandler& db_handler) {
    db_handler.optimizeDatabaseOn("Staff", {"full_name", "gender"});
}

void Staff::optimizeDatabaseOff(DatabaseHandler& db_handler) {
    db_handler.optimizeDatabaseOff("Staff", {"full_name", "gender"});
}

std::vector<std::string> Staff::convertObjectToVectorString() {
    return {getFullName(), getBirthDate(), getGender()};
}