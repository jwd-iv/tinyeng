if(NOT EXISTS "${tiny_SOURCE_DIR}/../ext/ttvfs/CMakeLists.txt")
    configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/tiny_core-reqs/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tiny_core-reqs" )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tiny_core-reqs" )
endif()

if(NOT EXISTS "${tiny_SOURCE_DIR}/../ext/ttvfs/CMakeLists.txt")
    message(FATAL_ERROR "Unable to find or download ttvfs, something's gone wrong.")
endif()

set(TTVFS_BUILD_CFILEAPI  OFF CACHE BOOL " ")
set(TTVFS_BUILD_GENERATOR OFF CACHE BOOL " ")
add_subdirectory("${CMAKE_SOURCE_DIR}/../ext/ttvfs" "${CMAKE_CURRENT_BINARY_DIR}/ttvfs_build" EXCLUDE_FROM_ALL)
message(STATUS "ttvfs found and added to build!")

include_directories(.
                    ${CMAKE_SOURCE_DIR}/../ext/ttvfs/ttvfs
                    ${CMAKE_SOURCE_DIR}/../ext/ttvfs/ttvfs_zip)

file(GLOB_RECURSE TINY_CORE_src *.cpp)
file(GLOB_RECURSE TINY_CORE_hdr ../../include/core/*.h*)

add_library(tiny_core STATIC ${TINY_CORE_src} ${TINY_CORE_hdr}) 
set_target_properties(tiny_core PROPERTIES
    OUTPUT_NAME tiny_core
    DEBUG_POSTFIX -d
    FOLDER tiny
)

target_link_libraries(tiny_core tiny)
target_link_libraries(tiny_core ttvfs)
target_link_libraries(tiny_core ttvfs_zip)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/core
    DESTINATION include
)
install(
    TARGETS tiny_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
