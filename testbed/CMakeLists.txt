file(GLOB_RECURSE projectFiles src/*.h src/*.cpp src/*.inl LIST_DIRECTORIES ON)
add_executable(awRenderTestbed ${projectFiles})
target_link_libraries(awRenderTestbed PRIVATE aw::Render)
run_awpk_for_target(awRenderTestbed)
