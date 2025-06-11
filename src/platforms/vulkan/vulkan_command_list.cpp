#include "vulkan_command_list.h"

#include "vulkan_buffer.h"
#include "vulkan_device.h"
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
		const vk::BufferCopy region{0, 0, size};
		transfer().copyBuffer(static_cast<VulkanBuffer*>(from)->get_handle(), static_cast<VulkanBuffer*>(to)->get_handle(), region);
	}

	void VulkanCommandList::open_transfer()
	{
		transfer().reset();
		transfer().begin({});

		m_TransferOpen = true;
	}
} // namespace aw::render