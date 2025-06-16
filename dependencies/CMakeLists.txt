include(FetchContent)

if (NOT TARGET awCore)
    add_subdirectory(awCore)
endif ()

if (NOT TARGET glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
    add_subdirectory(glfw)
endif ()

if (NOT TARGET fmt)
    add_subdirectory(fmt)
endif ()

if (NOT TARGET VulkanMemoryAllocator)
    add_subdirectory(VulkanMemoryAllocator)
endif ()