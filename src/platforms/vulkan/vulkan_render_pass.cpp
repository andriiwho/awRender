#include "vulkan_render_pass.h"

#include "vulkan_device.h"
#include "vulkan_pixel_format.h"
#include "aw/render/api/device_resources.h"

#include <optional>

namespace aw::render
{
	static constexpr vk::ImageLayout to_vulkan_image_layout(const DeviceResourceState state)
	{
		switch (state)
		{
			case DeviceResourceState::undefined:
				return vk::ImageLayout::eUndefined;
			case DeviceResourceState::color_attachment:
				return vk::ImageLayout::eColorAttachmentOptimal;
				break;
			case DeviceResourceState::depth_attachment:
				return vk::ImageLayout::eDepthStencilAttachmentOptimal;
				break;
			case DeviceResourceState::depth_read_only:
				return vk::ImageLayout::eDepthReadOnlyOptimal;
				break;
			case DeviceResourceState::stencil_attachment:
				return vk::ImageLayout::eStencilAttachmentOptimal;
				break;
			case DeviceResourceState::stencil_read_only:
				return vk::ImageLayout::eStencilReadOnlyOptimal;
				break;
			case DeviceResourceState::depth_stencil_attachment:
				return vk::ImageLayout::eDepthStencilAttachmentOptimal;
				break;
			case DeviceResourceState::depth_stencil_read_only:
				return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
				break;
			case DeviceResourceState::depth_read_only_stencil:
				return vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal;
				break;
			case DeviceResourceState::shader_resource:
				return vk::ImageLayout::eShaderReadOnlyOptimal;
				break;
			case DeviceResourceState::copy_src:
				return vk::ImageLayout::eTransferSrcOptimal;
				break;
			case DeviceResourceState::copy_dst:
				return vk::ImageLayout::eTransferDstOptimal;
				break;
			case DeviceResourceState::present:
				return vk::ImageLayout::ePresentSrcKHR;
				break;
		}

		throw std::runtime_error("Invalid resource state");
	}

	static constexpr vk::AttachmentLoadOp to_vulkan_load_op(const ImageLoadOp op)
	{
		switch (op)
		{
			case ImageLoadOp::load:
				return vk::AttachmentLoadOp::eLoad;
				break;
			case ImageLoadOp::clear:
				return vk::AttachmentLoadOp::eClear;
				break;
			case ImageLoadOp::none:
				return vk::AttachmentLoadOp::eDontCare;
				break;
		}

		return vk::AttachmentLoadOp::eDontCare;
	}

	static constexpr vk::AttachmentStoreOp to_vulkan_store_op(const ImageStoreOp op)
	{
		switch (op)
		{
			case ImageStoreOp::store:
				return vk::AttachmentStoreOp::eStore;
			case ImageStoreOp::none:
				return vk::AttachmentStoreOp::eDontCare;
		}

		return vk::AttachmentStoreOp::eDontCare;
	}

	VulkanRenderPass::VulkanRenderPass(const std::string& debug_name)
		: m_DebugName(debug_name)
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
	}

	void VulkanRenderPass::build()
	{
		// Collect attachments
		std::vector<vk::AttachmentDescription> attachments;
		std::vector<vk::AttachmentReference> color_attachments;
		std::optional<vk::AttachmentReference> depth_stencil_attachment;

		for (const PassImageDesc& image : m_Images)
		{
			const auto attachment = vk::AttachmentDescription()
										.setFormat(to_vulkan_format(image.format))
										.setSamples(vk::SampleCountFlagBits::e1)
										.setLoadOp(to_vulkan_load_op(image.load_op))
										.setStoreOp(to_vulkan_store_op(image.store_op))
										.setStencilLoadOp(to_vulkan_load_op(image.stencil_load))
										.setStencilStoreOp(to_vulkan_store_op(image.stencil_store))
										.setInitialLayout(to_vulkan_image_layout(image.state_before_pass))
										.setFinalLayout(to_vulkan_image_layout(image.state_after_pass));

			const core::u32 ref_index = static_cast<core::u32>(attachments.size());
			attachments.push_back(attachment);

			const auto ref = vk::AttachmentReference()
								 .setAttachment(ref_index)
								 .setLayout(to_vulkan_image_layout(image.state_in_pass));

			switch (image.state_in_pass)
			{
				case DeviceResourceState::depth_attachment:
				case DeviceResourceState::depth_read_only:
				case DeviceResourceState::depth_stencil_attachment:
				case DeviceResourceState::depth_stencil_read_only:
				case DeviceResourceState::depth_read_only_stencil:
					depth_stencil_attachment = ref;
					break;
				case DeviceResourceState::color_attachment:
					color_attachments.push_back(ref);
					break;
				default:
					throw std::runtime_error("Cannot figure out the purpose of the render pass image.");
			}

			color_attachments.push_back(ref);
		}

		// FIXME: don't really care about dependencies atm.
		vk::SubpassDescription subpass;
		std::array dependencies = {
			vk::SubpassDependency()
				.setSrcSubpass(VK_SUBPASS_EXTERNAL)
				.setDstSubpass(0)
				.setSrcStageMask(vk::PipelineStageFlagBits::eTopOfPipe)
				.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
				.setSrcAccessMask(vk::AccessFlagBits::eMemoryRead)
				.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite),
			vk::SubpassDependency()
				.setSrcSubpass(0)
				.setDstSubpass(VK_SUBPASS_EXTERNAL)
				.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
				.setDstStageMask(vk::PipelineStageFlagBits::eBottomOfPipe)
				.setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
				.setDstAccessMask(vk::AccessFlagBits::eMemoryRead)
		};

		const auto create_info = vk::RenderPassCreateInfo()
									 .setAttachments(attachments)
									 .setSubpasses(subpass)
									 .setDependencies(dependencies);
		m_RenderPass = g_vulkan_device->get_device().createRenderPass(create_info);
	}

	void VulkanRenderPass::add_pass_image(const PassImageDesc& image_desc)
	{
		m_Images.push_back(image_desc);
	}
} // namespace aw::render