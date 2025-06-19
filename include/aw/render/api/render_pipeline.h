#pragma once

#include "aw/core/all.h"

#include <span>
#include <string>

namespace aw::render
{
	class IDeviceShaderModule;

	enum class PrimitiveTopology : core::u8
	{
		point_list,
		line_list,
		line_strip,
		triangle_list,
		triangle_strip,
		triangle_fan,
		line_list_adj,
		line_strip_adj,
		triangle_list_adj,
		triangle_strip_adj,
		patch_list,
	};

	struct RenderPipelineCreateInfo
	{
		std::string debug_name{};
		std::span<IDeviceShaderModule*> shaders{};
		PrimitiveTopology primitive_topology{};

		core::usize get_hash_code() const noexcept;
	};

	class IRenderPipeline : public core::IntrusiveRefCounted
	{
	public:
		virtual core::usize get_hash_code() const = 0;
	};
}