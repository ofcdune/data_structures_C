cmake_minimum_required(VERSION 3.28)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_library(untitled SHARED stack.c
        alloc_s.h)
