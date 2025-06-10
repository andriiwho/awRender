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

		void present_swap_chain(ISwapChain* swap_chain) override;

		void submit(IDeviceCommandList* command_list, IDeviceFence* fence_to_signal) override;

	private:
		vk::Queue m_Queue;
		core::u32 m_QueueFamilyIndex;
		DeviceQueueType m_QueueType;

		core::RefPtr<IDeviceFence> m_SignalFence = nullptr;
		core::Vector<vk::Semaphore> m_PresentWaitSemaphores{};
	};
}