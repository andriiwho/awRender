#include "aw/render/api/render_pipeline.h"

#include "aw/render/api/device_shader_module.h"

namespace aw::render
{
	core::usize RenderPipelineCreateInfo::get_hash_code() const noexcept
	{
		core::usize out_hash_code = 0x12345ff;
		for (const IDeviceShaderModule* shader_module : shaders)
		{
			out_hash_code = core::hash_combine(out_hash_code, reinterpret_cast<core::u64>(shader_module));
		}

		out_hash_code = core::hash_combine(out_hash_code, static_cast<core::usize>(primitive_topology));

		return out_hash_code;
	}
} // namespace aw::render