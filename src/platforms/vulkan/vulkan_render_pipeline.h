#pragma once

#include "aw/render/api/render_pipeline.h"

namespace aw::render
{
	class VulkanRenderPipeline final : public IRenderPipeline
	{
	public:
		explicit VulkanRenderPipeline(RenderPipelineCreateInfo&& info);
		~VulkanRenderPipeline() override;

		core::usize get_hash_code() const override;

	private:
	};
}