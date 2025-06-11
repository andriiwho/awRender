#pragma once

#include "aw/render/system/shader_compiler.h"

namespace slang
{
	struct IGlobalSession;
}

namespace aw::render
{
	class VulkanShaderCompiler final : public IShaderCompiler
	{
	public:
		VulkanShaderCompiler();
		~VulkanShaderCompiler() override;

		IDeviceShaderModule* compile_shader(std::string_view path, std::string_view entry_point, ShaderStage stage) override;

	private:
		static slang::IGlobalSession* s_GlobalSession;
	};
}