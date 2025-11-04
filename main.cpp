#include <iostream>
#include <vector>
#include <random>
#include "DBHandler.h"
#include "Employee.h"

std::vector<Employee> generateEmployees(int n = 10'000);
std::vector<Employee> generateEmployeesByFirstLetter(char c = 'F', const std::string& = "Male", int n = 100);

static const std::string filename = "db_config.ini";

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(const int argc, char* argv[]) {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    if (argc < 2) {
        std::cerr << "Usage: myApp <mode> [args...]" << std::endl;
        return -1;
    }

    DBHandler db(filename);

    switch (std::stoi(argv[1])) {
        case 1:
            db.createEmployeesTable();
            break;

        case 2: {
            if (argc != 5) {
                std::cerr << "Usage: myApp 2 <FIO> <birthDate> <Male|Female>\n";
                return -1;
            }

            const Employee e(argv[2], argv[3], argv[4]);
            db.insertNewEmployee(e);
            break;
        }

        case 3:
            db.getUnique(std::cout);
            break;

        case 4:
            for (int i = 0; i < 100; ++i) {
                db.insertBatch(generateEmployees());
            }
            db.insertBatch(generateEmployeesByFirstLetter());
            break;

        case 5:
            db.getFiltered();
            break;

        case 6:
            db.getFilteredOptimized();
            break;

        default:
            std::cerr << "Invalid mode" << std::endl;
            break;
        }

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}

std::vector<Employee> generateEmployees(const int n) {
    std::vector<Employee> employees;
    employees.reserve(n);

    std::vector<std::string> firstLetters;
    for (char c = 'A'; c <= 'Z'; ++c)
        firstLetters.emplace_back(1, c);

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> genderDist(0, 1);
    std::uniform_int_distribution<int> yearDist(1970, 2000);
    std::uniform_int_distribution<int> monthDist(1, 12);
    std::uniform_int_distribution<int> dayDist(1, 28);
    std::uniform_int_distribution<int> letterDist(0, 25);

    for (int i = 0; i < n; ++i) {
        std::string sex = genderDist(rng) ? "Male" : "Female";
        const char first = 'A' + letterDist(rng);
        std::string fullName = std::string(1, first);
        if (sex == "Male") fullName += " Ivan Ivanov";
        else fullName += " Maria Ivanovna";
        std::string birthDay = std::to_string(yearDist(rng)) + "-" +
                                std::to_string(monthDist(rng)) + "-" +
                                std::to_string(dayDist(rng));

        employees.emplace_back(fullName, birthDay, sex);
    }

    return employees;
}

std::vector<Employee> generateEmployeesByFirstLetter(const char c, const std::string& sex, const int n) {
    std::vector<Employee> employees;
    employees.reserve(n);

    for (int i = 0; i < n; ++i) {
        std::string fullName(1, c);
        if (sex == "Male") fullName += " Ivan Ivanov";
        else fullName += " Maria Ivanovna";

        std::string birthDay = "1985-01-01";
        employees.emplace_back(fullName, birthDay, sex);
    }

    return employees;
}

