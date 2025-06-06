include(FetchContent)

if (NOT TARGET awCore)
    FetchContent_Declare(awCore GIT_REPOSITORY https://github.com/andriiwho/awCore GIT_TAG release-0.1)
    FetchContent_MakeAvailable(awCore)
endif ()

if (NOT TARGET glfw)
    FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw GIT_TAG 3.4)
    FetchContent_MakeAvailable(glfw)
endif ()

if(NOT TARGET fmt)
    FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt GIT_TAG 11.2.0)
    FetchContent_MakeAvailable(fmt)
endif()