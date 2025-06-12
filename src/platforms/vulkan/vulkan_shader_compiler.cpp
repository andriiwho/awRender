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

	IDeviceShaderModule* VulkanShaderCompiler::compile_shader(core::IFileReader* file_reader, const std::string_view entry_point, const ShaderStage stage)
	{
		auto shader_binary = file_reader->read_binary();
		if (shader_binary.empty())
		{
			throw std::runtime_error(fmt::format("Failed to read shader binary from file: {}", file_reader->get_path()));
		}

		return aw_new VulkanShaderModule(std::span(
			reinterpret_cast<core::u32*>(shader_binary.data()),
			shader_binary.size() / sizeof(core::u32)),
			stage,
			entry_point.data());
	}
} // namespace aw::render