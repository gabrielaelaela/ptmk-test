//
// Created by 1 on 29.10.2025.
//

#include "DBHandler.h"

DBHandler::DBHandler(const std::string &dbName
    , const std::string &userName
    , const std::string &dbPassword
    , const std::string &dbHost
    , int dbPort)
        : conn_("dbname=" + dbName +
        " user=" + userName +
        " password=" + dbPassword +
        " hostaddr=" + dbHost +
        " port=" + std::to_string(dbPort)) {
    std::cout << "Connected to database" << std::endl;
}


void DBHandler::createEmployeesTable() {
    try {
        pqxx::work work(conn_);
        const std::string create_table_sql = "CREATE TABLE IF NOT EXISTS Employees ("
                                          "id SERIAL PRIMARY KEY, "
                                          "full_name TEXT NOT NULL, "
                                          "birthday DATE NOT NULL, "
                                          "sex TEXT CHECK (sex IN ('Male', 'Female')));";

        work.exec(create_table_sql);
        work.commit();
        std::cout << "Table created" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error creating table: " << e.what() << std::endl;
        throw;
    }
}

void DBHandler::insertNewEmployee(const Employee& e) {
    try {
        pqxx::work work(conn_);
        std::ostringstream query;
        query << "INSERT INTO Employees (full_name, birthday, sex) VALUES ("
            << work.quote(e.getName()) << ", "
            << work.quote(e.getBirthDay()) << ", "
            << work.quote(e.getSex()) << ");";
        work.exec(query.str());
        work.commit();

        std::cout << "Employee " << e.getName() << " inserted" << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Error creating new Employee: " << ex.what() << std::endl;
        throw;
    }
}

void DBHandler::getUnique(std::ostream& os) {
    try {
        pqxx::work work(conn_);
        const std::string query_sql = "SELECT DISTINCT ON (full_name, birthday) "
                                    "full_name, birthday, sex, "
                                    "EXTRACT(YEAR FROM AGE(birthday)) AS age "
                                    "FROM Employees";

        pqxx::result res = work.exec(query_sql);
        os << "Full Name \t\t\t\t| Birthday \t\t| Sex \t| Age\n";
        os << "------------------------------------------------------\n";

        for (const auto& row : res) {
            std::string name = row["full_name"].as<std::string>();
            std::string birthday = row["birthday"].as<std::string>();
            std::string sex = row["sex"].as<std::string>();

            const Employee e(name, birthday, sex);
            os << e;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error getting unique value: " << e.what() << std::endl;
    }
}

void DBHandler::insertBatch(const std::vector<Employee>& employees) {
    try {
        pqxx::work work(conn_);

        std::ostringstream query;
        query << "INSERT INTO Employees (full_name, birthday, sex) VALUES";

        for (size_t i = 0; i < employees.size(); ++i) {
            const auto& e = employees[i];
            query << "(" << work.quote(e.getName()) << ", "
            << work.quote(e.getBirthDay()) << ", "
            << work.quote(e.getSex()) << ")";

            if (i != employees.size() - 1)
                query << ", ";
        }

        query << ";";

        work.exec(query.str());
        work.commit();

        std::cout << "Batch of " << employees.size() << " inserted" << std::endl;

    } catch (std::exception &e) {
        std::cerr << "Error inserting batch: " << e.what() << std::endl;
    }
}

void DBHandler::getFiltered(std::ostream& os, const std::string& sex, const char firstLetter) {
    try {
        std::ostringstream query_sql;
        query_sql << "SELECT * FROM Employees WHERE full_name LIKE '" << firstLetter << "%' AND sex = '" << sex << "';";

        const auto start = std::chrono::high_resolution_clock::now();
        pqxx::work work(conn_);
        const pqxx::result res = work.exec(query_sql.str());
        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> elapsed = end - start;

        os << "Found " << res.size() << " rows\n";
        os << "Time taken: " << elapsed.count() << " seconds\n";

    } catch (const std::exception &e) {
        std::cerr << "Error getting filtered values: " << e.what() << std::endl;
    }

}

void DBHandler::getFilteredOptimized(std::ostream& os, const std::string& sex, const char firstLetter) {
    try {
        pqxx::work work(conn_);
        const std::string sql_query = "CREATE INDEX IF NOT EXISTS idx_employees_fullname_sex "
                                    "ON Employees (full_name text_pattern_ops, sex);";
        work.exec(sql_query);
        work.commit();

        getFiltered(os, sex, firstLetter);

    } catch (const std::exception &e) {
        std::cerr << "Error getting filtered values: " << e.what() << std::endl;
    }

}

DBHandler::~DBHandler() {
    if (conn_.is_open()) conn_.close();
}
