//
// Created by 1 on 31.10.2025.
//

#include "Employee.h"

#include <iomanip>
#include <ostream>
#include <sstream>


Employee::Employee(const std::string& fullName
                   , const std::string &date
                   , const std::string& sex): fullName_(fullName) ,birthDay_(date) {
    if (std::toupper(sex[0]) == 'F') sex_ = Sex::Female;
    else if (std::toupper(sex[0]) == 'M') sex_ = Sex::Male;
    else throw std::invalid_argument("Invalid sex value. Usage: <Male|Female>");
}

std::string Employee::getName() const {
    return fullName_;
}

std::string Employee::getBirthDay() const {
    return birthDay_;
}

std::string Employee::getSex() const {
    return (sex_ == Sex::Female) ? std::string("Female") : std::string("Male");
}

unsigned int Employee::getAge() const {
    const std::time_t now = std::time(nullptr);
    const std::tm* now_tm = std::localtime(&now);

    std::tm tmBirthdate = {};
    std::stringstream ss(birthDay_);
    ss >> std::get_time(&tmBirthdate, "%Y-%m-%d");
    if (ss.fail()) {
        throw std::invalid_argument("Invalid birthDate format");
    }
    unsigned int age = now_tm->tm_year - tmBirthdate.tm_year;
    if (now_tm->tm_yday < tmBirthdate.tm_yday) --age;
    return age;
}

std::ostream& operator<<(std::ostream& os, const Employee& e) {
    os << e.getName()
       << "\t| " << e.getBirthDay()
       << "\t| " << e.getSex()
       << "\t| " << e.getAge() << std::endl;
    return os;
}


