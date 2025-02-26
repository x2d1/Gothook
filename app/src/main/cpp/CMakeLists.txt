
cmake_minimum_required(VERSION 3.22.1)

project("native_hook")

add_library(got_hook
         SHARED
         got_hook.cpp)

add_library(hello
         SHARED
         hello.cpp)

target_link_libraries(got_hook android log)

target_link_libraries(hello android log)