cmake_minimum_required(VERSION 3.20.0)
project(webserver LANGUAGES CXX)

include_directories( include )
file(GLOB SRCS src/*.cpp)
add_executable(webserver SRCS)
