cmake_minimum_required(VERSION 3.30)
project(awRender)

option(AWRENDER_USE_AWPK "Whether to use AWPK" OFF)

include(cmake/aw_render_macros.cmake)
add_subdirectory(dependencies)

add_library(awRender STATIC)
add_library(aw::Render ALIAS awRender)

file(GLOB_RECURSE renderFiles src/aw/*.cpp src/aw/*.h include/*.h LIST_DIRECTORIES ON)
target_sources(awRender PRIVATE ${renderFiles})

add_subdirectory(src/platforms)

target_link_libraries(awRender PUBLIC aw::Core)
target_compile_features(awRender PUBLIC cxx_std_23)
target_include_directories(awRender PUBLIC include PRIVATE src)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/generated/aw/render/misc)
configure_file(src/aw/render/aw_render_checks.h.in ${CMAKE_CURRENT_BINARY_DIR}/generated/aw/render/misc/aw_render_checks.h)
target_include_directories(awRender PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/generated)

if (AW_VULKAN_SUPPORTED)
    target_link_libraries(awRender PUBLIC awVulkanPlatform)
endif ()

macro(copy_slang_binary targetName)
    add_custom_command(TARGET ${targetName} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:slang> $<TARGET_FILE_DIR:${targetName}>/)
endmacro()

option(AWRENDER_INCLUDE_TESTBED "Whether to include the testbed project in the build" ON)
if (AWRENDER_INCLUDE_TESTBED)
    add_subdirectory(testbed)
endif ()