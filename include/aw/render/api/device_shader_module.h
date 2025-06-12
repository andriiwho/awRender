#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	enum class ShaderStage : core::u8
	{
		vertex,
		fragment,
		compute,
		geometry,
		tess_control,
		tess_eval,
	};

	class IDeviceShaderModule : public core::IntrusiveRefCounted
	{
	public:
		virtual ShaderStage get_shader_stage() const = 0;
	};
}