#pragma once

#include "aw/render/api/device_shader_module.h"
#include "vulkan_common.h"

#include <span>

namespace aw::render
{
	class VulkanShaderModule final : public IDeviceShaderModule
	{
	public:
		VulkanShaderModule(std::span<core::u32> byte_code, ShaderStage stage, const core::String& entry_point);

		ShaderStage get_shader_stage() const override { return m_Stage; }

	private:
		vk::raii::ShaderModule m_ShaderModule{nullptr};
		ShaderStage m_Stage{};
		core::String m_EntryPoint{};
	};
}