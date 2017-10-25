#download AntTweakBar
#set build variables
#add lib and include directory

file(GLOB_RECURSE TINY_EDITOR_src *.cpp)
file(GLOB_RECURSE TINY_EDITOR_hdr ../../include/tiny/editor/*.h*)

add_library(tiny-editor STATIC ${TINY_EDITOR_src} ${TINY_EDITOR_hdr}) 
set_target_properties(tiny-editor PROPERTIES
  OUTPUT_NAME tiny-editor
  DEBUG_POSTFIX -d
  FOLDER tiny
)

target_link_libraries(tiny-editor tiny)
#target_link_libraries(tiny-editor AntTweakBar)

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
