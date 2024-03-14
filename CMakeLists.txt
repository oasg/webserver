cmake_minimum_required(VERSION 3.25.0)
project(webserver LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 23)
include_directories( include )
file(GLOB SRCS src/*)

add_executable(webserver ${SRCS})


find_package(PkgConfig)
pkg_check_modules(MySQL REQUIRED)
target_include_directories(webserver PRIVATE ${MySQL_INCLUDE_DIRS})
target_link_libraries(webserver PRIVATE ${MySQL_LIBRARIES})
