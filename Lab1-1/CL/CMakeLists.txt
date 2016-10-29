cmake_minimum_required(VERSION 3.6)
project(CL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES beta1.cpp)
add_executable(CL ${SOURCE_FILES})