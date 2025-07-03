cmake_minimum_required(VERSION 3.28)
project(udp_server LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network PrintSupport)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(udp_server
        main.cpp
        udp_server.cpp
        qcustomplot.cpp
        udp_server.h
)

target_link_libraries(udp_server
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::PrintSupport
)

find_package(QCustomPlot)
if(TARGET QCustomPlot::QCustomPlot)
    target_link_libraries(udp_server QCustomPlot::QCustomPlot)
else()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot.h")
        target_sources(udp_server PRIVATE qcustomplot.cpp)
        target_include_directories(udp_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        message(FATAL_ERROR "QCustomPlot not found! Install it or copy files to the project.")
    endif()
endif()
