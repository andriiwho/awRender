#include "vulkan_shader_compiler.h"
#include "vulkan_shader_module.h"

#include <fmt/printf.h>

namespace aw::render
{
	VulkanShaderCompiler::VulkanShaderCompiler()
	{
	}

	VulkanShaderCompiler::~VulkanShaderCompiler()
	{
	}

	IDeviceShaderModule* VulkanShaderCompiler::compile_shader(const std::string_view path, const std::string_view entry_point, const ShaderStage stage)
	{
		auto shader_binary = core::file::read_file_to_binary(path);
		if (shader_binary.empty())
		{
			throw std::runtime_error(fmt::format("Failed to read shader file: {}", path));
		}

		return aw_new VulkanShaderModule(std::span(
			reinterpret_cast<core::u32*>(shader_binary.data()),
			shader_binary.size() / sizeof(core::u32)),
			stage,
			entry_point.data());
	}
} // namespace aw::render