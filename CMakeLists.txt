cmake_minimum_required(VERSION 3.16)
project(miniProj01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include headers
include_directories(${PROJECT_SOURCE_DIR}/header)

# collect source files
file(GLOB_RECURSE SOURCES "source/*.cpp")

# create executable
add_executable(mP01 main.cpp ${SOURCES})