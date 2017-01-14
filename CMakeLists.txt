cmake_minimum_required(VERSION 3.6)
project(AISDI_Graph)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
set(SOURCE_FILES main.cpp Graph.cpp Graph.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})