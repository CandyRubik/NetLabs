cmake_minimum_required(VERSION 3.6)
project(Lab1)

set(CMAKE_CXX_STANDARD 20)

# using pkg-config to configure uuid
find_package(PkgConfig REQUIRED)
pkg_search_module(UUID REQUIRED uuid)

add_executable(app main.cpp ClientServerIPv4.cpp ClientServerIPv4.h ClientServer.cpp ClientServer.h ClientServerIPv6.cpp ClientServerIPv6.h)
target_include_directories(app PUBLIC ${UUID_INCLUDE_DIRS})
target_link_libraries(app ${UUID_LIBRARIES})
