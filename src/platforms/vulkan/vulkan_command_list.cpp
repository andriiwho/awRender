#include "vulkan_command_list.h"

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
				.setCommandBufferCount(1)
				.setLevel(vk::CommandBufferLevel::ePrimary));
	}

	VulkanCommandList::~VulkanCommandList()
	{
	}

	void VulkanCommandList::open()
	{
		// Reset command buffer
		current().reset();

		constexpr auto begin_info = vk::CommandBufferBeginInfo();
		current().begin(begin_info);
	}

	void VulkanCommandList::close()
	{
		current().end();
	}
} // namespace aw::render