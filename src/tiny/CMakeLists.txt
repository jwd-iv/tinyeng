if(NOT EXISTS "${TINY_SOURCE_DIR}/../RIKU/CMakeLists.txt")
    configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/tiny-reqs/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tiny-reqs" )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tiny-reqs" )
endif()

if(NOT EXISTS "${TINY_SOURCE_DIR}/../RIKU/CMakeLists.txt")
    message(FATAL_ERROR "Unable to find or download RIKU, something's gone wrong.")
endif()

set(RIKU_TEST OFF CACHE BOOL " ")
add_subdirectory("${TINY_SOURCE_DIR}/../RIKU" "${CMAKE_CURRENT_BINARY_DIR}/riku_build")
message(STATUS "RIKU found and added to build!")

include_directories(${RIKU_SOURCE_DIR}/include/)
include_directories(${TINY_SOURCE_DIR}/include/)

file(GLOB_RECURSE TINY_src *.cpp)
file(GLOB_RECURSE TINY_h ${TINY_SOURCE_DIR}/include/tiny.h)
file(GLOB_RECURSE TINY_hdrs ${TINY_SOURCE_DIR}/include/tiny/*.h*)

add_library(tiny STATIC ${TINY_src} ${TINY_h} ${TINY_hdrs})
set_target_properties(tiny PROPERTIES
    OUTPUT_NAME tiny
    DEBUG_POSTFIX -d
    FOLDER tiny
)
target_link_libraries(tiny RIKU)

install(
    FILES ${PROJECT_SOURCE_DIR}/include/tiny.h
    DESTINATION include
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/tiny
    DESTINATION include
)

install(
    TARGETS tiny
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
