if(NOT EXISTS "${tiny_SOURCE_DIR}/../ext/AntTweakBar/CMakeLists.txt")
  # Download and unpack AntTweakBar at configure time
  configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/tinyed-reqs/CMakeLists.txt)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tinyed-reqs" )
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .                 WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tinyed-reqs" )
endif()

if(NOT EXISTS "${tiny_SOURCE_DIR}/../ext/AntTweakBar/CMakeLists.txt")
  message(FATAL_ERROR "Unable to find or download AntTweakBar!!")
endif()
add_definitions(-DTW_STATIC)
add_subdirectory("${tiny_SOURCE_DIR}/../ext/AntTweakBar" "${CMAKE_BINARY_DIR}/AntTweakBar_build")
message(STATUS "AntTweakBar found and added to build!")

include_directories(SYSTEM ${tiny_SOURCE_DIR}/../ext/AntTweakBar/include)

file(GLOB_RECURSE TINY_EDITOR_src *.cpp)
file(GLOB_RECURSE TINY_EDITOR_hdr ../../include/tiny/editor/*.h*)

add_library(tiny-editor STATIC ${TINY_EDITOR_src} ${TINY_EDITOR_hdr}) 
set_target_properties(tiny-editor PROPERTIES
  OUTPUT_NAME tiny-editor
  DEBUG_POSTFIX -d
  FOLDER tiny
)

target_link_libraries(tiny-editor tiny)
target_link_libraries(tiny-editor AntTweakBarStatic)

install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/tiny/editor
  DESTINATION include/tiny
)
install(
  TARGETS tiny-editor
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
