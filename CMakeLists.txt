cmake_minimum_required (VERSION 2.6)
project (satpi)

file(GLOB SOURCES "src/*.cpp")

add_executable(satpi ${SOURCES})
