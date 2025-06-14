file(GLOB_RECURSE projectFiles src/*.h src/*.cpp src/*.inl LIST_DIRECTORIES ON)
add_executable(awRenderTestbed ${projectFiles})
target_link_libraries(awRenderTestbed PRIVATE aw::Render)

add_subdirectory(shaders)

run_awpk_for_target(awRenderTestbed)
add_dependencies(awRenderTestbed awTestbedShaders)
add_dependencies(awRenderTestbed_awpk awTestbedShaders)