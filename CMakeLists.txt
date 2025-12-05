cmake_minimum_required(VERSION 3.8)
project(Lab4OSSync)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(manager STATIC
    manager/manager.cpp
    manager/manager.h
)

add_executable(Receiver
    Receiver/Receiver.cpp
    Receiver/Receiver.h
)
target_link_libraries(Receiver PUBLIC manager)

add_executable(Sender
    Sender/Sender.cpp
    Sender/Sender.h
)
target_link_libraries(Sender PUBLIC manager)

if (WIN32)
    target_link_libraries(Receiver PUBLIC ws2_32)
    target_link_libraries(Sender PUBLIC ws2_32)
endif()
