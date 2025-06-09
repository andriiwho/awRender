#pragma once
#include "aw/render/api/device_queue.h"

#include "vulkan_common.h"

namespace aw::render
{
	enum class DeviceQueueType : core::u8;

	class VulkanDeviceQueue final : public IDeviceQueue
	{
	public:
		VulkanDeviceQueue(vk::Queue queue, core::u32 queue_family_index, DeviceQueueType queue_type);
		~VulkanDeviceQueue() override;

		void set_signal_fence_on_submit(IDeviceFence* fence) override;

	private:
		vk::Queue m_Queue;
		core::u32 m_QueueFamilyIndex;
		DeviceQueueType m_QueueType;

		core::RefPtr<IDeviceFence> m_SignalFence = nullptr;
	};
}