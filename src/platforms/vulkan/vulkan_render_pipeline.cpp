#include "vulkan_render_pipeline.h"

#include "vulkan_device.h"
#include "vulkan_shader_module.h"

namespace aw::render
{
	static vk::ShaderStageFlagBits to_vk_shader_stage(ShaderStage stage)
	{
		switch (stage)
		{
			case ShaderStage::vertex:
				return vk::ShaderStageFlagBits::eVertex;
			case ShaderStage::fragment:
				return vk::ShaderStageFlagBits::eFragment;
			case ShaderStage::compute:
				return vk::ShaderStageFlagBits::eCompute;
			case ShaderStage::geometry:
				return vk::ShaderStageFlagBits::eGeometry;
			case ShaderStage::tess_control:
				return vk::ShaderStageFlagBits::eTessellationControl;
			case ShaderStage::tess_eval:
				return vk::ShaderStageFlagBits::eTessellationEvaluation;
		}

		throw std::runtime_error("Unknown shader stage.");
	}

	static vk::PrimitiveTopology to_vk_topology(PrimitiveTopology topology)
	{
		switch (topology)
		{
			case PrimitiveTopology::point_list:
				return vk::PrimitiveTopology::ePointList;
			case PrimitiveTopology::line_list:
				return vk::PrimitiveTopology::eLineList;
			case PrimitiveTopology::line_strip:
				return vk::PrimitiveTopology::eLineStrip;
			case PrimitiveTopology::triangle_list:
				return vk::PrimitiveTopology::eTriangleList;
			case PrimitiveTopology::triangle_strip:
				return vk::PrimitiveTopology::eTriangleStrip;
			case PrimitiveTopology::triangle_fan:
				return vk::PrimitiveTopology::eTriangleFan;
			case PrimitiveTopology::line_list_adj:
				return vk::PrimitiveTopology::eLineListWithAdjacency;
			case PrimitiveTopology::line_strip_adj:
				return vk::PrimitiveTopology::eLineStripWithAdjacency;
			case PrimitiveTopology::triangle_list_adj:
				return vk::PrimitiveTopology::eTriangleListWithAdjacency;
			case PrimitiveTopology::triangle_strip_adj:
				return vk::PrimitiveTopology::eTriangleStripWithAdjacency;
			case PrimitiveTopology::patch_list:
				return vk::PrimitiveTopology::ePatchList;
		}

		throw std::runtime_error("Unknown primitive topology.");
	}

	VulkanRenderPipeline::VulkanRenderPipeline(RenderPipelineCreateInfo&& info)
		: m_Info(std::move(info))
	{
		std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;
		shader_stages.reserve(m_Info.shaders.size());

		for (const auto& shader : m_Info.shaders)
		{
			const VulkanShaderModule* vk_shader = static_cast<VulkanShaderModule*>(shader);
			shader_stages.emplace_back()
				.setStage(to_vk_shader_stage(shader->get_shader_stage()))
				.setModule(vk_shader->get_shader_module())
				.setPName(vk_shader->get_entry_point().data());
		}

		const auto input_assembly_state = vk::PipelineInputAssemblyStateCreateInfo()
			.setTopology(to_vk_topology(m_Info.primitive_topology))
			.setPrimitiveRestartEnable(false);

		const auto create_info = vk::GraphicsPipelineCreateInfo()
			.setStages(shader_stages)
			.setPVertexInputState(nullptr)
			.setPInputAssemblyState(&input_assembly_state);
		m_Pipeline = g_vulkan_device->get_device().createGraphicsPipeline(
			g_vulkan_device->get_pipeline_cache(),
			create_info);
	}

	VulkanRenderPipeline::~VulkanRenderPipeline()
	{
	}

	core::usize VulkanRenderPipeline::get_hash_code() const
	{
		return m_Info.get_hash_code();
	}
} // namespace aw::render