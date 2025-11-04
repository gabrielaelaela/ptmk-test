//
// Created by 1 on 29.10.2025.
//

#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "Employee.h"

class DBHandler {
public:
    DBHandler(const std::string& dbName, const std::string& userName, const std::string& dbPassword, const std::string& dbHost, int dbPort);
    DBHandler(const std::string& filename);

    void createEmployeesTable(); //mode 1
    void insertNewEmployee(const Employee&); //mode 2
    void getUnique(std::ostream& = std::cout); //mode 3
    void insertBatch(const std::vector<Employee>&); //mode 4
    void getFiltered(std::ostream& os = std::cout, const std::string& sex = "Male", char firstLetter = 'F'); //mode 5
    void getFilteredOptimized(std::ostream& os = std::cout, const std::string& sex = "Male", char firstLetter = 'F'); //mode 6


    ~DBHandler();

private:
    pqxx::connection conn_;

    static std::string initParser(const std::string& filename);
    static inline void trim(std::string &s);
};

#endif //DBHANDLER_H
