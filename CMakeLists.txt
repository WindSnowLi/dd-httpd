cmake_minimum_required(VERSION 3.20)
project(httpd)

set(CMAKE_CXX_STANDARD 17)
file(GLOB source_files "*.hpp")

add_executable(httpd ${source_files} main.cpp)
target_link_libraries(httpd ws2_32)