cmake_minimum_required(VERSION 3.17)
project(Kursach)

set(CMAKE_CXX_STANDARD 14)

add_executable(Kursach WIN32 framework.h resource.h main.cpp 3D.h 3D.cpp arena.cpp)