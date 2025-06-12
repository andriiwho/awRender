#pragma once

#include "aw/render/system/shader_compiler.h"

namespace aw::render
{
	class VulkanShaderLoader final : public IShaderLoader
	{
	public:
		IDeviceShaderModule* compile_shader(core::IFileReader* file_reader, std::string_view entry_point, ShaderStage stage) override;

	private:
		std::mutex m_LibraryMutex;
		static std::unordered_map<std::string, core::RefPtr<IDeviceShaderModule>> m_Shaders;
	};
}