#pragma once

#include "aw/core/all.h"

#include <span>
#include <string>

namespace aw::render
{
	class IDeviceShaderModule;

	struct RenderPipelineCreateInfo
	{
		std::string debug_name{};
		std::span<IDeviceShaderModule*> shaders{};
	};

	class IRenderPipeline : public core::IntrusiveRefCounted
	{
	public:
		virtual core::usize get_hash_code() const = 0;
	};
}