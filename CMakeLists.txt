cmake_minimum_required(VERSION 3.20.0)
project(webserver LANGUAGES CXX)

include_directories( include )
add_executable(webserver src/main.cpp)
