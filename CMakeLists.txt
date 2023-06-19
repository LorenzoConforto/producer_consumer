cmake_minimum_required(VERSION 3.24)
project(producer_consumer)

set(CMAKE_CXX_STANDARD 11)

add_executable(producer_consumer main.cpp one_consumer_version.cpp multiple_consumer_version.cpp prova.cpp)
