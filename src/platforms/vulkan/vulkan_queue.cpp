#include "vulkan_queue.h"

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
} // namespace aw::render