cmake_minimum_required(VERSION 3.28)
project(cpu_monitor)

add_executable(cpu_monitor cpu_monitor.c)
target_link_libraries(cpu_monitor)
