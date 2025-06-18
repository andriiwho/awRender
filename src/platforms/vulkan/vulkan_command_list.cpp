#include "vulkan_command_list.h"

#include "vulkan_buffer.h"
#include "vulkan_device.h"
#include "vulkan_frame_buffer.h"
#include "vulkan_render_pass.h"
#include "vulkan_swap_chain.h"

namespace aw::render
{
	VulkanCommandList::VulkanCommandList()
	{
		const auto pool_info = vk::CommandPoolCreateInfo()
								   .setQueueFamilyIndex(g_vulkan_device->get_graphics_queue_index())
								   .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
		m_CommandPool = g_vulkan_device->get_device().createCommandPool(pool_info);
		m_CommandBuffer = g_vulkan_device->get_device().allocateCommandBuffers(
			vk::CommandBufferAllocateInfo()
				.setCommandPool(m_CommandPool)
				.setCommandBufferCount(2)
				.setLevel(vk::CommandBufferLevel::ePrimary));
	}

	VulkanCommandList::~VulkanCommandList()
	{
	}

	void VulkanCommandList::open()
	{
		DeviceCommandList::open();

		m_TransferOpen = false;
		m_HasAnyTransferCommands = false;

		// Reset command buffer
		cmd().reset();

		constexpr auto begin_info = vk::CommandBufferBeginInfo();
		cmd().begin(begin_info);
	}

	void VulkanCommandList::close()
	{
		cmd().end();

		if (has_any_transfer_commands())
		{
			transfer().end();
		}

		DeviceCommandList::close();
	}

	void VulkanCommandList::copy_buffer(DeviceBuffer* from, DeviceBuffer* to, core::u64 size)
	{
		if (!m_TransferOpen)
		{
			open_transfer();
		}

		m_HasAnyTransferCommands = true;
		const vk::BufferCopy region{ 0, 0, size };
		transfer().copyBuffer(static_cast<VulkanBuffer*>(from)->get_handle(), static_cast<VulkanBuffer*>(to)->get_handle(), region);
	}

	void VulkanCommandList::begin_render_pass(IRenderPass* render_pass, IFrameBuffer* frame_buffer)
	{
		if (!!m_State.current_render_pass)
		{
			throw std::runtime_error("Tried to start render pass in the middle of another render pass.");
		}

		const vk::RenderPass pass_handle = static_cast<VulkanRenderPass*>(render_pass)->get_render_pass();
		const vk::Framebuffer frame_buffer_handle = [frame_buffer]() -> vk::Framebuffer {
			if (frame_buffer->get_frame_buffer_type() == FrameBufferType::normal)
			{
				return static_cast<VulkanFrameBuffer*>(frame_buffer)->get_handle();
			}

			if (frame_buffer->get_frame_buffer_type() == FrameBufferType::swap_chain)
			{
				return static_cast<VulkanSwapChainFrameBuffer*>(frame_buffer)->get_handle();
			}

			throw std::runtime_error("Invalid frame buffer type.");
		}();

		const auto clear_values = static_cast<VulkanRenderPass*>(render_pass)->get_clear_values();
		const auto begin_info = vk::RenderPassBeginInfo()
									.setRenderPass(pass_handle)
									.setFramebuffer(frame_buffer_handle)
									.setRenderArea({ { 0, 0 }, { frame_buffer->get_width(), frame_buffer->get_height() } })
									.setClearValues(clear_values);
		cmd().beginRenderPass(begin_info, vk::SubpassContents::eInline);
		m_State.current_render_pass = render_pass;
		m_State.current_frame_buffer = frame_buffer;
	}

	void VulkanCommandList::end_render_pass()
	{
		cmd().endRenderPass();
		m_State.current_render_pass = nullptr;
		m_State.current_frame_buffer = nullptr;
	}

	void VulkanCommandList::open_transfer()
	{
		transfer().reset();
		transfer().begin({});

		m_TransferOpen = true;
	}
} // namespace aw::render