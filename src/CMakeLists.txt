add_subdirectory(tiny)

include_directories(${RIKU_SOURCE_DIR}/include)

if(TINY_CORE)
    add_subdirectory(core)
endif()

if(TINY_GFX)
    add_subdirectory(gfx)
endif()
