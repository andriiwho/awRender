#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	enum class ShaderStage : core::u8;
	class IDeviceShaderModule;

	class IShaderCompiler : public core::IntrusiveRefCounted
	{
	public:
		virtual IDeviceShaderModule* compile_shader(std::string_view path, std::string_view entry_point, ShaderStage stage) = 0;
	};
} // namespace aw::render