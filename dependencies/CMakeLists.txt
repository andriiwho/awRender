include(FetchContent)

if (NOT TARGET awCore)
    FetchContent_Declare(awCore GIT_REPOSITORY https://github.com/andriiwho/awCore)
    FetchContent_MakeAvailable(awCore)
endif ()

if (NOT TARGET glfw)
    FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw GIT_TAG 3.4)
    FetchContent_MakeAvailable(glfw)
endif ()

if (NOT TARGET fmt)
    FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt GIT_TAG 11.2.0)
    FetchContent_MakeAvailable(fmt)
endif ()

if (NOT TARGET VulkanMemoryAllocator)
    FetchContent_Declare(VulkanMemoryAllocator GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator GIT_TAG v3.3.0)
    FetchContent_MakeAvailable(VulkanMemoryAllocator)
endif ()