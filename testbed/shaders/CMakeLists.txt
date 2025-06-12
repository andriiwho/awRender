find_package(Vulkan QUIET COMPONENTS glslangValidator)

if (DEFINED Vulkan_GLSLANG_VALIDATOR_EXECUTABLE)
    message(STATUS "Found glslangValidator at ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}")
    add_custom_target(testbed_compile_shaders ALL)

    file(GLOB_RECURSE shaderFiles *.vert *.frag *.comp *.geom *.tesc *.tese LIST_DIRECTORIES ON)
    if (NOT IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
        execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_LIST_DIR}/bin)
    endif ()

    foreach (shaderFile ${shaderFiles})
        get_filename_component(shaderName ${shaderFile} NAME)
        add_custom_command(TARGET testbed_compile_shaders POST_BUILD
                COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} ARGS -V ${shaderFile} -o bin/${shaderName}.spv WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    endforeach ()
endif ()