#pragma once

#include "aw/render/api/device_shader_module.h"
#include "vulkan_common.h"

#include <span>

namespace aw::render
{
	class VulkanShaderModule final : public IDeviceShaderModule
	{
	public:
		VulkanShaderModule(std::span<core::u32> byte_code, ShaderStage stage, const std::string& entry_point);
		vk::ShaderModule get_shader_module() const noexcept { return m_ShaderModule; }

		ShaderStage get_shader_stage() const override { return m_Stage; }
		std::string_view get_entry_point() const noexcept { return m_EntryPoint; }

	private:
		vk::raii::ShaderModule m_ShaderModule{nullptr};
		ShaderStage m_Stage{};
		std::string m_EntryPoint{};
	};
}