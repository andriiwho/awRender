add_custom_target(awCompileShaders ALL)

find_program(SLANG_EXECUTABLE slangc)
if (NOT SLANG_EXECUTABLE)
    message(FATAL_ERROR "Slang executable not found")
else()
    message(STATUS "Slang executable found ${SLANG_EXECUTABLE}")
endif ()

macro(aw_compile_shader target_name shader_path)
    set(options ONLY_SPV)
    set(one_value_args SHADER_TARGET OUTPUT PROFILE ENTRY)
    set(multi_value_args NONE)

    cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    set(slangc_args)
    if(ARG_PROFILE)
        list(APPEND slangc_args -profile ${ARG_PROFILE})
    endif()

    if(ARG_ENTRY)
        list(APPEND slangc_args -entry ${ARG_ENTRY})
    endif()

    add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${SLANG_EXECUTABLE} ARGS ${shader_path} -o ${ARG_OUTPUT} -target ${ARG_SHADER_TARGET} ${slangc_args}
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
endmacro()