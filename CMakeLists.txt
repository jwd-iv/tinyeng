cmake_minimum_required(VERSION 3.8)
include(ExternalProject)
enable_testing()

project(tiny)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    SET(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} /MTd")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    SET(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /MT")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

option(TINY_DOCS "Build tiny engine documentation"   ON)
option(TINY_TEST "Build tiny engine test executable" ON)
option(TINY_CORE "Build tiny core package"           ON)
option(TINY_GFX  "Build tiny graphics package"       ON)

include_directories(include)

add_subdirectory(src)

include_directories(${RIKU_SOURCE_DIR}/include)

if(TINY_TEST)
    add_subdirectory(test)
endif()

if(TINY_DOCS)
    add_subdirectory(docs)
endif()
