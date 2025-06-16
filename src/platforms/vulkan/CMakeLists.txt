add_library(awVulkanPlatform STATIC)

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)
target_link_libraries(awVulkanPlatform PUBLIC Vulkan::Vulkan aw::Render fmt::fmt PRIVATE glfw GPUOpen::VulkanMemoryAllocator Vulkan::shaderc_combined)

file(GLOB_RECURSE vulkanFiles *.h *.cpp *.inl LIST_DIRECTORIES ON)
target_sources(awVulkanPlatform PRIVATE ${vulkanFiles})
target_compile_features(awVulkanPlatform PRIVATE cxx_std_23)
target_include_directories(awVulkanPlatform PUBLIC ${CMAKE_CURRENT_LIST_DIR})