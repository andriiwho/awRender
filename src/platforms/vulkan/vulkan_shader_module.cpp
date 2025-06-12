#include "vulkan_shader_module.h"

#include "vulkan_device.h"

namespace aw::render
{

	VulkanShaderModule::VulkanShaderModule(const std::span<core::u32> byte_code, const ShaderStage stage, const std::string& entry_point)
		: m_Stage(stage)
		, m_EntryPoint(entry_point)
	{
		const auto create_info = vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), byte_code.size_bytes(), byte_code.data());
		m_ShaderModule = g_vulkan_device->get_device().createShaderModule(create_info);
	}
} // namespace aw::render