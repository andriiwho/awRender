#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	enum class ShaderStage : core::u8;
	class IDeviceShaderModule;

	class IShaderLoader : public core::IntrusiveRefCounted
	{
	public:
		virtual IDeviceShaderModule* compile_shader(std::string_view shader_file_path, std::string_view entry_point, ShaderStage stage) = 0;

		virtual void clear_cache() = 0;
	};
} // namespace aw::render