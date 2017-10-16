cmake_minimum_required(VERSION 3.8)
include(CMakeDependentOption)
enable_testing()

project(tiny)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    SET(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} /MTd")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    SET(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /MT")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

option(TINY_DOCS "Build tiny engine documentation" ON)
option(TINY_CORE "Build tiny core package"         ON)
CMAKE_DEPENDENT_OPTION(TINY_TEST "Build tiny engine test executable" ON  "TINY_CORE" OFF)

include_directories(include)

add_subdirectory(src)

if(TINY_TEST)
    add_subdirectory(test)
endif()

if(TINY_DOCS)
    add_subdirectory(docs)
endif()
