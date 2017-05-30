include_directories(./)
include_directories(${CMAKE_SOURCE_DIR}/../ext/ttvfs/ttvfs)
include_directories(${CMAKE_SOURCE_DIR}/../ext/ttvfs/ttvfs_zip)

file(GLOB_RECURSE TINY_CORE_src *.cpp)
file(GLOB_RECURSE TINY_CORE_hdr **.h*)

add_library(tiny_core STATIC ${TINY_CORE_src} ${TINY_CORE_hdr})
set_target_properties(tiny_core PROPERTIES
    OUTPUT_NAME tiny_core
    DEBUG_POSTFIX -d
    FOLDER tiny_core
)
set(install_libs ${install_libs} tiny_core)

if(EXISTS "${CMAKE_SOURCE_DIR}/../ext/ttvfs/CMakeLists.txt")
    set(TTVFS_BUILD_CFILEAPI OFF CACHE BOOL " ")
    set(TTVFS_BUILD_GENERATOR OFF CACHE BOOL " ")

    add_subdirectory("${CMAKE_SOURCE_DIR}/../ext/ttvfs" "${CMAKE_CURRENT_BINARY_DIR}/ttvfs_build" EXCLUDE_FROM_ALL)
else()
    message(FATAL_ERROR "ttvfs not found, please download to ext/ttvfs in the same folder as tiny.")
endif()

target_link_libraries(tiny_core tiny)
target_link_libraries(tiny_core ttvfs)
target_link_libraries(tiny_core ttvfs_zip)

install(
    TARGETS ${install_libs}
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION bin
)
