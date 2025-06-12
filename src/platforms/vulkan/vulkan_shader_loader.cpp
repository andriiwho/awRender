#include "vulkan_shader_loader.h"
#include "vulkan_shader_module.h"

#include <fmt/printf.h>

namespace aw::render
{
	std::unordered_map<std::string, core::RefPtr<IDeviceShaderModule>> VulkanShaderLoader::m_Shaders;

	IDeviceShaderModule* VulkanShaderLoader::compile_shader(core::IFileReader* file_reader, const std::string_view entry_point, const ShaderStage stage)
	{
		{
			std::lock_guard lock(m_LibraryMutex);
			if (const auto iter = m_Shaders.find(std::string(file_reader->get_path())); iter != m_Shaders.end())
			{
				return iter->second.get();
			}
		}

		auto shader_binary = file_reader->read_binary();
		if (shader_binary.empty())
		{
			throw std::runtime_error(fmt::format("Failed to read shader binary from file: {}", file_reader->get_path()));
		}

		{
			const core::RefPtr out_module = aw_new VulkanShaderModule(
				std::span(
					reinterpret_cast<core::u32*>(shader_binary.data()),
					shader_binary.size() / sizeof(core::u32)),
				stage,
				entry_point.data());

			// Add shader to the library and return
			std::lock_guard lock(m_LibraryMutex);
			m_Shaders[std::string(file_reader->get_path())] = out_module;
			return out_module.get();
		}
	}
} // namespace aw::render