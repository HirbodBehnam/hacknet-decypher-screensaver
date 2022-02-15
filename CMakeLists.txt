cmake_minimum_required(VERSION 3.19)
project(hacknet-decypher-screensaver C)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
set(CMAKE_C_STANDARD 11)

add_executable(hacknet-decypher-screensaver main.c)
target_link_libraries(hacknet-decypher-screensaver ${CURSES_LIBRARIES})