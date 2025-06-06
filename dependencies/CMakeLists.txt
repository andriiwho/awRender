include(FetchContent)

if(NOT TARGET awCore)
    FetchContent_Declare(awCore GIT_REPOSITORY https://github.com/andriiwho/awCore)
    FetchContent_MakeAvailable(awCore)
endif()