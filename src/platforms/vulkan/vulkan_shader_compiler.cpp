#include "vulkan_shader_compiler.h"
#include "vulkan_shader_module.h"
#include "slang/slang.h"

#include <fmt/printf.h>

namespace aw::render
{
	slang::IGlobalSession* VulkanShaderCompiler::s_GlobalSession;

	VulkanShaderCompiler::VulkanShaderCompiler()
	{
		if (!s_GlobalSession)
		{
			if (const SlangResult result = slang_createGlobalSession(SLANG_API_VERSION, &s_GlobalSession); !SLANG_SUCCEEDED(result))
			{
				throw std::runtime_error("Failed to create slang global session");
			}
		}

		fmt::println("Slang global session initialized.");
	}

	VulkanShaderCompiler::~VulkanShaderCompiler()
	{
		s_GlobalSession->release();
		slang_shutdown();
	}

	IDeviceShaderModule* VulkanShaderCompiler::compile_shader(std::string_view path, const std::string_view entry_point, const ShaderStage stage)
	{


		return aw_new VulkanShaderModule({}, ShaderStage::vertex, entry_point.data());
	}
} // namespace aw::render