//
// Created by 1 on 29.10.2025.
//

#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <string>
#include <ctime>

class Employee {
public:
    enum class Sex {Male, Female};

    Employee() = delete;
    Employee(const std::string& fullName, const std::string& date, const std::string& sex);

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getBirthDay() const;
    [[nodiscard]] std::string getSex() const;

    [[nodiscard]] unsigned int getAge() const;

    friend std::ostream& operator<< (std::ostream&, const Employee&);

private:
    std::string fullName_;
    std::string birthDay_;
    Sex sex_;
};

std::ostream& operator<<(std::ostream& os, const Employee& e);

#endif //EMPLOYEE_H
