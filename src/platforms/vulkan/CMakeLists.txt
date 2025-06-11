add_library(awVulkanPlatform STATIC)

find_package(Vulkan REQUIRED)
target_link_libraries(awVulkanPlatform PUBLIC Vulkan::Vulkan aw::Render fmt::fmt PRIVATE glfw GPUOpen::VulkanMemoryAllocator)

file(GLOB_RECURSE vulkanFiles *.h *.cpp *.inl LIST_DIRECTORIES ON)
target_sources(awVulkanPlatform PRIVATE ${vulkanFiles})
target_compile_features(awVulkanPlatform PRIVATE cxx_std_23)
target_include_directories(awVulkanPlatform PUBLIC ${CMAKE_CURRENT_LIST_DIR})


#
# Link to SLANG
#
if (NOT IS_DIRECTORY $ENV{VULKAN_SDK})
    message(FATAL_ERROR "VulkanSDK environment variable is not set. Please set the environment variable")
endif ()

if (WIN32)
    target_link_directories(awVulkanPlatform PUBLIC $ENV{VULKAN_SDK}/Lib)
    target_include_directories(awVulkanPlatform PRIVATE $ENV{VULKAN_SDK}/Include)
endif ()

target_link_libraries(awVulkanPlatform PUBLIC
        $<$<CONFIG:Debug>:slangd>
        $<$<CONFIG:RelWithDebInfo>:slangd>
        $<$<CONFIG:Release>:slang>
        $<$<CONFIG:MinSizeRel>:slang>)