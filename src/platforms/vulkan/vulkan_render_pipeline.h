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
		void init_bindless_set_layout();
		void init_bindless_layout();


		RenderPipelineCreateInfo m_Info;

		// TODO: Descriptor set layout must be a global set layout, we literally don't need it to be unique per pipeline.
		vk::raii::DescriptorSetLayout m_BindlessDescriptorSetLayout{nullptr};

		// TODO: Same for the pipeline layout.
		vk::raii::PipelineLayout m_BindlessPipelineLayout{nullptr};
		vk::raii::Pipeline m_Pipeline{nullptr};
	};
}