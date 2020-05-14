cmake_minimum_required(VERSION 3.14)
project(priority_queue)

set(CMAKE_CXX_STANDARD 11)

add_library(priority_queue INTERFACE)
target_include_directories(priority_queue INTERFACE include)
target_sources(priority_queue INTERFACE
        include/hara/PriorityQueue.h
        include/hara/PriorityQueueImpl.h)

add_executable(test main.cpp)
target_link_libraries(test priority_queue)