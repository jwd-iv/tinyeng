include_directories(${CMAKE_SOURCE_DIR}/include/)

file(GLOB_RECURSE TINY_src *.cpp)
file(GLOB_RECURSE TINY_hdr ${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h*)

add_library(tiny STATIC ${TINY_src} ${TINY_hdr})
set_target_properties(tiny PROPERTIES
    OUTPUT_NAME tiny
    DEBUG_POSTFIX -d
    FOLDER tiny
)
target_link_libraries(tiny RIKU)
set(install_libs ${install_libs} tiny)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include
    DESTINATION .
    FILES_MATCHING PATTERN "**.h*"
)

install(
    TARGETS ${install_libs}
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION bin
)
