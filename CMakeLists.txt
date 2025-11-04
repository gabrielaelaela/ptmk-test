cmake_minimum_required(VERSION 3.20)
project(testproject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(testproject main.cpp DBHandler.cpp DBHandler.h Employee.cpp Employee.h)

target_link_libraries(testproject PRIVATE -lpqxx -lpq)
