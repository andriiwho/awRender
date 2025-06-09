#include "vulkan_queue.h"

#include "vulkan_swap_chain.h"

namespace aw::render
{
	VulkanDeviceQueue::VulkanDeviceQueue(const vk::Queue queue, const core::u32 queue_family_index, const DeviceQueueType queue_type)
		: m_Queue(queue)
		, m_QueueFamilyIndex(queue_family_index)
		, m_QueueType(queue_type)
	{
	}

	VulkanDeviceQueue::~VulkanDeviceQueue()
	{
		m_Queue.waitIdle();
	}

	void VulkanDeviceQueue::set_signal_fence_on_submit(IDeviceFence* fence)
	{
		m_SignalFence = fence;
	}

	void VulkanDeviceQueue::present_swap_chain(ISwapChain* swap_chain)
	{
		const VulkanSwapChain* vulkan_swap_chain = static_cast<VulkanSwapChain*>(swap_chain);

		const vk::SwapchainKHR handle = vulkan_swap_chain->get_swap_chain();
		const core::u32 image_index = vulkan_swap_chain->get_current_image_index();

		const auto present_info = vk::PresentInfoKHR()
									  .setSwapchains(handle)
									  .setImageIndices(image_index)
									  .setWaitSemaphores(m_PresentWaitSemaphores);
		if (const vk::Result result = m_Queue.presentKHR(present_info); result != vk::Result::eSuccess)
		{
			throw std::runtime_error("Failed to present swap chain");
		}

		m_PresentWaitSemaphores.clear();
	}

	void VulkanDeviceQueue::submit(IDeviceCommandList* command_list)
	{
		// TODO: Figure out where to get wait and signal semaphores
	}
} // namespace aw::render