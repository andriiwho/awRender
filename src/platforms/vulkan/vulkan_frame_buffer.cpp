#include "vulkan_frame_buffer.h"

#include "vulkan_device.h"
#include "vulkan_image_view.h"
#include "vulkan_render_pass.h"
#include "aw/render/api/device_views.h"

namespace aw::render
{
	VulkanFrameBuffer::VulkanFrameBuffer(const FrameBufferCreateInfo& info)
	{
		collect_handles(info);
		if (m_ColorAttachments.empty())
		{
			if (m_DepthStencilAttachment)
			{
				collect_size(*m_DepthStencilAttachment);
			}
			else
			{
				throw std::runtime_error("No color attachments or depth stencil attachment provided");
			}
		}
		else
		{
			collect_size(m_ColorAttachments.at(0));
		}

		validate_size();

		std::vector<vk::ImageView> attachments;
		for (const auto& image_view : m_ColorAttachments)
		{
			attachments.push_back(image_view->get_handle());
		}

		if (m_DepthStencilAttachment)
		{
			attachments.push_back(m_DepthStencilAttachment.value()->get_handle());
		}

		const auto create_info = vk::FramebufferCreateInfo()
									 .setRenderPass(m_RenderPass->get_render_pass())
									 .setAttachments(attachments)
									 .setWidth(m_Width)
									 .setHeight(m_Height)
									 .setLayers(m_Layers);

		m_Framebuffer = g_vulkan_device->get_device().createFramebuffer(create_info);
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
	}

	void VulkanFrameBuffer::collect_handles(const FrameBufferCreateInfo& info)
	{
		for (const DeviceImageView* view : info.color_attachments)
		{
			const core::RefPtr vulkan_view = static_cast<VulkanImageView*>(const_cast<DeviceImageView*>(view));
			vulkan_view->add_ref();
			m_ColorAttachments.push_back(vulkan_view);
		}

		if (info.depth_stencil_attachment)
		{
			m_DepthStencilAttachment = core::RefPtr(static_cast<VulkanImageView*>(info.depth_stencil_attachment.value()));
			m_DepthStencilAttachment.value()->add_ref();
		}

		if (info.render_pass)
		{
			m_RenderPass = core::RefPtr(static_cast<VulkanRenderPass*>(info.render_pass));
			m_RenderPass->add_ref();
		}
	}

	void VulkanFrameBuffer::collect_size(const VulkanImageView* view)
	{
		if (!view)
		{
			throw std::runtime_error("Failed to deduce framebuffer size. No attachment is provided.");
		}

		if (const auto resource = view->get_image())
		{
			const auto& create_info = resource->get_create_info();
			m_Width = create_info.width;
			m_Height = create_info.height;
			m_Layers = view->get_create_info().array_layers;
		}
	}

	void VulkanFrameBuffer::validate_size() const
	{
		if (m_Width == 0 || m_Height == 0)
		{
			throw std::runtime_error("Failed to deduce framebuffer size. No attachment is provided.");
		}

		for (const auto& view : m_ColorAttachments)
		{
			if (const auto resource = view->get_image())
			{
				if (const auto& create_info = resource->get_create_info();
					create_info.width != m_Width || create_info.height != m_Height)
				{
					throw std::runtime_error("All color attachments must have the same size");
				}
			}
		}

		if (m_DepthStencilAttachment)
		{
			if (const auto resource = m_DepthStencilAttachment.value()->get_image())
			{
				if (const auto& create_info = resource->get_create_info();
					create_info.width != m_Width || create_info.height != m_Height)
				{
					throw std::runtime_error("The depth stencil attachment must have the same size as color attachments.");
				}
			}
		}
	}

	VulkanSwapChainFrameBuffer::VulkanSwapChainFrameBuffer(VulkanSwapChain* swap_chain, const IRenderPass* render_pass)
		: m_SwapChain(swap_chain)
	{
		m_SwapChain->add_ref();
		swap_chain->on_swap_chain_resized.add(this, &VulkanSwapChainFrameBuffer::on_swap_chain_resized);

		for (const auto& image_view : swap_chain->get_swap_chain_image_views())
		{
			vk::ImageView attachments = image_view->get_handle();
			const auto create_info = vk::FramebufferCreateInfo()
										 .setRenderPass(static_cast<const VulkanRenderPass*>(render_pass)->get_render_pass())
										 .setAttachments(attachments)
										 .setWidth(swap_chain->get_extent().width)
										 .setHeight(swap_chain->get_extent().height)
										 .setLayers(1);
			m_Framebuffers.push_back(g_vulkan_device->get_device().createFramebuffer(create_info));
		}
	}

	VulkanSwapChainFrameBuffer::~VulkanSwapChainFrameBuffer()
	{
		m_SwapChain->on_swap_chain_resized.remove(this);
	}

	DeviceImageView* VulkanSwapChainFrameBuffer::get_current_image_view() const
	{
		return m_SwapChain->get_swap_chain_image_views()[m_SwapChain->get_current_image_index()];
	}

	core::u32 VulkanSwapChainFrameBuffer::get_width() const
	{
		const vk::Extent2D extent = m_SwapChain->get_extent();
		return extent.width;
	}

	core::u32 VulkanSwapChainFrameBuffer::get_height() const
	{
		const vk::Extent2D extent = m_SwapChain->get_extent();
		return extent.height;
	}

	void VulkanSwapChainFrameBuffer::on_swap_chain_resized()
	{
	}
} // namespace aw::render