cmake_minimum_required(VERSION 3.24)
project(HW3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(HW3
        Page.cc
        Page.h
        PageTable.cc
        PageTable.h
        PageTemplates.cc
        test_page.cc
        test_pagetable.cc
        test_suite.cc
        test_suite.h)
