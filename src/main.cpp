#include "Utils.hpp"
#include "Staff.hpp"
#include "Timer.hpp"
#include <filesystem>

int checkArgumentsValid(int argc, char* argv[]) {
    if (argc < 2 || argc > 5) {
        std::cout << "Invalid number of arguments!" << std::endl;
        return 0;
    }

    int mode = atoi(argv[1]);
    if (mode < 1 || mode > 6) {
        std::cout << "Invalid mode. Please use mode between 1 and 6." << std::endl;
        return 0;
    }

    if (mode == 2 && argc != 5) {
        std::cout << "Usage for mode 2: myApp 2 \"full_name\" \"birth_date\" \"gender\"" << std::endl;
        return 0;
    }

    if (mode == 2) {
        std::string fullName = argv[2];
        if (!isValidName(fullName)) {
            std::cout << "Invalid full name format. Use \"last)name first_name middle_name\"." << std::endl;
            return 0;
        }

        std::string birthDate = argv[3];
        if (!isValidDate(birthDate)) {
            std::cout << "Invalid birth date format. Use \"YYYY-MM-DD\"." << std::endl;
            return 0;
        }

        std::string gender = argv[4];
        if (!isValidGender(gender)) {
            std::cout << "Gender must be \"Male\" or \"Female\"." << std::endl;
            return 0;
        }
    }
    return 1;
}

void appRun(int argc, char* argv[]) {
    if(checkArgumentsValid(argc, argv)) {
        DatabaseHandler db_handler("../database/test.db");
        if(*argv[1] == '1') {
            db_handler.createTable("Staff", {
                {"full_name", "TEXT NOT NULL"},
                {"birth_date", "TEXT NOT NULL"},
                {"gender", "TEXT NOT NULL"}
            });
        } else if (*argv[1] == '2') {
            std::cout << "send\n";
            Staff employee(argv[2], argv[3], argv[4]);
            employee.saveToDatabase(SINGLE_REQUEST_SEND, db_handler, &employee);
        } else if (*argv[1] == '3') {
            std::vector<Staff> staff = Staff::getUniqueFromDatabase(db_handler);
            for(auto& employee : staff) {
                std::cout << employee.getFullName() << " " << employee.getBirthDate() << " " << employee.getGender() << " " << employee.calcAge() << " year" << std::endl;
            }
        } else if (*argv[1] == '4') {
            std::vector<std::vector<std::string>> stt;
            for(int i = 0; i < 1'000'000; i++) {
                stt.emplace_back(Staff(Utils::getRandomFIO(5, WITHOUT_F), Utils::getRandomDate(1950, 2010), Utils::getRandomGender()).convertObjectToVectorString());
            }
            std::cout << "1'000'000 objects created!" << std::endl;
            for(int i = 0; i < 100; i++) {
                stt.emplace_back(Staff(Utils::getRandomFIO(5, WITH_F), Utils::getRandomDate(1950, 2010), Utils::getRandomGender()).convertObjectToVectorString());
            }
            std::cout << "100 objects with 'F' created!" << std::endl;
            Staff::saveToDatabase(PACKAGE_REQUEST_SEND, db_handler, nullptr, stt);
        } else if (*argv[1] == '5') {
            std::cout << "Without optimization!" << std::endl;
            Timer timer;
            std::vector<Staff> otv = Staff::getFromDatabase(db_handler);
            std::cout << "Time: " << timer.getMicroseconds() << "ms\n";
        } else if (*argv[1] == '6') {
            Staff::optimizeDatabaseOn(db_handler);
            std::cout << "With optimization!" << std::endl;
            Timer timer;
            std::vector<Staff> otv = Staff::getFromDatabase(db_handler);
            std::cout << "Time: " << timer.getMicroseconds() << "ms\n";
            Staff::optimizeDatabaseOff(db_handler);
        }
    }
}

int main(int argc, char* argv[]) {
    appRun(argc, argv);
    return 0;
} 