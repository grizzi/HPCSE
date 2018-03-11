cmake_minimum_required(VERSION 3.9)
project(openmp_tutorials)

set(CMAKE_CXX_STANDARD 11)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(hello hello.cpp)
add_executable(omp_for_example omp_for_example.cpp)
add_executable(omp_sections_example omp_sections_example.cpp)
add_executable(omp_critical_example omp_critical_example.cpp)