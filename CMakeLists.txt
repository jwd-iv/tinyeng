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

option(TINY_CORE "Build tiny_core library" ON)
option(TINY_TEST "Build tiny engine test executable" ON)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../install CACHE PATH "default install path" FORCE)
endif()

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR lib)
endif()
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../RIKU/CMakeLists.txt")
    # Download and unpack RIKU at configure time
    configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/tiny-reqs/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tiny-reqs" )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tiny-reqs" )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/../RIKU/CMakeLists.txt")
    message(STATUS "RIKU found and added to build!")
    include_directories(${CMAKE_SOURCE_DIR}/../RIKU/include/)
    set(RIKU_TEST OFF CACHE BOOL " ")
    add_subdirectory("${CMAKE_SOURCE_DIR}/../RIKU" "${CMAKE_CURRENT_BINARY_DIR}/riku_build")
else()
    message(FATAL_ERROR "Unable to find or download RIKU, something's gone wrong.")
endif()

include_directories(include/)
add_subdirectory(tiny)
if(TINY_CORE)
    add_subdirectory(tiny_core)
endif()
if(TINY_TEST)
    add_subdirectory(test)
endif()
