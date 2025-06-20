#ifdef AW_VULKAN_BACKEND
    #define AW_UniformBinding(binding, set) [[vk::binding(binding, set)]]
    #define AW_VertexID SV_VulkanVertexID
#else
    #define AW_UniformBinding(set, binding)
    #define AW_VertexID SV_VertexID
#endif