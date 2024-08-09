cmake_minimum_required(VERSION 3.10)
project(GP_Task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_executable(GP_Task
    main.cpp
    messageQueueMgmt.cpp
    Tasks.cpp
)
