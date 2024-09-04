cmake_minimum_required(VERSION 3.10) 
project(SafeCPP)                    

set(CMAKE_CXX_STANDARD 20)           
set(CMAKE_CXX_STANDARD_REQUIRED ON)  

# Add the directory containing your source files
add_executable(SafeCPP main.cpp SafeCPP.h)
