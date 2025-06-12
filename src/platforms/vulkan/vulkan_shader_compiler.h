#pragma once

#include "aw/render/system/shader_compiler.h"

namespace aw::render
{
	class VulkanShaderCompiler final : public IShaderCompiler
	{
	public:
		VulkanShaderCompiler();
		~VulkanShaderCompiler() override;

		IDeviceShaderModule* compile_shader(core::IFileReader* file_reader, std::string_view entry_point, ShaderStage stage) override;
	};
}