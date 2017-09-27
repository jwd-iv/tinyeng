add_subdirectory(tiny)

if(TINY_CORE)
    add_subdirectory(core)
endif()

if(TINY_GFX)
    add_subdirectory(gfx)
endif()
