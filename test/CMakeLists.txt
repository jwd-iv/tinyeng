add_executable(tiny_test main.cpp)

target_link_libraries(tiny_test tiny_core)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set_target_properties(tiny_test PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    set_target_properties(tiny_test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/game")
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tiny_test)
endif()

add_test(NAME tiny_launchtest COMMAND tiny_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/game)
