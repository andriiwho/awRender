#pragma once

#include "aw/render/api/render_pipeline.h"
#include "vulkan_common.h"

namespace aw::render
{
	class VulkanRenderPipeline final : public IRenderPipeline
	{
	public:
		explicit VulkanRenderPipeline(RenderPipelineCreateInfo&& info);
		~VulkanRenderPipeline() override;

		core::usize get_hash_code() const override;

	private:
		RenderPipelineCreateInfo m_Info;
		vk::raii::PipelineLayout m_PipelineLayout{nullptr};
		vk::raii::Pipeline m_Pipeline{nullptr};
	};
}