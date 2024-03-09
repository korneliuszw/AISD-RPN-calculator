cmake_minimum_required(VERSION 3.25)
project(kalkulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(kalkulator
        functions.cpp
        functions.hpp
        list.hpp
        main.cpp
        onp.cpp
        onp.hpp
        optional.hpp
        stack.hpp
        tokenizer.cpp
        tokenizer.hpp
        tokens.cpp
        tokens.hpp)
