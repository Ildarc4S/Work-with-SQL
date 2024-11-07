#include "Staff.hpp"
#include "Timer.hpp"
#include "Utils.hpp"

void appRun(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    appRun(argc, argv);
    return 0;
}

void appRun(int argc, char* argv[]) {
    if (Utils::checkArgumentsValid(argc, argv)) {
        DatabaseHandler db_handler("../database/test.db");
        if (*argv[1] == '1') {
            db_handler.createTable("Staff", {{"full_name", "TEXT NOT NULL"},
                                             {"birth_date", "TEXT NOT NULL"},
                                             {"gender", "TEXT NOT NULL"}});
        } else if (*argv[1] == '2') {
            Staff employee(argv[2], argv[3], argv[4]);
            employee.saveToDatabase(SINGLE_REQUEST_SEND, db_handler, &employee);
        } else if (*argv[1] == '3') {
            std::vector<Staff> staff = Staff::getUniqueFromDatabase(db_handler);
            for (auto& employee : staff) {
                std::cout << employee.getFullName() << " " << employee.getBirthDate() << " "
                          << employee.getGender() << " " << employee.getAge() << " year" << std::endl;
            }
        } else if (*argv[1] == '4') {
            std::vector<std::vector<std::string>> stt;
            for (int i = 0; i < 1'000'000; i++) {
                stt.emplace_back(Staff(Utils::getRandomFIO(5, WITHOUT_F), Utils::getRandomDate(1950, 2010),
                                       Utils::getRandomGender())
                                     .convertObjectToVectorString());
            }
            std::cout << "1'000'000 objects created!" << std::endl;
            for (int i = 0; i < 100; i++) {
                stt.emplace_back(Staff(Utils::getRandomFIO(5, WITH_F), Utils::getRandomDate(1950, 2010),
                                       Utils::getRandomGender())
                                     .convertObjectToVectorString());
            }
            std::cout << "100 objects with 'F' created!" << std::endl;
            Staff::saveToDatabase(PACKAGE_REQUEST_SEND, db_handler, nullptr, stt);
        } else if (*argv[1] == '5') {
            std::cout << "------Without optimization!-----" << std::endl;
            Timer timer;
            std::vector<Staff> otv = Staff::getFromDatabase(db_handler);
            std::cout << "Time: " << timer.getMicroseconds() << "ms\n";
        } else if (*argv[1] == '6') {
            Staff::optimizeDatabaseOn(db_handler);
            std::cout << "-----With optimization!-----" << std::endl;
            Timer timer;
            std::vector<Staff> otv = Staff::getFromDatabase(db_handler);
            std::cout << "Time: " << timer.getMicroseconds() << "ms\n";
            Staff::optimizeDatabaseOff(db_handler);
        }
    }
}