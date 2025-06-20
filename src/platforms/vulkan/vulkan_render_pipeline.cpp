#include "vulkan_render_pipeline.h"

#include "vulkan_device.h"
#include "vulkan_shader_module.h"
#include "vulkan_render_pass.h"

#include <array>

namespace aw::render
{
	class VulkanRenderPass;
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
		if (!info.compatible_render_pass)
		{
			throw std::runtime_error("Render pipeline must have a compatible render pass.");
		}

		init_bindless_set_layout();
		init_bindless_layout();

		std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;
		shader_stages.reserve(m_Info.shaders.size());

		for (const auto& shader : m_Info.shaders)
		{
			const VulkanShaderModule* vk_shader = static_cast<VulkanShaderModule*>(shader);
			shader_stages.emplace_back()
				.setStage(to_vk_shader_stage(shader->get_shader_stage()))
				.setModule(vk_shader->get_shader_module())

				// The entry point name for vulkan shaders is always 'main'
				.setPName("main");
		}

		// This will be empty because we are using bindless vertex input.
		constexpr auto vertex_input_state = vk::PipelineVertexInputStateCreateInfo();

		const auto input_assembly_state = vk::PipelineInputAssemblyStateCreateInfo()
											  .setTopology(to_vk_topology(m_Info.primitive_topology))
											  .setPrimitiveRestartEnable(false);

		const auto create_info = vk::GraphicsPipelineCreateInfo()
									 .setStages(shader_stages)
									 .setPVertexInputState(&vertex_input_state)
									 .setPInputAssemblyState(&input_assembly_state)
									 .setLayout(m_BindlessPipelineLayout)

									 .setRenderPass(static_cast<const VulkanRenderPass*>(info.compatible_render_pass)->get_render_pass());
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

	void VulkanRenderPipeline::init_bindless_set_layout()
	{
		std::array descriptor_types = {
			vk::DescriptorType::eUniformBuffer,
			vk::DescriptorType::eStorageBuffer,
			vk::DescriptorType::eCombinedImageSampler,
		};

		std::array<vk::DescriptorSetLayoutBinding, 3> bindings{};
		std::array<vk::DescriptorBindingFlags, 3> binding_flags{};
		for (core::u32 index = 0; index < 3; ++index)
		{
			bindings[index] = vk::DescriptorSetLayoutBinding()
								  .setBinding(index)
								  .setDescriptorType(descriptor_types[index])
								  .setDescriptorCount(1)
								  .setStageFlags(vk::ShaderStageFlagBits::eAll);
			binding_flags.at(index) = vk::DescriptorBindingFlagBits::ePartiallyBound | vk::DescriptorBindingFlagBits::eUpdateAfterBind;
		}

		const auto binding_flags_info = vk::DescriptorSetLayoutBindingFlagsCreateInfo()
											.setBindingFlags(binding_flags);

		const auto bindless_set_layout_info = vk::DescriptorSetLayoutCreateInfo()
												  .setBindings(bindings)
												  .setFlags(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool)
												  .setPNext(&binding_flags_info);

		// TODO: Again, this must be global. Probably should be inside the device.
		m_BindlessDescriptorSetLayout = g_vulkan_device->get_device().createDescriptorSetLayout(bindless_set_layout_info);
	}

	void VulkanRenderPipeline::init_bindless_layout()
	{
		vk::DescriptorSetLayout set_layouts[1] = { m_BindlessDescriptorSetLayout };
		const auto layout_info = vk::PipelineLayoutCreateInfo()
									 .setSetLayouts(set_layouts);

		m_BindlessPipelineLayout = g_vulkan_device->get_device().createPipelineLayout(layout_info);
	}
} // namespace aw::render