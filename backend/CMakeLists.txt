cmake_minimum_required(VERSION 3.15)
project(ProgramHelper)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(Boost REQUIRED COMPONENTS system)
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(ProgramHelper main.cpp)
target_link_libraries(ProgramHelper Boost::system ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)