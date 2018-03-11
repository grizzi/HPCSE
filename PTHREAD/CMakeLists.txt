cmake_minimum_required(VERSION 3.9)
project(pthread)


SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")

add_executable(pthread hello.c)

add_executable(mutexes_ex mutexes_ex.c)

add_executable(join_example join_example.c)
target_link_libraries(join_example m)        # includes the math library

add_executable(condition_variables_ex condition_variables_ex.c)